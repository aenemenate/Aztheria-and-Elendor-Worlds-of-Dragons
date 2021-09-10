#pragma once

#include <map>
#include <string>

enum Material {
  Wood = 0,
  Dirt, 
  Stone,
  Tin,
  Zinc,
  Copper,
  Bronze,
  Brass,
  Iron,
  Steel,
  Platinum,
  Glass,
  Leather,
  Cloth,
  Silk,
  Bone,
  Plant,
  Meat,
  Coal
};

// Measured in cubic feet
std::map<Material, int> GetDensities();

std::map<Material, int> GetImpactYields();

std::map<Material, int> GetImpactFractures();

std::map<Material, int> GetShearYields();

std::map<Material, int> GetShearFractures();

std::map<Material, std::string> GetMaterialNames();

std::map<Material, std::string> GetMaterialColors();