#pragma once

#include "fov.h"

class Settings {
  fov_settings_type* fov_settings;
public:
  Settings();
  ~Settings();

  /*void LoadSettings(std::string filename);
  void SaveSettings(std::string filename);*/

  fov_settings_type* get_fov_settings() { return fov_settings; }
};
