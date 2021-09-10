#include "materials.h"


// Measured in cubic feet
std::map<Material, int> densities = {
  {Wood, 0.026},
  {Dirt, 0.063}, 
  {Stone, 0.09},
  {Tin, 0.26},
  {Zinc, 0.258},
  {Copper, 0.323},
  {Bronze, 0.34},
  {Brass, 0.31},
  {Iron, 0.28},
  {Steel, 0.28},
  {Platinum, 0.77},
  {Glass, 0.086},
  {Leather, 0.031},
  {Cloth, 0.014},
  {Silk, 0.009},
  {Bone, 0.052},
  {Plant, 0.016},
  {Meat, 0.028},
  {Coal, 0.05}
};

std::map<Material, int> impactYields = {
  {Wood, 9},
  {Stone, 13},
  {Tin, 7},
  {Zinc, 6},
  {Copper, 20},
  {Bronze, 40},
  {Brass, 30},
  {Iron, 50},
  {Steel, 60},
  {Platinum, 80},
  {Glass, 1},
  {Leather, 2},
  {Cloth, 1},
  {Silk, 1},
  {Bone, 8},
  {Plant, 1},
  {Meat, 2},
  {Coal, 8}
};

std::map<Material, int> impactFractures = {
  {Wood, 200},
  {Stone, 200},
  {Tin, 200},
  {Zinc, 200},
  {Copper, 250},
  {Bronze, 350},
  {Brass, 300},
  {Iron, 400},
  {Steel, 500},
  {Platinum, 600},
  {Glass, 12},
  {Leather, 125},
  {Cloth, 250},
  {Silk, 350},
  {Bone, 50},
  {Plant, 50},
  {Meat, 125},
  {Coal, 200}
};

std::map<Material, int> shearYields = {
  {Wood, 3},
  {Stone, 16},
  {Tin, 15},
  {Zinc, 10},
  {Copper, 21},
  {Bronze, 31},
  {Brass, 26},
  {Iron, 37},
  {Steel, 43},
  {Platinum, 51},
  {Glass, 19},
  {Leather, 1},
  {Cloth, 1},
  {Silk, 1},
  {Bone, 10},
  {Plant, 1},
  {Meat, 1},
  {Coal, 13}
};

std::map<Material, int> shearFractures = {
  {Wood, 75},
  {Stone, 100},
  {Tin, 100},
  {Zinc, 100},
  {Copper, 100},
  {Bronze, 200},
  {Brass, 150},
  {Iron, 300},
  {Steel, 400},
  {Platinum, 500},
  {Glass, 200},
  {Leather, 25},
  {Cloth, 1},
  {Silk, 1},
  {Bone, 50},
  {Plant, 15},
  {Meat, 15},
  {Coal, 100}
};

std::map<Material, std::string> materialNames = {
  {Wood, "wood"},
  {Dirt, "dirt"}, 
  {Stone, "stone"},
  {Tin, "tin"},
  {Zinc, "zinc"},
  {Copper, "copper"},
  {Bronze, "bronze"},
  {Brass, "brass"},
  {Iron, "iron"},
  {Steel, "steel"},
  {Platinum, "platinum"},
  {Glass, "glass"},
  {Leather, "leather"},
  {Cloth, "cloth"},
  {Silk, "silk"},
  {Bone, "bone"},
  {Plant, "plant"},
  {Meat, "meat"},
  {Coal, "coal"}
};

std::map<Material, std::string> materialColors = {
  {Wood, "130,82,1"},
  {Dirt, "155,118,83"}, 
  {Stone, "gray"},
  {Tin, "211,212,213"},
  {Zinc, "186,196,200"},
  {Copper, "184,115,51"},
  {Bronze, "205,127,50"},
  {Brass, "181,166,66"},
  {Iron, "203,205,205"},
  {Steel, "224,223,219"},
  {Platinum, "229,228,226"},
  {Glass, "201,219,220"},
  {Leather, "122,92,57"},
  {Cloth, "253,243,234"},
  {Silk, "255,248,220"},
  {Bone, "227,218,201"},
  {Plant, "58,95,11"},
  {Meat, "209,0,0"},
  {Coal, "dark gray"}
};

std::map<Material, int> GetDensities() {
  return densities;
}

std::map<Material, int> GetImpactYields() {
  return impactYields;
}

std::map<Material, int> GetImpactFractures() {
  return impactFractures;
}

std::map<Material, int> GetShearYields() {
  return shearYields;
}

std::map<Material, int> GetShearFractures() {
  return shearFractures;
}

std::map<Material, std::string> GetMaterialNames() {
  return materialNames;
}

std::map<Material, std::string> GetMaterialColors() {
  return materialColors;
}