#include "settings.h"
#include "world/world.h"
#include "map/area.h"
#include "ecs/entity.h"
#include <vector>

static void init_fov(fov_settings_type* fov_settings);
static bool is_opaque(void* area, void* world, int x, int y, int z, int wx, int wy);
static void set_visible(void* area, int x, int y, int z, int dx, int dy, void* src);

Settings::Settings() {
  fov_settings = new fov_settings_type;
  init_fov(fov_settings);
}

Settings::~Settings() {
  fov_settings_free(fov_settings);
  delete fov_settings;
}

static void init_fov(fov_settings_type* fov_settings) {
  fov_settings_init(fov_settings);
  fov_settings_set_shape(fov_settings, FOV_SHAPE_CIRCLE);
  fov_settings_set_corner_peek(fov_settings, FOV_CORNER_PEEK);
  fov_settings_set_opaque_apply(fov_settings, FOV_OPAQUE_APPLY);
  fov_settings_set_opacity_test_function(fov_settings, is_opaque);
  fov_settings_set_apply_lighting_function(fov_settings, set_visible);
}

static bool is_opaque(void* area, void* world, int x, int y, int z, int wx, int wy) {
  Area* area_ptr = static_cast <Area*>(area);
  World* world_ptr = static_cast <World*>(world);
  if (area_ptr->PointWithinBounds(x, y))
    if (area_ptr->GetBlock(x, y, z)->opaque)
      return true;
    else if (area_ptr->GetEntity(x, y, z) != -1)
      if (!world_ptr->entities[area_ptr->GetEntity(x, y, z)].HasComponent(EC_NOTSOLID_ID))
        return true;
  return false;
}

static void set_visible(void* area, int x, int y, int z, int dx, int dy, void* src) {
  Area* area_ptr = static_cast<Area*>(area);
  std::vector<Position>* visiblepoints = static_cast<std::vector<Position>*>(src);
  if (!area_ptr->PointWithinBounds(x, y))
    return;
  visiblepoints->push_back({ static_cast<uint16_t>(x), static_cast<uint16_t>(y), static_cast<uint16_t>(z) });
}