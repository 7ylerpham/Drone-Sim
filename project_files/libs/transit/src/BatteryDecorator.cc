#include "BatteryDecorator.h"

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "Drone.h"
#include "Weather.h"

BatteryDecorator::BatteryDecorator(Drone* dronem) {
  energy = 300.0;
  drone = dronem;
}

BatteryDecorator::~BatteryDecorator() {
  // Delete dynamically allocated variables
  for (int i = 0; i < rechargeStations.size(); i++) {
    delete rechargeStations[i];
  }
  delete drone;
  delete nearestStation;
  delete toRecharge;
}

void BatteryDecorator::UpdateNearestStation(Vector3 pos) {
  float minDis = std::numeric_limits<float>::max();
  for (auto station : GetRechargeStations()) {
    float disToStation = pos.Distance(station->GetPosition());
    if (disToStation <= minDis) {
      minDis = disToStation;
      nearestStation = station;
    }
  }
}

void BatteryDecorator::Update(double dt, std::vector<IEntity*> scheduler) {
  Weather* weather = Weather::GetWeather();
  if (drone->GetIsFixer()) {
    IStrategy* toDrone = drone->GetToRobot();
    IStrategy* toHome = drone->GetToFinalDestination();
    Vector3 direction = drone->GetDirection();
    float directionX = direction[0];
    float directionY = direction[1];
    float windX = weather->GetWind()[0];
    float windY = (weather->GetWind())[1];
    float adjustedSpeed =
        ((windX * directionX) + (windY * directionY)) /
        (std::pow((std::pow(directionX, 2) + std::pow(directionY, 2)), 1 / 2));

    int newSpeed = 30 + adjustedSpeed;
    if (newSpeed >= 0 && weather->GetWeatherType() != "tornado") {
      drone->SetSpeed(newSpeed);
    } else {
      drone->SetSpeed(0);
    }
    if (toDrone) {
      toDrone->Move(drone, dt);
      if (toDrone->IsCompleted()) {
        delete toDrone;
        drone->SetToRobot(nullptr);
      }
    } else if (toHome) {
      toHome->Move(drone, dt);
      if (toHome->IsCompleted()) {
        delete toHome;
        drone->SetToFinalDestination(nullptr);
      }
    }
  } else {               // Not repair drones
    if (currCharging) {  // Currently charging
      std::cout << "Charging: " << energy << std::endl;
      Charging(dt);

    } else {  // Not currently charging
      // Drone has energy
      if (energy > 0) {
        // Drone is available for next robot
        if (drone->GetAvailability()) {
          drone->GetNearestEntity(scheduler);
          IStrategy* toRobot = drone->GetToRobot();
          // If drone has a path to robot, check if need recharge
          if (toRobot) {
            chrg = EnoughCharge();
          }
        }
        // Drone wants to pick up robot
        if (drone->GetToRobot()) {
          // We have enough charge to pick up robot
          if (chrg) {
            drone->Update(dt, scheduler);
            SetEnergy(energy - dt);
          } else {  // Not enough charge to pick up robot, go to recharge
                    // station

            Vector3 direction =
                ((nearestStation->GetPosition() - this->GetPosition()).Unit());
            float directionX = direction[0];
            float directionY = direction[1];
            float windX = weather->GetWind()[0];
            float windY = (weather->GetWind())[1];
            float adjustedSpeed =
                ((windX * directionX) + (windY * directionY)) /
                (std::pow((std::pow(directionX, 2) + std::pow(directionY, 2)),
                          1 / 2));

            int newSpeed = 30 + adjustedSpeed;
            if (newSpeed >= 0 && weather->GetWeatherType() != "tornado") {
              drone->SetSpeed(newSpeed);
            } else {
              drone->SetSpeed(0);
            }

            UpdateNearestStation(drone->GetPosition());
            toRecharge = new BeelineStrategy(this->GetPosition(),
                                             nearestStation->GetPosition());
            // Update drone toRobot pathing to start from recharge station
            drone->SetToRobot(new BeelineStrategy(nearestStation->GetPosition(),
                                                  drone->GetDestination()));
            toRecharge->Move(drone, dt);
            SetEnergy(energy - dt);
            if (toRecharge->IsCompleted()) {
              delete toRecharge;
              toRecharge = nullptr;
              currCharging = true;
            }
          }
        } else if (drone->GetToFinalDestination()) {
          // drone goes to finalDestination
          drone->Update(dt, scheduler);
          SetEnergy(energy - dt);
          // uncomment to test fixer drone logic
        }
      } else {  // drone is out of energy and is dead
        UpdateNearestStation(drone->GetPosition());
        Vector3 nearest = nearestStation->GetPosition();
        for (auto station : GetRechargeStations()) {
          if (station->GetPosition().x == nearest.x &&
              station->GetPosition().y == nearest.y &&
              station->GetPosition().z == nearest.z) {
            Vector3 homePos = station->GetPosition();
            homePos.y = 400;
            // Get repair drone
            BatteryDecorator* repair = station->GetRepairDrone();

            repair->drone->SetToRobot(new BeelineStrategy(
                repair->GetPosition(), drone->GetPosition()));
            // if to robot path is not completed update direction
            if (repair->drone->GetToRobot()->IsCompleted() == false) {
              repair->SetDirection(
                  (drone->GetPosition() - repair->GetPosition()).Unit());
            }
            repair->drone->SetToFinalDestination(
                new BeelineStrategy(repair->GetPosition(), homePos));
            // if to recharge station path is not completed update direction
            if (repair->drone->GetToFinalDestination()->IsCompleted() ==
                false) {
              repair->SetDirection(
                  (nearestStation->GetPosition() - repair->GetPosition())
                      .Unit());
            }
            if (repair->drone->GetToRobot()->IsCompleted()) {
              currCharging = true;
            }
          }
        }
      }
    }
  }
}

void BatteryDecorator::Charging(double dt) {
  if (energy < 300) {
    SetEnergy(energy + (dt * 50));
    currCharging = true;
  } else {
    currCharging = false;
    chrg = true;
  }
}

bool BatteryDecorator::EnoughCharge() {
  UpdateNearestStation(drone->GetPosition());
  float distanceToRobot = drone->GetToRobot()->GetTotalDistance();
  float distanceToFinal = drone->GetToFinalDestination()->GetTotalDistance();
  if (toRecharge == nullptr) {
    toRecharge =
        new BeelineStrategy(this->GetPosition(), nearestStation->GetPosition());
  }
  float distanceToRecharge = toRecharge->GetTotalDistance();
  float total = distanceToRobot + distanceToFinal + distanceToRecharge;
  // energy is 1 unit of power / 22.25 units of distance
  return total < energy * 22.25;
}

void BatteryDecorator::SetGraph(const IGraph* graph) { drone->SetGraph(graph); }

float BatteryDecorator::GetEnergy() { return this->energy; }
