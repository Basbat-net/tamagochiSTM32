#include "tamagotchi.h"

#include <math.h>

static float clamp(float value, float low, float high)
{
    if (value < low) {
        return low;
    }
    return value > high ? high : value;
}

void tmg_pet_init(tmg_pet_t *pet)
{
    if (pet == NULL) {
        return;
    }
    pet->hunger = 0;
    pet->happiness = 100;
    pet->age_ticks = 0;
    pet->position = (tmg_position_t){{0.0f, 0.0f}, 0.0f, false};
}

void tmg_pet_tick(tmg_pet_t *pet)
{
    if (pet == NULL) {
        return;
    }
    pet->age_ticks++;
    pet->hunger = (unsigned)clamp((float)pet->hunger + 1.0f, 0.0f, 100.0f);
    if (pet->hunger > 80) {
        pet->happiness = (unsigned)clamp((float)pet->happiness - 1.0f, 0.0f, 100.0f);
    }
}

void tmg_pet_feed(tmg_pet_t *pet)
{
    if (pet != NULL) {
        pet->hunger = (unsigned)clamp((float)pet->hunger - 25.0f, 0.0f, 100.0f);
    }
}

void tmg_pet_play(tmg_pet_t *pet)
{
    if (pet != NULL) {
        pet->happiness = (unsigned)clamp((float)pet->happiness + 10.0f, 0.0f, 100.0f);
        pet->hunger = (unsigned)clamp((float)pet->hunger + 5.0f, 0.0f, 100.0f);
    }
}

void tmg_pet_set_position(tmg_pet_t *pet, tmg_position_t position)
{
    if (pet != NULL && position.valid) {
        pet->position = position;
    }
}

tmg_position_t tmg_position_estimate(const tmg_position_sample_t *samples,
                                     size_t count,
                                     float tx_power_dbm,
                                     float path_loss_exponent)
{
    tmg_position_t result = {{0.0f, 0.0f}, 0.0f, false};
    float weight_total = 0.0f;
    size_t i;

    if (samples == NULL || count == 0 || path_loss_exponent <= 0.0f) {
        return result;
    }

    for (i = 0; i < count; ++i) {
        const tmg_position_sample_t *sample = &samples[i];
        float distance;
        float angle;
        float weight;

        if (!sample->rssi_valid || !sample->aoa_valid ||
            !isfinite(sample->rssi_dbm) || !isfinite(sample->aoa_degrees)) {
            continue;
        }

        distance = powf(10.0f, (tx_power_dbm - sample->rssi_dbm) /
                                  (10.0f * path_loss_exponent));
        angle = sample->aoa_degrees * (float)M_PI / 180.0f;
        weight = 1.0f / (1.0f + distance);
        result.position.x += weight * (sample->position.x + distance * cosf(angle));
        result.position.y += weight * (sample->position.y + distance * sinf(angle));
        weight_total += weight;
    }

    if (weight_total > 0.0f) {
        result.position.x /= weight_total;
        result.position.y /= weight_total;
        result.confidence = clamp(weight_total / (float)count, 0.0f, 1.0f);
        result.valid = true;
    }
    return result;
}
