#define _USE_MATH_DEFINES
#include "Helicopter.h"

#include <cmath>
#include <limits>

#include "BeelineStrategy.h"

Helicopter::Helicopter(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];
  originalSpeed = speed;
  weather = Weather::GetWeather();
}

Helicopter::~Helicopter() {
  // Delete dynamically allocated variables
  delete graph;
  delete weather;
}

void Helicopter::CreateNewDestination() {
  destination = {Random(-1400, 1500), position.y, Random(-800, 800)};
  toDestination = new BeelineStrategy(position, destination);
}

void Helicopter::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

void Helicopter::Update(double dt, std::vector<IEntity*> scheduler) {
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

  if (toDestination) {
    if (toDestination->IsCompleted()) {
      CreateNewDestination();
    } else {
      toDestination->Move(this, dt);
    }
  } else {
    CreateNewDestination();
  }
}
