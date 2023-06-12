#ifndef RECHARGE_STATION_H_
#define RECHARGE_STATION_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"
#include "BatteryDecorator.h"

class BatteryDecorator;

/**
 * @class RechargeStation
 * @brief Represents a RechargeStation in a physical system. 
 * RechargeStations move using euler integration based on a 
 * specified velocity and direction.
 */
class RechargeStation : public IEntity {
 public:
  /**
   * @brief RechargeStations are created with a name
   * @param obj JSON object containing the RechargeStation's information
   */
  RechargeStation(JsonObject& obj);

  /**
   * @brief Destructor
   */  
  ~RechargeStation() {delete repairDrone; }

  /**
   * @brief Gets the speed of the RechargeStation
   * @return The speed of the RechargeStation
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the RechargeStation
   * @return The position of the RechargeStation
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the RechargeStation
   * @return The direction of the RechargeStation
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the RechargeStation
   * @return The destination of the RechargeStation
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the RechargeStation
   * @return The details information of the RechargeStation
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Sets the position of the RechargeStation
   * @param pos_ The new position of the RechargeStation
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the RechargeStation
   * @param dir_ The new direction of the RechargeStation
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the RechargeStation
   * @param des_ The new destination of the RechargeStation
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that RechargeStations cannot be copied.
   */
  RechargeStation(const RechargeStation& RechargeStation) = delete;
  RechargeStation& operator=(const RechargeStation& RechargeStation) = delete;

  /**
   * @brief Sets the repairDrone of the RechargeStation
   * @param repair The new repairDrone of the RechargeStation
   */
  void SetRepairDrone(BatteryDecorator* repair) { this->repairDrone = repair; }

  /**
   * @brief Gets the repairDrone of the RechargeStation
   * @Return The repiarDrone of the RechargeStation
   */
  BatteryDecorator* GetRepairDrone() { return repairDrone; }

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  bool available;
  BatteryDecorator* repairDrone;
};

#endif
