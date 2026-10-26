cmake_minimum_required(VERSION 3.20)

# Native SDL3 port of DOS Rayman (GOG v1.21) built from the historical reconstruction. See port/README.md.
#
# Ownership:
#   ../src/            historical reconstruction: authoritative, matched against RAYMAN.EXE, never edited for the port
#   derive/            reviewed patches for the few historical units that cannot compile natively unchanged
#   compat/            Watcom/DOS dialect and services (headers pre-included into historical units + their C side)
#   video/             native implementations of the original low-level drawing/timing contracts (ASM_SPEC.md)
#   platform/          SDL3 host: window, presentation, input, files, time
#   game/              port-owned glue: entry points, per-unit prototype headers, unit treatment (units.json)
#   tests/             native regression gates
project(rayman_port LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(RAY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/.. CACHE PATH "Repository root (src/, config/, docs/); override when building a copy of port/")
set(RAY_SRC ${RAY_ROOT}/src)
set(RAY_PORT ${CMAKE_CURRENT_SOURCE_DIR})
set(RAY_DATA "" CACHE PATH "Directory with the player's original Rayman data (RAYMAN.EXE's directory)")
set(RAY_CD_TEST_IMAGE "${RAY_ROOT}/assets/DOS_CD/RAYMAN.cue" CACHE FILEPATH "The player's Rayman CD image (CUE/BIN) used by the CD and end-to-end gates")

# --- Historical units -------------------------------------------------------------------------------------
# Every 'game' profile unit of config/units.json; derivatives replace their src file (derive/provenance.json).
file(READ ${RAY_ROOT}/config/units.json _units_json)
file(READ ${RAY_PORT}/game/units.json _port_units_json)
file(READ ${RAY_PORT}/derive/provenance.json _prov_json)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${RAY_ROOT}/config/units.json ${RAY_PORT}/game/units.json ${RAY_PORT}/derive/provenance.json)

