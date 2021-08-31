#include "entity_actions.h"
#include "../game.h"
#include "../world/world.h"
#include "../map/area.h"
#include "entity.h"
#include "../materials.h"

#include <random>
#include <string>
#include <iostream>

int Attack(Entity *src, Entity *def, World *world) {
  std::string message;
  if (src->HasComponent(EC_STATS_ID) && def->HasComponent(EC_STATS_ID)) {
    std::shared_ptr<Stats> src_stats = dynamic_pointer_cast<Stats>(src->GetComponent(EC_STATS_ID));
    std::shared_ptr<Stats> def_stats = dynamic_pointer_cast<Stats>(def->GetComponent(EC_STATS_ID));
    std::shared_ptr<Name> src_name = dynamic_pointer_cast<Name>(src->GetComponent(EC_NAME_ID));
    std::shared_ptr<Name> def_name = dynamic_pointer_cast<Name>(def->GetComponent(EC_NAME_ID));
    bool attackLanded = rand()%100 <= (src_stats->skills[Brawling] + src_stats->attributes[Speed] / 5 + src_stats->attributes[Luck] / 10) 
			* (0.75 + 0.5 * src_stats->resources[Stamina] / src_stats->resources[MaxStamina]);
    if (attackLanded) {
      bool attackEvaded = rand()%100 <= (src_stats->attributes[Speed] / 5 + def_stats->attributes[Luck] / 10) 
			* (0.75 + 0.5 * def_stats->resources[Stamina] / def_stats->resources[MaxStamina]);
      if (!attackEvaded) {
        int damage = impactYields[Bone] * ((src_stats->attributes[Strength] + 50) / 100);
        message = src_name->name + " attacked " + def_name->name + " for " + std::to_string(damage) + " points.";
	def_stats->resources[Health] -= damage;
        if (def_stats->resources[Health] <= 0) {
          def_name->name = "dead " + def_name->name;
          std::shared_ptr<Renderable> def_renderable = dynamic_pointer_cast<Renderable>(def->GetComponent(EC_RENDERABLE_ID));
          def_renderable->graphic.fgcolor = "red";
        }
      }
      else {
        message = def_name->name + " evaded " + src_name->name + "\'s attack.";
      }
    }
    else {
      message = src_name->name + " attacked " + def_name->name + " and missed.";
    }
    if (src->HasComponent(EC_PLAYER_ID) || def->HasComponent(EC_PLAYER_ID)) {
      world->msgConsole.PushLine(message);
    }
    return 1000;
  }
  else
    return 0;
}

int Move::Do(Entity *src, World *world) {
  int cost = 0;
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
    if (!curmap->PointWithinBounds(new_x, new_y) && new_z == pos->z && pos->z == 0) {
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
    else if (curmap->PointWithinBounds(new_x, new_y)) {
      if (new_z >= 0 && new_z <= curmap->GetDungeonFloors()->size()
      && !curmap->GetTile(new_x, new_y, new_z)->walkable
      || curmap->GetBlock(new_x, new_y, new_z)->solid
      || curmap->GetEntity(new_x, new_y, new_z) != nullptr) {
        if (curmap->GetEntity(new_x, new_y, new_z) != nullptr)
          cost = Attack(src, curmap->GetEntity(new_x, new_y, new_z), world);
        new_x = pos->x;
        new_y = pos->y;
        new_z = pos->z;
      }
    }
    else if (!curmap->PointWithinBounds(new_x, new_y)) {
      new_x = pos->x;
      new_y = pos->y;
    }
// if we decided to move maps, check if the desired position is actually walkable
    if (new_wx != pos->wx || new_wy != pos->wy || new_z != pos->z) {
      Area *newmap = world->GetArea(new_wx, new_wy);
      if (!newmap->GetTile(new_x,new_y,new_z)->walkable
      ||  newmap->GetBlock(new_x,new_y,new_z)->solid
      ||  newmap->GetEntity(new_x, new_y, new_z) != nullptr) {
        if (newmap->GetEntity(new_x, new_y, new_z) != nullptr)
          cost = Attack(src, newmap->GetEntity(new_x, new_y, new_z), world);
	new_x = pos->x;
	new_y = pos->y;
	new_z = pos->z;
	new_wx = pos->wx;
	new_wy = pos->wy;
      }
    }
    if (new_x != pos->x || new_y != pos->y || new_z != pos->z || new_wx != pos->wx || new_wy != pos->wy) {
      world->GetArea(pos->wx, pos->wy)->SetEntity(pos->x,pos->y,pos->z,nullptr);
      world->GetArea(new_wx, new_wy)->SetEntity(new_x,new_y,new_z,src);
      cost = 1000;
    }
// set the new positions
    pos->x = new_x;
    pos->y = new_y;
    pos->z = new_z;
    pos->wx = new_wx;
    pos->wy = new_wy;
  }
  return cost;
}

int ActivateBlock::Do(Entity *src, World *world) {
  int cost = 0;
  if (src->HasComponent(EC_POSITION_ID)) {
    Position this_pos = ( dynamic_pointer_cast<EntPosition>( src->GetComponent(EC_POSITION_ID) ) )->position;
    Position block_pos = { this_pos.x + this->xdir, this_pos.y + this->ydir, this_pos.z, this_pos.wx, this_pos.wy };
    world->GetArea(block_pos.wx, block_pos.wy)->GetBlock(block_pos.x, block_pos.y, block_pos.z)->Activate((void*)src, world);
    cost = 1000;
  }
  return cost;
}
