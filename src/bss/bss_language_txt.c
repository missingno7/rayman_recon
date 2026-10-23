/* Language string table, GOG v1.21 2:32b48 (RAYVERSE name language_txt).
 * DOS LoadLanguageTxt 1:3800c stores one pointer per string; every RAY.LNG block initialises indices 0..236, and
 * ToDoAtExit 1:272df reads slot 231 (game_title is that interior element). 237 entries (948 bytes) is the PROVEN
 * minimum extent. The next referenced object lies at +0x410 in every build examined (December 1995 demos, v1.12,
 * v1.12 Preview, FR and US v1.21), so the original capacity is 237..260 entries; the 116 bytes 2:32ee4..32f58 stay an
 * unattributed hole (OPEN - docs/BLOCKERS.md B9). Do not widen this declaration without independent evidence.
 */
char *language_txt[237];
