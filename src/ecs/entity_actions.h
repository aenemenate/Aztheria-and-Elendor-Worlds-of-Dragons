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

#define EA_MOVE_ID		0
#define EA_ACTIVATEBLOCK_ID	1

class Move : public EntityAction {
public:
  int xdir, ydir, zdir;
  Move() : EntityAction() {}
  Move(int _xdir, int _ydir, int _zdir) : xdir(_xdir), ydir(_ydir), zdir(_zdir), EntityAction(EA_MOVE_ID) {}
  void Do(Entity *src, World *world);
};

class ActivateBlock : public EntityAction {
public:
  int xdir, ydir;
  ActivateBlock() : EntityAction() {}
  ActivateBlock(int _xdir, int _ydir) : xdir(_xdir), ydir(_ydir), EntityAction(EA_ACTIVATEBLOCK_ID) {}
  void Do(Entity *src, World *world);
};
