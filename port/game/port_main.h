/*
 * Entry points into the historical game.
 *
 *  port_run_dos_main  the original program: main() -> PcMain() of src/pcmain.c, unchanged.
 *  port_run_demo      TEST ENTRY: PcMain's initialisation, then one of the six original recorded demos
 *                     (DemoRecord, played by the game's own demo code), then return.
 *  port_run_menus     TEST ENTRY: PcMain's initialisation and loop with its original menus (new game, world
 *                     map, levels), interactively.
 *
 * The test entries follow PcMain line by line (src/pcmain.c 1009-1144); every step they replace or skip is
 * listed in port_main.c and port/docs/DIVERGENCES.md, and tests/test_port_mirror.py fails when PcMain changes.
 */
#ifndef PORT_GAME_PORT_MAIN_H
#define PORT_GAME_PORT_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

int port_run_dos_main(int argc, char **argv);
int port_run_demo(int demo);
int port_run_menus(void);

#ifdef __cplusplus
}
#endif

#endif
