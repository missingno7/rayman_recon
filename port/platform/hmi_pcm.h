/* PCM and serialized-word conversion at the DOS audio boundary. */
#ifndef PORT_HMI_PCM_H
#define PORT_HMI_PCM_H
#include <stdint.h>
static inline uint32_t port_hmi_le32(const uint8_t *p)
{ return (uint32_t)p[0] | (uint32_t)p[1]<<8 | (uint32_t)p[2]<<16 | (uint32_t)p[3]<<24; }
static inline int16_t port_hmi_pcm(const uint8_t *p, int speech)
{
    if (speech) {
        unsigned v = (unsigned)p[0] | (unsigned)p[1]<<8;
        return (int16_t)(v < 32768 ? (int)v : (int)v - 65536);
    }
    return (int16_t)(((int)*p - 128) * 256);
}
#endif
