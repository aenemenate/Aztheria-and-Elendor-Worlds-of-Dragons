#pragma once

class World;
class Entity;

/* An action stored as an object, can perform itself (upon which it is deleted),
   and has data which are arguments */
class EntityAction {
public:
  int ID;
  EntityAction() {}
  EntityAction(int ID) : ID(ID) {}
  virtual ~EntityAction() {}
  virtual int Do(Entity *ent, World *world) = 0;
};

#define EA_MOVE_ID		0
#define EA_ACTIVATEBLOCK_ID	1
#define EA_USEITEM_ID		2
#define EA_UNEQUIP_ID		3
#define EA_DROPITEM_ID		4
#define EA_GETFROMBODY_ID	5

int Attack(Entity *src, Entity *def, World *world);

/* All-purpose move, takes a direction (x, y, and z), and tries to move that
 * direction. It will move maps if necessary.
 */
class Move : public EntityAction {
public:
  int xdir, ydir, zdir;
  Move() : EntityAction() {}
  Move(int _xdir, int _ydir, int _zdir) : xdir(_xdir), ydir(_ydir), zdir(_zdir), EntityAction(EA_MOVE_ID) {}
  int Do(Entity *src, World *world);
};

/* Can activate any block in a radius. */
class ActivateBlock : public EntityAction {
public:
  int xdir, ydir;
  ActivateBlock() : EntityAction() {}
  ActivateBlock(int _xdir, int _ydir) : xdir(_xdir), ydir(_ydir), EntityAction(EA_ACTIVATEBLOCK_ID) {}
  int Do(Entity *src, World *world);
};

class UseItem : public EntityAction {
public:
  int itemIndex;
  UseItem() : EntityAction() {}
  UseItem(int itemIndex) : itemIndex(itemIndex), EntityAction(EA_USEITEM_ID) {}
  int Do(Entity *src, World *world);
};

class Unequip : public EntityAction {
public:
  int itemIndex;
  Unequip() : EntityAction() {}
  Unequip(int itemIndex) : itemIndex(itemIndex), EntityAction(EA_UNEQUIP_ID) {}
  int Do(Entity *src, World *world);
};

class DropItem : public EntityAction {
public:
  int itemIndex;
  DropItem() : EntityAction() {}
  DropItem(int itemIndex) : itemIndex(itemIndex), EntityAction(EA_DROPITEM_ID) {}
  int Do(Entity *src, World *world);
};

class GetFromBody : public EntityAction {
public:
  int itemIndex;
  Entity *body;
  GetFromBody() : EntityAction() {}
  GetFromBody(int itemIndex, Entity *body) : itemIndex(itemIndex), body(body), EntityAction(EA_GETFROMBODY_ID) {}
  int Do(Entity *src, World *world);
};