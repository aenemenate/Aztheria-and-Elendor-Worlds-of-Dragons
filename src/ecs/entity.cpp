#include "entity.h"
#include "../game.h"
#include "../world.h"
#include "../map/area.h"

void Entity::RemoveComponent(int component_type) {
  components.erase(
    std::remove_if(components.begin(), components.end(), [component_type](std::shared_ptr<EntityComponent> c){ return c->ID != component_type; }),
    components.end()
  );
}

void Entity::Tick(Game *game, int prio) {
  for (auto c : components)
    if (c->prio == prio)
      c->Tick(this, game);
}

bool Entity::Act(World *world) {
  if (actions.size() > 0) {
    actions[0]->Do(this, world);
    actions.erase(actions.begin());
    return true;
  }
  return false;
}
