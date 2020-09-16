#pragma once

#include "fov.h"

class Settings {
public:
  Settings();
  ~Settings();

  /*void LoadSettings(std::string filename);
  void SaveSettings(std::string filename);*/

  fov_settings_type* get_fov_settings() { return fov_settings; }
private:
  fov_settings_type* fov_settings;
};
