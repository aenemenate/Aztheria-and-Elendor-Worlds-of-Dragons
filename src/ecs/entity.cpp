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

void Entity::Tick(Game *game) {
  for (auto c : components) c->Tick(this, game);
}

void Entity::Act(World *world) {
  if (actions.size() > 0) {
    actions[0]->Do(this, world);
    actions.erase(actions.begin());
  }
}