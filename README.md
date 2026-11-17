# Tamagotchi STM32

An embedded-friendly Tamagotchi core with indoor positioning from Bluetooth
angle-of-arrival (AOA) and received signal strength (RSSI) measurements.

The code in `include/` and `src/` is deliberately independent of STM32 HAL so
it can be unit-tested on a host and then called from an STM32 application:

1. Read RSSI and AOA from the radio/antenna array in the HAL callback.
2. Fill `tmg_position_sample_t` values for the configured anchors.
3. Call `tmg_position_estimate()` and store the result in the pet with
   `tmg_pet_set_position()`.
4. Call `tmg_pet_tick()` from the periodic STM32 timer and render the pet
   state on the attached display.

RSSI is converted to a distance using the log-distance path-loss model. Each
anchor's distance and AOA form a ray estimate; valid estimates are combined
with quality weights, making the result useful when one radio measurement is
temporarily unavailable.

## Host tests

The focused tests require only a C compiler and can be run with:

```sh
make test
```