#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "utils/remath.h"
#include <SDL2/SDL_render.h>

typedef struct {
  Vector2D base_pos, pos;
  Vector2D base_scale, scale;
  Vector2D base_vel, vel;
  float base_lifeTime, lifeTime;
  SDL_Texture* texture;
} Particle;

typedef struct {
  Vector2D pos, scale;
  Particle** particles;
  size_t count;
  size_t capacity;
} ParticleSystem;

typedef struct {
  ParticleSystem** systems;
  size_t count;
  size_t capacity;
} ParticleManager;

Particle* particle_create(Vector2D pos, Vector2D scale, Vector2D vel, SDL_Texture* texture);
void particle_destroy(Particle* particle);
void particle_update(Particle* particle);
void particle_render(Particle* particle);

ParticleSystem* particle_system_create(Vector2D pos, size_t initial_capacity);
void particle_system_destroy(ParticleSystem* system);
void particle_system_update(ParticleSystem* system);
void particle_system_render(ParticleSystem* system);
void particle_system_add_particle(ParticleSystem* system, Vector2D scale, Vector2D vel, SDL_Texture* texture);
void particle_system_remove_particle(ParticleSystem* system, int id);
void particle_system_modify_vel(ParticleSystem* system, Vector2D value);

ParticleManager* particle_manager_create(size_t initial_capacity);
void particle_manager_destroy();
void particle_manager_update();
void particle_manager_render();

#endif
