#include "game/particle_manager.h"
#include "core/global.h"
#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

Particle* particle_create(Vector2D pos, Vector2D scale, Vector2D vel, SDL_Texture* texture) {
  Particle* particle = malloc(sizeof(Particle));
  if (!particle) return NULL;

  particle->base_pos = pos;
  particle->pos = particle->base_pos;
  particle->base_vel = vel;
  particle->vel = particle->base_vel;
  particle->base_scale = scale;
  particle->scale = particle->base_scale;
  particle->base_lifeTime = random_value(2, 3);
  particle->lifeTime = particle->base_lifeTime;
  particle->texture = texture;

  return particle;
}

void particle_destroy(Particle* particle) {
  if (!particle) return;
  if (particle->texture) SDL_DestroyTexture(particle->texture);
  free(particle);
}

void particle_update(Particle* particle) {
  particle->vel = particle->base_vel;
  particle->pos.x += particle->vel.x;
  particle->pos.y += particle->vel.y;
  particle->lifeTime -= 0.2;
  particle->scale.x -= 0.2 * particle->lifeTime;
  particle->scale.y -= 0.2 * particle->lifeTime;
  if (particle->scale.x <= 0 && particle->scale.y <= 0) particle->scale = particle->base_scale;
}

void particle_render(Particle* particle) {
  SDL_Rect rect = {
    particle->pos.x * global.camera->zoom - global.camera->pos.x,
    particle->pos.y * global.camera->zoom - global.camera->pos.y,
    particle->scale.x * global.camera->zoom,
    particle->scale.y * global.camera->zoom
  };

  if (!particle->texture) {
    SDL_RenderFillRect(global.renderer, &rect);
  } else {
    SDL_RenderCopy(global.renderer, particle->texture, NULL, &rect);
  }
}

ParticleSystem* particle_system_create(Vector2D pos, size_t initial_capacity) {
  ParticleManager* manager = global.particle_manager;

  // Add extra capacity to the manager on overflow
  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->systems = realloc(manager->systems, manager->capacity * sizeof(ParticleSystem*));
    if (!manager->systems) return NULL;
  }

  ParticleSystem* system = malloc(sizeof(ParticleSystem));
  if (!system) return NULL;

  system->particles = malloc(initial_capacity * sizeof(Particle*));
  system->pos = pos;
  system->count = 0;
  system->capacity = initial_capacity;
  manager->systems[manager->count++] = system;

  return system;
}

void particle_system_destroy(ParticleSystem* system) {
  if (!system) return;
  for (size_t i = 0; i < system->count; ++i) {
    particle_destroy(system->particles[i]);
  }
  free(system->particles);
  free(system);
}

void particle_system_render(ParticleSystem* system) {
  for (size_t i = 0; i < system->count; ++i) {
    particle_render(system->particles[i]);
  }
}

void particle_system_update(ParticleSystem* system) {
  for (size_t i = 0; i < system->count; ++i) {
    particle_update(system->particles[i]);
    if (system->particles[i]->lifeTime <= 0) particle_system_remove_particle(system, i);  
  }
}

void particle_system_add_particle(ParticleSystem* system, Vector2D scale, Vector2D vel, SDL_Texture* texture) {
  if (system->count >= system->capacity) return;
  system->particles[system->count++] = particle_create(system->pos, scale, vel, texture);
}

void particle_system_remove_particle(ParticleSystem* system, int id) {
  if (!system || id >= system->count) return;
  particle_destroy(system->particles[id]);
  for (size_t i = id; i < system->count - 1; ++i) {
    system->particles[i] = system->particles[i + 1];
  }
  system->count--;
  system->particles = realloc(system->particles, sizeof(Particle*) * system->capacity);
}

void particle_system_modify_vel(ParticleSystem* system, Vector2D value) {
  for (size_t i = 0; i < system->count; ++i) {
    system->particles[i]->base_vel = value;
  }
}

ParticleManager* particle_manager_create(size_t initial_capacity) {
  ParticleManager* system = malloc(sizeof(ParticleManager));
  if (!system) return NULL;

  system->systems = malloc(initial_capacity * sizeof(ParticleSystem*));
  system->count = 0;
  system->capacity = initial_capacity;

  return system;
}

void particle_manager_destroy() {
  if (!global.particle_manager) return;
  for (size_t i = 0; i < global.particle_manager->count; ++i) {
    particle_system_destroy(global.particle_manager->systems[i]);
  }
  free(global.particle_manager->systems);
}

void particle_manager_render() {
  for (size_t i = 0; i < global.particle_manager->count; ++i) {
    particle_system_render(global.particle_manager->systems[i]);
  }
}

void particle_manager_update() {
  for (size_t i = 0; i < global.particle_manager->count; ++i) {
    particle_system_update(global.particle_manager->systems[i]);
  }
}
