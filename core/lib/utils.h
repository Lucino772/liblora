#ifndef _LIBLORA_CORE_LIB_UTILS_H_
#define _LIBLORA_CORE_LIB_UTILS_H_

#include <stdint.h>
#include <stdbool.h>


#define LIBLORA_UTILS_CR_4_5 1
#define LIBLORA_UTILS_CR_4_6 2
#define LIBLORA_UTILS_CR_4_7 3
#define LIBLORA_UTILS_CR_4_8 4

#define LIBLORA_UTILS_TOA_STATS_INIT(payload_bytes)  { 125, 7, LIBLORA_UTILS_CR_4_8, 8, payload_bytes, true, false };

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct {
        uint16_t bandwidth;
        uint8_t spreading_factor;
        uint8_t coding_rate;
        uint8_t preamble_len;
        uint8_t payload_bytes;
        bool crc_on;
        bool implicit_header;

        // The following value will be set when calling the function
        // liblora_toa_stats_update. Those value will be calculated
        // based on the other values above.
        double bitrate;
        double duty_cycle;
        double symbols_rate;
        double symbols_len;
        double preamble_symbols;
        double preamble_duration;
        double payload_symbols;
        double payload_duration;
        double packet_duration;
    } liblora_toa_stats_t;

    void liblora_toa_stats_update(liblora_toa_stats_t *stats)

#ifdef __cplusplus
}
#endif

#endif