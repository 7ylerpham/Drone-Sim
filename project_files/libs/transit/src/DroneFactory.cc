#include "DroneFactory.h"

#include "BatteryDecorator.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    Drone* currDrone = new Drone(entity);
    BatteryDecorator* bat = new BatteryDecorator(currDrone);
    return bat;
  }
  return nullptr;
}
