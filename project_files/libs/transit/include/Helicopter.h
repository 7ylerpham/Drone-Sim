#ifndef Helicopter_H_
#define Helicopter_H_

#include "IEntity.h"
#include "IStrategy.h"
#include "Weather.h"

/**
 * @class Helicopter
 * @brief Represents a helicopter in a physical system. 
 * Helicopters move using euler integration based on a 
 * specified velocity and direction.
 */
class Helicopter : public IEntity {
 public:
  /**
   * @brief Helicopter are created with a name
   * @param obj JSON object containing the helicopter's information
   */
  Helicopter(JsonObject& obj);

  /**
   * @brief Destroy the Helicopter object
   */
  ~Helicopter();
  /**
   * @brief Gets the speed of the helicopter
   * @return The speed of the helicopter
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the helicopter
   * @return The position of the helicopter
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the helicopter
   * @return The direction of the helicopter
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the helicopter
   * @return The destination of the helicopter
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the helicopter
   * @return The details information of the helicopter
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Updates the helicopter's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the helicopter
   * @param pos_ The new position of the helicopter
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the helicopter
   * @param dir_ The new direction of the helicopter
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the helicopter
   * @param des_ The new destination of the helicopter
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Creates a random new destination for the helicopter
   */
  void CreateNewDestination();

  /**
   * @brief Rotates the helicopter
   * @param angle The angle by which the helicopter should be rotated
   */
  void Rotate(double angle);

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  float speed;
  float originalSpeed;
  IStrategy* toDestination = nullptr;
  Weather* weather;
};

#endif
