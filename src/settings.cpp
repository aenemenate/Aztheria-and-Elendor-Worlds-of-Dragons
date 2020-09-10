
#include "settings.h"
#include "map.h"
#include "entity.h"
#include <vector>

static void init_fov(fov_settings_type* fov_settings);
static bool is_opaque(void* map, int x, int y);
static void set_visible(void* map, int x, int y, int dx, int dy, void* src);

Settings::Settings() {
  fov_settings = new fov_settings_type;
  init_fov(fov_settings);
}

Settings::~Settings() {
  fov_settings_free(fov_settings);
}

static void init_fov(fov_settings_type* fov_settings) {
  fov_settings_init(fov_settings);
  fov_settings_set_shape(fov_settings, FOV_SHAPE_CIRCLE);
  fov_settings_set_corner_peek(fov_settings, FOV_CORNER_PEEK);
  fov_settings_set_opaque_apply(fov_settings, FOV_OPAQUE_APPLY);
  fov_settings_set_opacity_test_function(fov_settings, is_opaque);
  fov_settings_set_apply_lighting_function(fov_settings, set_visible);
}

static bool is_opaque(void* map, int x, int y)
{
  Map* map_ptr = static_cast <Map*>(map);
  if (map_ptr->PointWithinBounds(x, y))
    return map_ptr->GetTile(x, y)->opaque;
  else
    return false;
}

static void set_visible(void* map, int x, int y, int dx, int dy, void* src)
{
  Map* map_ptr = static_cast<Map*>(map);
  std::vector<Position>* visiblepoints = static_cast<std::vector<Position>*>(src);
  if (!map_ptr->PointWithinBounds(x, y))
    return;
  visiblepoints->push_back({ static_cast<uint16_t>(x), static_cast<uint16_t>(y) });
}