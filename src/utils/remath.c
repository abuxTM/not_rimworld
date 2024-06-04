#include "utils/remath.h"
#include <stdint.h>
#include <stdlib.h>

float random_value(float min, float max) {
  return min + ((float)rand() / RAND_MAX) * (max - min);
}

float value_noise(Vector2D pos, Vector2D scale, int seed) {
    uint32_t n = (uint32_t)(pos.x * scale.x + pos.y * scale.y * 57) + (uint32_t)seed * 131;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731U + 789221U) + 1376312589U) & 0x7fffffffU) / 1073741824.0f);
}

