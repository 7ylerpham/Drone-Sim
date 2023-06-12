#include "BeelineStrategy.h"

BeelineStrategy::BeelineStrategy(Vector3 position, Vector3 destination)
    : position(position), destination(destination) {
  totalDistance = 0;
  totalDistance += sqrt(pow(position.x - destination.x, 2) +
                        pow(position.y - destination.y, 2) +
                        pow(position.z - destination.z, 2));
}

void BeelineStrategy::Move(IEntity* entity, double dt) {
  if (IsCompleted()) return;

  Vector3 dir = (destination - position).Unit();

  position = position + dir * entity->GetSpeed() * dt;
  entity->SetPosition(position);
  entity->SetDirection(dir);
}

bool BeelineStrategy::IsCompleted() {
  return position.Distance(destination) < 4.0;
}
