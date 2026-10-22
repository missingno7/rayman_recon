/* INVENTED names. DOS 1:79099..790ed; lookup is unchecked, as in the oracle.
 * 2:2eb30 is a near-pointer table, not a far-pointer table. */
#include "hmi_local.h"

char *_pSOSErrorStrings[20] = {
    "Error Code Does Not Indicate An Error",
    "Specified Driver Is Not Loaded",
    "Specified Pointer Is NULL",
    "Detection System Is Already Initialized",
    "File Open Failure",
    "Memory Allocation Failure",
    "Invalid Driver ID",
    "Driver Not Found",
    "Detection System Failed To Find Hardware",
    "Driver Already Loaded Using Specified Handle",
    "Invalid Handle",
    "No Handles Available",
    "Hardware Already Paused",
    "Hardware Not Paused",
    "Data Is Not Valid",
    "HMI*.386 File Open Failure",
    "Incorrect Port",
    "Incorrect IRQ",
    "Incorrect DMA",
    "Incorrect DMA/IRQ",
};
void sosDIGIErr_Start(void) {}
char *sosGetErrorString(unsigned error)
{
    return _pSOSErrorStrings[error];
}
void sosDIGIErr_End(void) {}
