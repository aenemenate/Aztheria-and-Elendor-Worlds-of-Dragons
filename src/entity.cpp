#include "entity.h"
#include "game.h"
#include "world.h"
#include "map/area.h"

Entity::Entity(Graphic gset, std::string name, Position pos, int viewradius)
{
  this->gset = gset;
  this->name = name;
  this->pos = pos;
  this->viewradius = viewradius;
}

void Entity::Update(Game *game, bool isPlayer)
{
// update fov
  World* world = game->world;
  visiblepoints.clear();
  Area* map_ptr = world->GetArea(this->pos.wx, this->pos.wy);
  fov_circle(game->settings.get_fov_settings(), map_ptr, &visiblepoints, this->pos.x, this->pos.y, this->pos.z, viewradius);
  if (isPlayer) {
    for (int vp = 0; vp < visiblepoints.size(); vp++) {
      Position *point = &(visiblepoints[vp]);
      map_ptr->GetTile(point->x, point->y, point->z)->explored = true;
    }
  }
}

void Entity::Move(int xsign, int ysign, int zsign, World *world)
{
// clamp sign between -1 and 1
  xsign = (xsign <= -1) ? -1 : ((xsign >= 1) ? 1 : 0);
  ysign = (ysign <= -1) ? -1 : ((ysign >= 1) ? 1 : 0);
// set variables
  int new_x = this->pos.x + xsign, 
      new_y = this->pos.y + ysign,
      new_z = this->pos.z + zsign;
  int new_wx = this->pos.wx, new_wy = this->pos.wy;
  Area *curmap = world->GetArea(this->pos.wx,this->pos.wy);
// if new position is not within map bounds
  if (!curmap->PointWithinBounds(new_x, new_y) && new_z == this->pos.z) {
// check in each direction and set the new_wx, new_wy and new_x and new_y
    new_x = this->pos.x;
    new_y = this->pos.y;
    if (new_x + xsign < 0 && xsign == -1 && this->pos.wx - 1 >= 0) {
      new_x = new_x + xsign + curmap->width;
      new_wx -= 1;
    }
    else if (new_x + xsign >= curmap->width && xsign == 1 && this->pos.wx + 1 < world->width) {
      new_x = new_x + xsign - curmap->width;
      new_wx += 1;
    }
    if (new_y + ysign < 0 && ysign == -1 && this->pos.wy - 1 >= 0) {
      new_y = new_y + ysign + curmap->height;
      new_wy -= 1;
    }
    else if (new_y + ysign >= curmap->height && ysign == 1 && this->pos.wy + 1 < world->height) {
      new_y = new_y + ysign - curmap->height;
      new_wy += 1;
    }
  }
// else if this z is not the same as before
  else if (new_z != this->pos.z && curmap->PointWithinBounds(new_x, new_y)) {
    if (!(new_z > 0 && new_z <= curmap->GetDungeonFloors()->size())) {
      new_z = this->pos.z;
    }
  }
// else if it is on the current map, check if the new position is walkable
  else if (curmap->PointWithinBounds(new_x, new_y)
       &&  new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size()
       && !curmap->GetTile(new_x,new_y,new_z)->walkable 
       ||  curmap->GetEntity(new_x,new_y,new_z) != nullptr) {
    new_x = this->pos.x;
    new_y = this->pos.y;
    new_z = this->pos.z;
  }


// if we decided to move maps, check if the desired position is actually walkable
  if (new_wx != this->pos.wx || new_wy != this->pos.wy || new_z != this->pos.z) {
    Area *newmap = world->GetArea(new_wx, new_wy);
    if (!newmap->GetTile(new_x,new_y,new_z)->walkable 
    ||  newmap->GetEntity(new_x,new_y,new_z) != nullptr) {
      new_x = this->pos.x;
      new_y = this->pos.y;
      new_z = this->pos.z;
      new_wx = this->pos.wx;
      new_wy = this->pos.wy;
    }
  }
  world->GetArea(this->pos.wx, this->pos.wy)->SetEntity(this->pos.x,this->pos.y,this->pos.z,nullptr);
  world->GetArea(new_wx, new_wy)->SetEntity(new_x,new_y,new_z,this);
// set the new positions
  this->pos.x = new_x;
  this->pos.y = new_y;
  this->pos.z = new_z;
  this->pos.wx = new_wx;
  this->pos.wy = new_wy;
}