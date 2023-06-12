#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "SpinDecorator.h"
#include "Weather.h"

Drone::Drone(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];

  available = true;
  originalSpeed = speed;
  weather = Weather::GetWeather();
}

Drone::~Drone() {
  // Delete dynamically allocated variables
  delete nearestEntity;
  delete toRobot;
  delete toFinalDestination;
  delete weather;
}

void Drone::GetNearestEntity(std::vector<IEntity*> scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      float disToEntity = this->position.Distance(entity->GetPosition());
      if (disToEntity <= minDis) {
        minDis = disToEntity;
        nearestEntity = entity;
      }
    }
  }

  if (nearestEntity) {
    // set availability to the nearest entity
    nearestEntity->SetAvailability(false);
    available = false;
    pickedUp = false;

    destination = nearestEntity->GetPosition();
    finalDestination = nearestEntity->GetDestination();

    toRobot = new BeelineStrategy(position, destination);

    std::string strat = nearestEntity->GetStrategyName();
    if (strat == "astar")
      toFinalDestination = new JumpDecorator(
          new AstarStrategy(destination, finalDestination, graph));
    else if (strat == "dfs")
      toFinalDestination = new SpinDecorator(new JumpDecorator(
          new DfsStrategy(destination, finalDestination, graph)));
    else if (strat == "dijkstra")
      toFinalDestination = new JumpDecorator(new SpinDecorator(
          new DijkstraStrategy(destination, finalDestination, graph)));
    else
      toFinalDestination = new BeelineStrategy(destination, finalDestination);
  }
}
// afffected entities should have a reference to the singleton and every update
// check to see if the season has changed, then change the speed accordingly
void Drone::Update(double dt, std::vector<IEntity*> scheduler) {
  if (available) GetNearestEntity(scheduler);

  float directionX = direction[0];
  float directionY = direction[1];
  float windX = (weather->GetWind())[0];
  float windY = (weather->GetWind())[1];
  float adjustedSpeed =
      ((windX * directionX) + (windY * directionY)) /
      (std::pow((std::pow(directionX, 2) + std::pow(directionY, 2)), 1 / 2));

  int newSpeed = originalSpeed + adjustedSpeed;
  if (newSpeed >= 0 && weather->GetWeatherType() != "tornado") {
    speed = newSpeed;
  } else {
    speed = 0;
  }

  // std::cout<<"DRONE SPEED IS NOW "<<speed<<std::endl;

  if (toRobot) {
    toRobot->Move(this, dt);
    if (toRobot->IsCompleted()) {
      delete toRobot;
      toRobot = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination) {
    toFinalDestination->Move(this, dt);

    if (nearestEntity && pickedUp) {
      nearestEntity->SetPosition(position);
      nearestEntity->SetDirection(direction);
    }

    if (toFinalDestination->IsCompleted()) {
      delete toFinalDestination;
      toFinalDestination = nullptr;
      nearestEntity = nullptr;
      available = true;
      pickedUp = false;
    }
  }
}

void Drone::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

void Drone::Jump(double height) {
  if (goUp) {
    position.y += height;
    jumpHeight += height;
    if (jumpHeight > 5) {
      goUp = false;
    }
  } else {
    position.y -= height;
    jumpHeight -= height;
    if (jumpHeight < 0) {
      goUp = true;
    }
  }
}
