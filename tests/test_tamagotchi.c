#include "tamagotchi.h"

#include <assert.h>
#include <math.h>

static void test_pet_lifecycle(void)
{
    tmg_pet_t pet;
    tmg_pet_init(&pet);
    assert(pet.hunger == 0 && pet.happiness == 100);
    for (unsigned i = 0; i < 81; ++i) {
        tmg_pet_tick(&pet);
    }
    assert(pet.age_ticks == 81 && pet.hunger == 81 && pet.happiness == 99);
    tmg_pet_feed(&pet);
    assert(pet.hunger == 56);
    tmg_pet_play(&pet);
    assert(pet.happiness == 100 && pet.hunger == 61);
}

static void test_aoa_rssi_position(void)
{
    const tmg_position_sample_t sample = {
        {2.0f, 3.0f}, -50.0f, 0.0f, true, true
    };
    tmg_position_t position = tmg_position_estimate(&sample, 1, -40.0f, 2.0f);
    assert(position.valid);
    assert(fabsf(position.position.x - 5.1623f) < 0.01f);
    assert(fabsf(position.position.y - 3.0f) < 0.01f);
}

static void test_invalid_measurements(void)
{
    tmg_position_sample_t sample = {{0.0f, 0.0f}, -50.0f, 0.0f, false, true};
    tmg_position_t position = tmg_position_estimate(&sample, 1, -40.0f, 2.0f);
    assert(!position.valid);
}

int main(void)
{
    test_pet_lifecycle();
    test_aoa_rssi_position();
    test_invalid_measurements();
    return 0;
}
