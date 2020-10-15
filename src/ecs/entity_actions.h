#pragma once

class World;
class Entity;

// Entity Action

class EntityAction {
public:
  int ID;
  EntityAction() {}
  EntityAction(int ID) : ID(ID) {}
  virtual ~EntityAction() {}
  virtual void Do(Entity *ent, World *world) = 0;
};

// Defined Actions 

#define EA_WANTSTOMOVE_ID	0

class WantsToMove : public EntityAction {
public:
  int xdir, ydir, zdir;
  WantsToMove() : EntityAction() {}
  WantsToMove(int _xdir, int _ydir, int _zdir) : xdir(_xdir), ydir(_ydir), zdir(_zdir), EntityAction(EA_WANTSTOMOVE_ID) {}
  void Do(Entity *src, World *world);
};