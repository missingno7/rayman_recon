/* GOG v1.21 _DATA 2:062d8. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"

extern char dt_str_write_protect_violation[];
extern char dt_str_unknown_unit[];
extern char dt_str_drive_not_ready[];
extern char dt_str_unknown_command[];
extern char dt_str_crc_error[];
extern char dt_str_bad_drive_request_structure_length[];
extern char dt_str_seek_error[];
extern char dt_str_unknown_media[];
extern char dt_str_sector_not_found[];
extern char dt_str_printer_out_of_paper[];
extern char dt_str_write_fault[];
extern char dt_str_read_fault[];
extern char dt_str_general_failure[];
extern char dt_str_reserved[];
extern char dt_str_invalid_disk_change[];

char *dos_error_messages[16] = {
    dt_str_write_protect_violation,
    dt_str_unknown_unit,
    dt_str_drive_not_ready,
    dt_str_unknown_command,
    dt_str_crc_error,
    dt_str_bad_drive_request_structure_length,
    dt_str_seek_error,
    dt_str_unknown_media,
    dt_str_sector_not_found,
    dt_str_printer_out_of_paper,
    dt_str_write_fault,
    dt_str_read_fault,
    dt_str_general_failure,
    dt_str_reserved,
    dt_str_reserved,
    dt_str_invalid_disk_change,
};
