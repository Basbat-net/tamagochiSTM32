#ifndef TAMAGOTCHI_H
#define TAMAGOTCHI_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x;
    float y;
} tmg_point_t;

typedef struct {
    tmg_point_t position;
    float confidence;
    bool valid;
} tmg_position_t;

typedef struct {
    tmg_point_t position;
    float rssi_dbm;
    float aoa_degrees;
    bool rssi_valid;
    bool aoa_valid;
} tmg_position_sample_t;

typedef struct {
    unsigned hunger;
    unsigned happiness;
    unsigned age_ticks;
    tmg_position_t position;
} tmg_pet_t;

void tmg_pet_init(tmg_pet_t *pet);
void tmg_pet_tick(tmg_pet_t *pet);
void tmg_pet_feed(tmg_pet_t *pet);
void tmg_pet_play(tmg_pet_t *pet);
void tmg_pet_set_position(tmg_pet_t *pet, tmg_position_t position);

/*
 * Estimate a position from anchor samples. RSSI uses tx_power_dbm at one
 * metre and path_loss_exponent. AOA is measured clockwise from +X, in degrees.
 */
tmg_position_t tmg_position_estimate(const tmg_position_sample_t *samples,
                                     size_t count,
                                     float tx_power_dbm,
                                     float path_loss_exponent);

#endif
