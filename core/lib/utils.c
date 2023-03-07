#include "./utils.h"

#include <math.h>

void liblora_toa_stats_update(liblora_toa_stats_t *stats)
{
    // the low data rate optimize is automatically 
    // set when the symbols len exceeds 16ms
    bool low_data_rate_optimize;

    stats->symbols_rate = stats->bandwidth / (1 << stats->spreading_factor);
    stats->symbols_len = 1 / stats->symbols_rate;
    low_data_rate_optimize = stats->symbols_len > 16;

    double _up_frac = 4 / (4 + stats->coding_rate);
    double _down_frac = (1 << stats->spreading_factor) / stats->bandwidth;
    stats->bitrate = 100 * stats->spreading_factor * ( _up_frac / _down_frac );

    stats->preamble_symbols = stats->preamble_len + 4.25;
    stats->preamble_duration = stats->symbols_len * stats->preamble_symbols;

    double _num = 8*stats->payload_bytes - 4*stats->spreading_factor + 28 + 16*stats->crc_on - 20*stats->implicit_header;
    double _den = 4*(stats->spreading_factor - 2*low_data_rate_optimize);
    stats->payload_symbols = 8 + fmax(ceil((_num / _den) * (stats->coding_rate + 4)), 0);
    stats->payload_duration = stats->symbols_len * stats->payload_symbols;

    stats->packet_duration = stats->preamble_duration + stats->payload_duration;
}
