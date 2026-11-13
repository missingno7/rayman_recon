# 32-bit host loader for the Watcom BINNT stubs; needs ../../../win32_layer built (make there).
LAYER ?= $(HOME)/git/win32_layer
OUT   ?= ../../build/pe_run

$(OUT)/pe_run: pe_run.cpp $(LAYER)/build/libwin32_layer.a
	mkdir -p $(OUT)
	g++ -m32 -O1 -g -isystem $(LAYER)/include -include callconv.h -I$(LAYER)/src $< $(LAYER)/build/libwin32_layer.a -lSDL3 -lpthread -lm -o $@
