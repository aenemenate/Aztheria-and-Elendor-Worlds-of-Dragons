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

void Entity::Move(int xsign, int ysign, World *world)
{
// clamp sign between -1 and 1
  xsign = (xsign <= -1) ? -1 : ((xsign >= 1) ? 1 : 0);
  ysign = (ysign <= -1) ? -1 : ((ysign >= 1) ? 1 : 0);
// set variables
  int newx = this->pos.x + xsign, 
      newy = this->pos.y + ysign;
  int new_wx = this->pos.wx, new_wy = this->pos.wy;
  Area *curmap = world->GetArea(this->pos.wx,this->pos.wy);
// if new position is not within map
  if (!curmap->PointWithinBounds(newx, newy)) {
// check in each direction to move there if necessary
    newx = this->pos.x;
    newy = this->pos.y;
    if (newx + xsign < 0 && xsign == -1 && this->pos.wx - 1 >= 0) {
      newx = newx + xsign + curmap->width;
      new_wx -= 1;
    }
    else if (newx + xsign >= curmap->width && xsign == 1 && this->pos.wx + 1 < world->width) {
      newx = newx + xsign - curmap->width;
      new_wx += 1;
    }
    if (newy + ysign < 0 && ysign == -1 && this->pos.wy - 1 >= 0) {
      newy = newy + ysign + curmap->height;
      new_wy -= 1;
    }
    else if (newy + ysign >= curmap->height && ysign == 1 && this->pos.wy + 1 < world->height) {
      newy = newy + ysign - curmap->height;
      new_wy += 1;
    }
  }
// else if it is on the current map, check if the new position is walkable
  else if (!curmap->GetTile(newx,newy,0)->walkable 
       ||  curmap->ent_map[newx*curmap->width + newy] != nullptr) {
    newx = this->pos.x;
    newy = this->pos.y;
  }
// if we decided to move maps, check if the desired position is actually walkable
  if (new_wx != this->pos.wx || new_wy != this->pos.wy) {
    Area *newmap = world->GetArea(new_wx, new_wy);
    if (!newmap->GetTile(newx,newy,0)->walkable 
    ||  newmap->ent_map[newx*newmap->width + newy] != nullptr) {
      newx = this->pos.x;
      newy = this->pos.y;
      new_wx = this->pos.wx;
      new_wy = this->pos.wy;
    }
  }
  world->GetArea(this->pos.wx, this->pos.wy)->ent_map[this->pos.x*curmap->width + this->pos.y] = nullptr;
  world->GetArea(new_wx, new_wy)->ent_map[newx*curmap->width + newy] = this;
// set the new positions
  this->pos.x = newx;
  this->pos.y = newy;
  this->pos.wx = new_wx;
  this->pos.wy = new_wy;
}