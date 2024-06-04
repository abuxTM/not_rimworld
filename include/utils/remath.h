#ifndef REMATH_H
#define REMATH_H

typedef struct {
  float x, y;
} Vector2D;

typedef struct {
  int x, y;
} VectorI2D;

float random_value(float min, float max);
float value_noise(Vector2D pos, Vector2D scale, int seed);

#endif