string(JSON _prov_units GET "${_prov_json}" units)
string(JSON _nderived LENGTH "${_prov_units}")
set(RAY_DERIVED_DIR ${CMAKE_CURRENT_BINARY_DIR}/derived)
set(RAY_DERIVED_NAMES "")
set(RAY_DERIVED_OUTPUTS "")
set(RAY_DERIVED_DEPENDS "")
set(RAY_DERIVED_WITH_DIRS "")
if(_nderived GREATER 0)
    math(EXPR _last "${_nderived} - 1")
    foreach(i RANGE ${_last})
        string(JSON _name MEMBER "${_prov_units}" ${i})
        string(JSON _patch GET "${_prov_units}" ${_name} patch)
        list(APPEND RAY_DERIVED_NAMES ${_name})
        list(APPEND RAY_DERIVED_OUTPUTS ${RAY_DERIVED_DIR}/${_name})
        list(APPEND RAY_DERIVED_DEPENDS ${RAY_SRC}/${_name} ${RAY_PORT}/derive/${_patch})
        string(JSON _withdir ERROR_VARIABLE _nowithdir GET "${_prov_units}" ${_name} with_directory)
        if(NOT _nowithdir AND _withdir)
            get_filename_component(_d ${_name} DIRECTORY)
            list(APPEND RAY_DERIVED_WITH_DIRS ${_d})
            file(GLOB _dirfiles ${RAY_SRC}/${_d}/*)
            foreach(_f IN LISTS _dirfiles)
                file(RELATIVE_PATH _r ${RAY_SRC} ${_f})
                if(NOT _r IN_LIST RAY_DERIVED_NAMES)
                    list(APPEND RAY_DERIVED_OUTPUTS ${RAY_DERIVED_DIR}/${_r})
                    list(APPEND RAY_DERIVED_DEPENDS ${_f})
                endif()
            endforeach()
        endif()
    endforeach()
    add_custom_command(
        OUTPUT ${RAY_DERIVED_OUTPUTS}
        COMMAND ${CMAKE_COMMAND} -E env RAY_ROOT=${RAY_ROOT}
                ${Python3_EXECUTABLE} ${RAY_PORT}/tools/derive.py apply --out ${RAY_DERIVED_DIR}
        DEPENDS ${RAY_DERIVED_DEPENDS} ${RAY_PORT}/tools/derive.py ${RAY_PORT}/derive/provenance.json
        COMMENT "Applying port derivatives to the current historical sources")
endif()

string(JSON _units GET "${_units_json}" units)
string(JSON _nunits LENGTH "${_units}")
math(EXPR _last "${_nunits} - 1")
set(RAY_HIST_SOURCES "")
foreach(i RANGE ${_last})
    string(JSON _key MEMBER "${_units}" ${i})
    string(JSON _profile GET "${_units}" ${_key} profile)
    if(NOT _profile STREQUAL "game")
        continue()
    endif()
    string(JSON _source GET "${_units}" ${_key} source)
    string(JSON _replaced ERROR_VARIABLE _notreplaced GET "${_port_units_json}" replaced_units ${_source})
    if(NOT _notreplaced)
        continue()
    endif()
    get_filename_component(_stem ${_source} NAME_WE)
    string(REGEX REPLACE "^src/" "" _rel ${_source})
    get_filename_component(_reldir ${_rel} DIRECTORY)
    if(_rel IN_LIST RAY_DERIVED_NAMES OR (_reldir AND _reldir IN_LIST RAY_DERIVED_WITH_DIRS))
        set(_path ${RAY_DERIVED_DIR}/${_rel})
    else()
        set(_path ${RAY_ROOT}/${_source})
    endif()
    list(APPEND RAY_HIST_SOURCES ${_path})
    set(_opts "")
    if(EXISTS ${RAY_PORT}/game/protos/${_stem}.h)
        list(APPEND _opts "-include${RAY_PORT}/game/protos/${_stem}.h")
    endif()
    string(JSON _defs ERROR_VARIABLE _nodefs GET "${_port_units_json}" unit_defines ${_source})
    if(NOT _nodefs)
        string(JSON _ndefs LENGTH "${_defs}")
        math(EXPR _dl "${_ndefs} - 1")
        foreach(j RANGE ${_dl})
            string(JSON _d GET "${_defs}" ${j})
            set_property(SOURCE ${_path} APPEND PROPERTY COMPILE_DEFINITIONS ${_d})
        endforeach()
    endif()
    if(_opts)
        set_property(SOURCE ${_path} APPEND PROPERTY COMPILE_OPTIONS ${_opts})
    endif()
endforeach()

# Watcom 10.6 language semantics the historical code relies on (port/docs/ARCHITECTURE.md):
#   unsigned plain char, wrapping signed arithmetic, no type-based alias assumptions,
#   tentative definitions shared across units, SysV bit-field layout on every host.
set(RAY_HIST_FLAGS -funsigned-char -fwrapv -fno-strict-aliasing -fcommon)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # Incompatible pointer assignments are reviewed sites (port/docs/abi_sites.json), not build errors.
    list(APPEND RAY_HIST_FLAGS -Wno-unknown-pragmas -Werror=implicit-function-declaration
         -Werror=int-conversion -Werror=return-type -Wno-error=incompatible-pointer-types)
    if(MINGW)
        list(APPEND RAY_HIST_FLAGS -mno-ms-bitfields)
    endif()
endif()

# External data assets (PalPlasma, Def_Bande): generated from the player's own RAYMAN.EXE at build time, never
# tracked (user decision 2026-10-22, docs/LINK.md).
set(RAY_EXE "${RAY_DATA}/RAYMAN.EXE" CACHE FILEPATH "The player's GOG v1.21 RAYMAN.EXE")
if(NOT EXISTS "${RAY_EXE}")
    message(FATAL_ERROR "RAY_DATA must name the original game directory (RAYMAN.EXE not found at ${RAY_EXE})")
endif()
set(RAY_ASSETS_C ${CMAKE_CURRENT_BINARY_DIR}/assets/assets.c)
add_custom_command(
    OUTPUT ${RAY_ASSETS_C}
    COMMAND ${CMAKE_COMMAND} -E env RAY_ROOT=${RAY_ROOT} PYTHONDONTWRITEBYTECODE=1
            ${Python3_EXECUTABLE} ${RAY_PORT}/tools/gen_assets.py ${RAY_EXE} ${RAY_ASSETS_C}
    DEPENDS ${RAY_EXE} ${RAY_PORT}/tools/gen_assets.py
    COMMENT "Extracting external data assets from RAYMAN.EXE")

# game/port_main.c mirrors PcMain and is compiled exactly like the historical units it calls.
add_library(rayman_hist STATIC ${RAY_HIST_SOURCES} ${RAY_PORT}/game/port_main.c ${RAY_PORT}/game/dos_records.c
    ${RAY_ASSETS_C})
target_include_directories(rayman_hist PRIVATE
    ${RAY_PORT}/compat/include ${RAY_SRC}/include ${RAY_SRC} ${RAY_PORT} ${RAY_ROOT}/docs/semantics/generated)
target_compile_options(rayman_hist PRIVATE ${RAY_HIST_FLAGS}
    "SHELL:-include ${RAY_PORT}/compat/include/port_watcom.h")
target_compile_definitions(rayman_hist PRIVATE RAYMAN_PORT=1)

# --- Port-owned code ---------------------------------------------------------------------------------------
add_library(rayman_native STATIC
    compat/src/dos_clib.c
    compat/src/dos_services.c
    compat/src/gameport.c
    compat/src/hmi.c
    compat/src/inline_helpers.c
    compat/src/pc_hardware.c
    compat/src/unimplemented.c
    video/vga.c
    video/lowlevel_normal.c
    video/lowlevel_adapters.c
    platform/host.c
    platform/files.c
    platform/clock.c
    platform/movie_trace.c
    platform/cdrom.c
    platform/cd_audio.c
    platform/audio.c)
target_include_directories(rayman_native PUBLIC ${RAY_PORT} ${RAY_SRC}/include)
target_include_directories(rayman_native PRIVATE ${RAY_SRC}/include)
target_compile_options(rayman_native PRIVATE -funsigned-char -fwrapv -fno-strict-aliasing)
if(MINGW)
    target_compile_options(rayman_native PRIVATE -mno-ms-bitfields)
endif()
target_link_libraries(rayman_native PUBLIC SDL3::SDL3)

add_executable(rayman game/entry.c)
target_link_libraries(rayman PRIVATE rayman_native rayman_hist rayman_native m)
if(WIN32)
    add_custom_command(TARGET rayman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:rayman>)
endif()

# --- Gates ---------------------------------------------------------------------------------------------------
enable_testing()
# Source connection to the historical reconstruction (no build needed): derivatives still apply to the reviewed
# bases, the PcMain mirror is current, the per-unit prototypes are current, every 64-bit-sensitive site has a
# reviewed verdict.
add_test(NAME port_derivatives COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tools/derive.py check)
add_test(NAME port_mirror COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tools/mirror.py check)
add_test(NAME port_prototypes COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tools/gen_protos.py --check)
add_test(NAME port_abi_audit COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tools/abi_audit.py)
set_tests_properties(port_derivatives port_mirror PROPERTIES ENVIRONMENT "RAY_ROOT=${RAY_ROOT}")
set_tests_properties(port_prototypes port_abi_audit PROPERTIES ENVIRONMENT "CC=${CMAKE_C_COMPILER};RAY_ROOT=${RAY_ROOT}")

# Native unit tests of the compat layer and record decoding.
add_executable(port_unit_tests tests/unit_tests.c)
target_link_libraries(port_unit_tests PRIVATE rayman_native rayman_hist rayman_native m)
target_include_directories(port_unit_tests PRIVATE ${RAY_SRC}/include ${RAY_PORT})
add_test(NAME port_unit_tests COMMAND port_unit_tests)

# Screen-effect differential proof (x86_64 and i686 derivatives vs unmodified units at DOS pointer width).
include(tests/effect_diff.cmake)

# End to end: the six original recorded demos (worlds 1-5) through the whole game, deterministic, against the reviewed golden summary
# shared by all architectures (tests/golden).
# The game's own RAYMAN.CFG survives a restart (two sessions, scripted menu walk).
add_test(NAME port_config_roundtrip COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tests/config_roundtrip.py
    $<TARGET_FILE:rayman> ${RAY_DATA} ${RAY_CD_TEST_IMAGE})

foreach(_demo 0 1 2 3 4 5)
    add_test(NAME port_e2e_demo${_demo} COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tests/e2e_demo.py
        $<TARGET_FILE:rayman> ${RAY_DATA} ${_demo} ${RAY_CD_TEST_IMAGE})
    set_tests_properties(port_e2e_demo${_demo} PROPERTIES TIMEOUT 1800)
endforeach()

add_executable(port_cd_tests tests/cd_tests.c)
target_include_directories(port_cd_tests PRIVATE ${RAY_SRC}/include ${RAY_SRC} ${RAY_PORT})
target_link_libraries(port_cd_tests PRIVATE rayman_native rayman_hist rayman_native m)
add_test(NAME port_cd_tests COMMAND port_cd_tests ${RAY_CD_TEST_IMAGE})

add_executable(port_cd_audio_tests tests/cd_audio_tests.c)
target_include_directories(port_cd_audio_tests PRIVATE ${RAY_PORT})
target_link_libraries(port_cd_audio_tests PRIVATE rayman_native rayman_hist rayman_native m)
add_test(NAME port_cd_audio_tests COMMAND port_cd_audio_tests ${RAY_CD_TEST_IMAGE})
set_tests_properties(port_cd_audio_tests PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy")
add_test(NAME port_cd_missing_disc COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tests/cd_missing_disc.py
    $<TARGET_FILE:rayman> ${RAY_DATA} ${CMAKE_CURRENT_BINARY_DIR}/cd-no-image-save)

set(HMI_REFERENCE ${CMAKE_CURRENT_BINARY_DIR}/hmi_reference.c)
add_custom_command(OUTPUT ${HMI_REFERENCE}
    COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tools/gen_hmi_reference.py ${RAY_ROOT} ${HMI_REFERENCE}
    DEPENDS ${RAY_PORT}/tools/gen_hmi_reference.py
        ${RAY_SRC}/hmi/hmi_local.h ${RAY_SRC}/hmi/hmi_samples.c
        ${RAY_SRC}/hmi/hmi_status.c ${RAY_SRC}/hmi/hmi_active.c ${RAY_SRC}/hmi/hmi_voice_params.c)
add_executable(port_hmi_tests tests/hmi_tests.c ${HMI_REFERENCE})
target_include_directories(port_hmi_tests PRIVATE ${RAY_SRC}/hmi)
target_link_libraries(port_hmi_tests PRIVATE rayman_native rayman_hist rayman_native m)
add_test(NAME port_hmi_tests COMMAND port_hmi_tests ${RAY_DATA} ${CMAKE_CURRENT_BINARY_DIR}/hmi-test-save)

# Original FLC decoder, private native layout and independent scalar chunk fixtures/corpus.
set(RAY_MOVIE_TEST_DATA "${RAY_ROOT}/assets/DOS" CACHE PATH "Player's read-only INTRO.DAT/CONCLU.DAT test inputs")
add_executable(port_movie_tests tests/movie_tests.c)
add_dependencies(port_movie_tests rayman_hist)
target_compile_options(port_movie_tests PRIVATE ${RAY_HIST_FLAGS}
    "SHELL:-include ${RAY_PORT}/compat/include/port_watcom.h")
target_compile_definitions(port_movie_tests PRIVATE PORT_MOVIE_SOURCE="${RAY_DERIVED_DIR}/movie.c")
target_include_directories(port_movie_tests PRIVATE ${RAY_PORT}/compat/include ${RAY_SRC}/include ${RAY_SRC} ${RAY_PORT})
target_link_libraries(port_movie_tests PRIVATE rayman_native rayman_hist rayman_native m)
add_test(NAME port_movie_tests COMMAND port_movie_tests ${RAY_MOVIE_TEST_DATA} ${CMAKE_CURRENT_BINARY_DIR}/movie-test-save ${RAY_CD_TEST_IMAGE})

add_test(NAME port_movie_e2e COMMAND ${Python3_EXECUTABLE} ${RAY_PORT}/tests/e2e_movie.py
    $<TARGET_FILE:rayman> ${RAY_DATA} ${RAY_MOVIE_TEST_DATA} ${RAY_CD_TEST_IMAGE})
set_tests_properties(port_movie_e2e PROPERTIES TIMEOUT 600)
