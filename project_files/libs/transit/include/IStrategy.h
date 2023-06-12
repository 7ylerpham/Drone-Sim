#ifndef I_STRATEGY_H_
#define I_STRATEGY_H_

#include "IEntity.h"

/**
 * @brief Strategy interface
 */
class IStrategy {
 public:
  /**
   * @brief Move toward next position
   * @param entity Entity to move
   * @param dt Delta Time
   */
  virtual void Move(IEntity* entity, double dt) = 0;

  /**
   * @brief Check if the trip is completed
   * @return True if complete, false if not complete
   */
  virtual bool IsCompleted() = 0;

  /**
   * @brief Get the total distance of a strategy
   * @return A float representing the total distance
   */
  float GetTotalDistance(){ return totalDistance; }

 protected:
  float totalDistance = 0;
};

#endif
