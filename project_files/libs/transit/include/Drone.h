#ifndef DRONE_H_
#define DRONE_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "Weather.h"
#include "math/vector3.h"

/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(JsonObject& obj);

  /**
   * @brief Destructor for the drone
   */
  ~Drone();

  /**
   * @brief Gets the speed of the drone
   * @return The speed of the drone
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the drone
   * @return The position of the drone
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the drone
   * @return The direction of the drone
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the drone
   * @return The destination of the drone
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the color of the drone
   * @return The color of the drone
   */
  std::string GetColor() const { return color; }

  /**
   * @brief Gets the details information of the drone
   * @return The details information of the drone
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Gets the availability of the drone
   * @return The availability of the drone
   */
  bool GetAvailability() const { return available; }
  /**
   * @brief Gets the nearest entity in the scheduler
   * @param scheduler Vector containing all the entities in the system
   */
  void GetNearestEntity(std::vector<IEntity*> scheduler);

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the drone
   * @param pos_ The new position of the drone
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the drone
   * @param dir_ The new direction of the drone
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the drone
   * @param des_ The new destination of the drone
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Sets the color of the drone
   * @param col_ The new color of the drone
   */
  void SetColor(std::string col_) { color = col_; }

  /**
   * @brief Sets the speed of the drone
   * @param s_ The new speed of the drone
   */
  void SetSpeed(float s_) { speed = s_; }

  /**
   * @brief Rotates the drone
   * @param angle The angle by which the drone should be rotated
   */
  void Rotate(double angle);

  /**
   * @brief Makes the drone jump
   * @param height The height at which the drone should jump
   */
  void Jump(double height);

  /**
   * @brief Removing the copy constructor and assignment operator
   * so that drones cannot be copied.
   */
  Drone(const Drone& drone) = delete;
  Drone& operator=(const Drone& drone) = delete;

  /**
   * @brief Gets the drone's pickedUp boolean
   * @return boolean representing if the drone has picked up something
   */
  bool GetPickedUp() { return this->pickedUp; }

  /**
   * @brief Set the drone's pickedUp boolean
   * @param status A boolean status for the drone's pickedUp
   */
  void SetPickedUp(bool status) { this->pickedUp = status; }

  /**
   * @brief Set the drone's isFixer boolean
   * @param status A boolean status for the drone's isFixer
   */
  void SetIsFixer(bool status) { isFixer = status; }

  /**
   * @brief Gets the drone's isFixer boolean
   * @return A boolean representing if the drone is a fixer drone
   */
  bool GetIsFixer() { return isFixer; }

  /**
   * @brief Set the drone's nearestEntity pointer
   * @param entity A new entity to be the nearestEntity
   */
  void SetNearestEntity(IEntity* entity) { nearestEntity = entity; }

  /**
   * @brief Get the drone's nearestEntity pointer
   * @return IEntity pointer from drone's nearestEntity
   */
  IEntity* GetNearestEntity() { return nearestEntity; }

  /**
   * @brief Set the drone's toRobot strategy
   * @param strat A new IStrategy
   */
  void SetToRobot(IStrategy* strat) { toRobot = strat; }

  /**
   * @brief Get the drone's toRobot strategy
   * @return The drone's toRobot IStrategy
   */
  IStrategy* GetToRobot() { return toRobot; }

  /**
   * @brief Set the drone's toFinalDestination strategy
   * @param strat A new IStrategy
   */
  void SetToFinalDestination(IStrategy* strat) { toFinalDestination = strat; }

  /**
   * @brief Get the drone's toFinalDestination strategy
   * @return The drone's toFinalDestination IStrategy
   */
  IStrategy* GetToFinalDestination() { return toFinalDestination; }

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  IEntity* nearestEntity = nullptr;
  IStrategy* toRobot = nullptr;
  IStrategy* toFinalDestination = nullptr;
  std::string color = "None";  // None means default color
  float jumpHeight = 0;
  bool goUp = true;  // jump helper
  Vector3 destination;
  Vector3 finalDestination;
  float speed;
  float originalSpeed;
  bool available;
  bool pickedUp;
  Weather* weather;
  bool isFixer = false;
};

#endif
