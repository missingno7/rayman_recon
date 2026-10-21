#ifndef SFIN_MENU_KEYS_LOCAL_H
#define SFIN_MENU_KEYS_LOCAL_H
/* RAYVERSE names and initializers, checked against the DOS pointer tables
   at 2:6888 and 2:6a88. Mutable pointers: set_special_key_descriptions updates them. */
const char *key_descriptions_azerty[128] = {
    0, 0, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 0, 0, 0,
    0, "a", "z", "e", "r", "t", "y", "u", "i", "o", "p", 0, 0, 0,
    0, "q", "s", "d", "f", "g", "h", "j", "k", "l", "m", 0,
    0, 0, 0, "w", "x", "c", "v", "b", "n", "?", ".", ":", 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "5"
};
const char *key_descriptions_qwerty[128] = {
    0, 0, "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 0, 0, 0,
    0, "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", 0, 0, 0,
    0, "a", "s", "d", "f", "g", "h", "j", "k", "l", ":", 0,
    0, 0, 0, "z", "x", "c", "v", "b", "n", "m", 0, ".", "?", 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "5"
};
#endif
