#include "SimulationModel.h"

#include "BatteryDecorator.h"
#include "DroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "RechargeStationFactory.h"
#include "RobotFactory.h"
#include "util/json.h"

SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  compFactory = new CompositeFactory();
  AddFactory(new DroneFactory());
  AddFactory(new RobotFactory());
  AddFactory(new HumanFactory());
  AddFactory(new HelicopterFactory());
  AddFactory(new RechargeStationFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (int i = 0; i < entities.size(); i++) {
    delete entities[i];
  }
  for (int i = 0; i < stations.size(); i++) {
    delete stations[i];
  }
  for (int i = 0; i < scheduler.size(); i++) {
    delete scheduler[i];
  }
  delete graph;
  delete compFactory;
}

void SimulationModel::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  std::vector<RechargeStation*> tempStations;
  if (type.compare("rechargestation") == 0) {
    for (int i = 0; i < 3; i++) {
      entity["name"] = "RStation-" + std::to_string(i);
      std::string name = entity["name"];
      JsonArray arr(3);
      arr[0] = -1300 + (1300 * i);
      arr[1] = 500;
      arr[2] = 500 + (-500 * i);

      entity["position"] = arr;
      JsonArray position = entity["position"];
      std::cout << name << ": " << position << std::endl;

      RechargeStation* newEntity =
          dynamic_cast<RechargeStation*>(compFactory->CreateEntity(entity));
      newEntity->SetGraph(graph);

      // Call AddEntity to add it to the view
      controller.AddEntity(*newEntity);
      tempStations.push_back(newEntity);
    }
    stations = tempStations;
  } else if (type.compare("drone") == 0) {
    std::cout << stations.size() << std::endl;
    std::string name = entity["name"];
    JsonArray position = entity["position"];
    std::cout << name << ": " << position << std::endl;

    BatteryDecorator* myNewEntity =
        dynamic_cast<BatteryDecorator*>(compFactory->CreateEntity(entity));
    myNewEntity->SetGraph(graph);
    myNewEntity->SetRechargeStations(stations);
    // IEntity* temp = dynamic_cast<IEntity*>(myNewEntity);

    // Call AddEntity to add it to the view
    controller.AddEntity(*myNewEntity);
    entities.push_back(myNewEntity);

    // Create repair drones for each recharge station
    for (int i = 0; i < stations.size(); i++) {
      entity["name"] = "RepairDrone-" + std::to_string(i);
      std::cout << entity["name"] << std::endl;

      JsonArray arr(3);
      arr[0] = -1300 + (1300 * i);
      arr[1] = 400;
      arr[2] = 500 + (-500 * i);

      entity["position"] = arr;
      JsonArray position = entity["position"];
      std::cout << name << ": " << position << std::endl;

      // Create and set isFixer
      BatteryDecorator* wrappedDrone =
          dynamic_cast<BatteryDecorator*>(compFactory->CreateEntity(entity));
      wrappedDrone->GetDrone()->SetIsFixer(true);
      wrappedDrone->SetGraph(graph);

      // link station with repair drone
      RechargeStation* station = dynamic_cast<RechargeStation*>(stations.at(i));
      station->SetRepairDrone(wrappedDrone);

      // Call AddEntity to add it to the view
      controller.AddEntity(*dynamic_cast<IEntity*>(wrappedDrone));
      entities.push_back(dynamic_cast<IEntity*>(wrappedDrone));
    }
  } else {
    std::string name = entity["name"];
    JsonArray position = entity["position"];
    std::cout << name << ": " << position << std::endl;

    IEntity* myNewEntity = compFactory->CreateEntity(entity);
    myNewEntity->SetGraph(graph);

    // Call AddEntity to add it to the view
    controller.AddEntity(*myNewEntity);
    entities.push_back(myNewEntity);
  }
}

/// Schedules a trip for an object in the scene
void SimulationModel::ScheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  std::cout << name << ": " << start << " --> " << end << std::endl;

  for (auto entity : entities) {  // Add the entity to the scheduler
    JsonObject detailsTemp = entity->GetDetails();
    std::string nameTemp = detailsTemp["name"];
    std::string typeTemp = detailsTemp["type"];
    if (name.compare(nameTemp) == 0 && typeTemp.compare("robot") == 0 &&
        entity->GetAvailability()) {
      std::string strategyName = details["search"];
      entity->SetDestination(Vector3(end[0], end[1], end[2]));
      entity->SetStrategyName(strategyName);
      scheduler.push_back(entity);
      break;
    }
  }
  controller.SendEventToView("TripScheduled", details);
}

/// Updates the simulation
void SimulationModel::Update(double dt) {
  for (int i = 0; i < entities.size(); i++) {
    entities[i]->Update(dt, scheduler);
    controller.UpdateEntity(*entities[i]);
  }
}

void SimulationModel::AddFactory(IEntityFactory* factory) {
  compFactory->AddFactory(factory);
}
