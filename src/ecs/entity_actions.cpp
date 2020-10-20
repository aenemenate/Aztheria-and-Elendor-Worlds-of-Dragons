#include "entity_actions.h"
#include "../game.h"
#include "../world.h"
#include "../map/area.h"
#include "entity.h"

void Move::Do(Entity *src, World *world) {
// clamp sign between -1 and 1
  int xsign = (xdir <= -1) ? -1 : ((xdir >= 1) ? 1 : 0);
  int ysign = (ydir <= -1) ? -1 : ((ydir >= 1) ? 1 : 0);
  int zsign = (zdir <= -1) ? -1 : ((zdir >= 1) ? 1 : 0);
// set variables
  if (src->HasComponent(EC_POSITION_ID)) {
    std::shared_ptr<EntPosition> pos_c = dynamic_pointer_cast<EntPosition>(src->GetComponent(EC_POSITION_ID));
    Position *pos = &(pos_c->position);
    int new_x = pos->x + xsign, 
        new_y = pos->y + ysign,
        new_z = pos->z + zsign;
    int new_wx = pos->wx, new_wy = pos->wy;
    Area *curmap = world->GetArea(pos->wx,pos->wy);
// if new position is not within map bounds
    if (!curmap->PointWithinBounds(new_x, new_y) && new_z == pos->z) {
// check in each direction and set the new_wx, new_wy and new_x and new_y
      new_x = pos->x;
      new_y = pos->y;
      if (new_x + xsign < 0 && xsign == -1 && pos->wx - 1 >= 0) {
        new_x = new_x + xsign + curmap->width;
        new_wx -= 1;
      }
      else if (new_x + xsign >= curmap->width && xsign == 1 && pos->wx + 1 < world->width) {
        new_x = new_x + xsign - curmap->width;
        new_wx += 1;
      }
      if (new_y + ysign < 0 && ysign == -1 && pos->wy - 1 >= 0) {
        new_y = new_y + ysign + curmap->height;
        new_wy -= 1;
      }
      else if (new_y + ysign >= curmap->height && ysign == 1 && pos->wy + 1 < world->height) {
        new_y = new_y + ysign - curmap->height;
        new_wy += 1;
      }
    }
// else if this z is not the same as before
    else if (new_z != pos->z && curmap->PointWithinBounds(new_x, new_y)) {
      if (!(new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size())) {
        new_z = pos->z;
      }
    }
// else if it is on the current map, check if the new position is walkable
    else if (curmap->PointWithinBounds(new_x, new_y)
         &&  new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size()
         && !curmap->GetTile(new_x,new_y,new_z)->walkable 
         ||  curmap->GetEntity(new_x,new_y,new_z) != nullptr
         || curmap->GetBlock(new_x,new_y,new_z)->solid) {
      new_x = pos->x;
      new_y = pos->y;
      new_z = pos->z;
    }
// if we decided to move maps, check if the desired position is actually walkable
    if (new_wx != pos->wx || new_wy != pos->wy || new_z != pos->z) {
      Area *newmap = world->GetArea(new_wx, new_wy);
      if (!newmap->GetTile(new_x,new_y,new_z)->walkable 
      ||  newmap->GetEntity(new_x,new_y,new_z) != nullptr
      ||  newmap->GetBlock(new_x,new_y,new_z)->solid) {
	new_x = pos->x;
	new_y = pos->y;
	new_z = pos->z;
	new_wx = pos->wx;
	new_wy = pos->wy;
      }
    }
    world->GetArea(pos->wx, pos->wy)->SetEntity(pos->x,pos->y,pos->z,nullptr);
    world->GetArea(new_wx, new_wy)->SetEntity(new_x,new_y,new_z,src);
    if (new_wx != pos->wx || new_wy != pos->wy || new_z != pos->z)
      world->GetArea(new_wx, new_wy)->ClearEnts();
// set the new positions
    pos->x = new_x;
    pos->y = new_y;
    pos->z = new_z;
    pos->wx = new_wx;
    pos->wy = new_wy;
  }
}

void ActivateBlock::Do(Entity *src, World *world) {
  if (src->HasComponent(EC_POSITION_ID)) {
    Position this_pos = ( dynamic_pointer_cast<EntPosition>( src->GetComponent(EC_POSITION_ID) ) )->position;
    Position block_pos = { this_pos.x + this->xdir, this_pos.y + this->ydir, this_pos.z, this_pos.wx, this_pos.wy };
    world->GetArea(block_pos.wx, block_pos.wy)->GetBlock(block_pos.x, block_pos.y, block_pos.z)->Activate((void*)src, world);
  }
}
