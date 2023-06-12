#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_

#include <vector>

#include "Drone.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "RechargeStation.h"
#include "math/vector3.h"

class RechargeStation;
class Drone;

/**
 * @class BatteryDecorator
 * @brief Represents a battery wrapped drone in a physical system.
 */
class BatteryDecorator : public IEntity {
 public:
  /**
   * @brief BatteryDecorators are created with a drone
   * @param drone IEntity object representing the wrapped drone
   */
  BatteryDecorator(Drone* dronem);

  /**
   * @brief Destructor for the BatteryDecorator
   */
  ~BatteryDecorator();

  /**
   * @brief Gets the speed of the wrapped drone
   * @return The speed of the wrapped drone
   */
  float GetSpeed() const { return drone->GetSpeed(); }

  /**
   * @brief Calculates if the wrapped drone has 
   * enough energy to complete the trip
   * @return Boolean representing if it can complete the trip
   */
  bool EnoughCharge();

  /**
   * @brief Gets the position of the wrapped drone
   * @return The position of the wrapped drone
   */
  Vector3 GetPosition() const { return this->drone->GetPosition(); }

  /**
   * @brief Gets the direction of the wrapped drone
   * @return The direction of the wrapped drone
   */
  Vector3 GetDirection() const { return drone->GetDirection(); }

  /**
   * @brief Gets the destination of the wrapped drone
   * @return The destination of the wrapped drone
   */
  Vector3 GetDestination() const { return drone->GetDestination(); }

  /**
   * @brief Gets the details information of the wrapped drone
   * @return The details information of the wrapped drone
   */
  JsonObject GetDetails() const { return drone->GetDetails(); }

  /**
   * @brief Gets the availability of the wrapped drone
   * @return The availability of the wrapped drone
   */
  bool GetAvailability() const { return drone->GetAvailability(); }

  /**
   * @brief Gets the nearest drone in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  void GetNearestEntity(std::vector<IEntity*> scheduler);

  /**
   * @brief Updates the wrapped drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that wrapped drone cannot be copied.
   */
  BatteryDecorator(const BatteryDecorator& battery) = delete;
  BatteryDecorator& operator=(const BatteryDecorator& battery) = delete;

  /**
   * @brief Set the wrapped drone's graph
   * @param graph New graph
   */
  void SetGraph(const IGraph* graph);

  /**
   * @brief Charges the wrapped drone'
   * @param dt Delta time
   */
  void Charging(double dt);

  /**
   * @brief Sets the wrapped drone's energy
   * @param x New energy
   */
  void SetEnergy(float x) { energy = x; }

  /**
   * @brief Get the wrapped drone's energy
   * @return the wrapped drone's energy
   */
  float GetEnergy();

  /**
   * @brief Gets the drone from the wrapper class
   * @return A pointer to the drone
   */
  Drone* GetDrone() { return drone; }

  /**
   * @brief Gets the vector of RechargeStations
   * @return A vector of RehargeStations
   */
  std::vector<RechargeStation*> GetRechargeStations() {
    return rechargeStations;
  }
  /**
   * @brief Sets the vector of RechargeStations
   * @param stations A new vector of RechargeStations
   */
  void SetRechargeStations(std::vector<RechargeStation*> stations) {
    rechargeStations = stations;
  }

  /**
   * @brief Gets the wrapped drone's nearest recharge station
   * @param pos A Vector3 representing the drone's current position
   * @return A RechargStation pointer to the nearest RechargeStation
   */
  RechargeStation* GetNearestStation(Vector3 pos);

  /**
   * @brief Updates the wrapped drone's nearest recharge station
   * @param pos A Vector3 representing the drone's current position
   */
  void UpdateNearestStation(Vector3 pos);

 private:
  float energy;
  bool chrg = true;
  float total = 0;
  bool currCharging = 0;
  Drone* drone;
  IStrategy* toRecharge = nullptr;
  RechargeStation* nearestStation;
  std::vector<RechargeStation*> rechargeStations;
};

#endif
