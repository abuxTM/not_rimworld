# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.29
cmake_policy(SET CMP0009 NEW)

# SOURCES at CMakeLists.txt:29 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/abux/Projects/c/rimworld/src/*.c")
set(OLD_GLOB
  "/home/abux/Projects/c/rimworld/src/core/init.c"
  "/home/abux/Projects/c/rimworld/src/core/renderer.c"
  "/home/abux/Projects/c/rimworld/src/core/window.c"
  "/home/abux/Projects/c/rimworld/src/game/camera.c"
  "/home/abux/Projects/c/rimworld/src/game/game.c"
  "/home/abux/Projects/c/rimworld/src/game/item_manager/inventory.c"
  "/home/abux/Projects/c/rimworld/src/game/item_manager/item.c"
  "/home/abux/Projects/c/rimworld/src/game/mouse.c"
  "/home/abux/Projects/c/rimworld/src/game/particle_manager.c"
  "/home/abux/Projects/c/rimworld/src/game/pawn_manager.c"
  "/home/abux/Projects/c/rimworld/src/game/texture_manager.c"
  "/home/abux/Projects/c/rimworld/src/game/ui.c"
  "/home/abux/Projects/c/rimworld/src/game/world.c"
  "/home/abux/Projects/c/rimworld/src/utils/remath.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/abux/Projects/c/rimworld/build/CMakeFiles/cmake.verify_globs")
endif()
