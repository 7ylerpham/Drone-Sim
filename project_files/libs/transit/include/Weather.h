

#ifndef WEATHER_H_
#define WEATHER_H_
#include <cmath>
#include <string>
#include "math/vector3.h"

/**
 * @class Weather
 * @brief Weather Singleton that contains singular instance of weather
 **/
class Weather {
 protected:
  /**
   * @brief Construct a new Weather object
   */
  Weather();

  static Weather* instance;
  float windSpeed;
  float windDirection;
  std::string weatherType;
  Vector3 wind;

 public:
  /**
   * @brief copy constructor is restricted
   */
  Weather(Weather& other) = delete;

  /**
   * @brief Desctructor for the Weather instance
   */
  ~Weather() { delete instance; }

  /**
   * @brief assignment operator is restricted
   */
  void operator=(const Weather&) = delete;
  /**
   * @brief allows access to instance of Weather singleton object.
   * @return The Weather singleton object
   */
  static Weather* GetWeather(/*const std::string& value*/);

  /**
   * @brief getter method for the wind speed
   * @return The wind speed in weather instance
   */
  float GetWindSpeed();

  /**
   * @brief gets the direction of the wind in Weather
   * @return the direction of the wind
   */
  float GetWindDirection();

  /**
   * @brief gets the type of weather currently in Weather
   * @return the type of weather currently in the Weather instance
   */
  std::string GetWeatherType();

  /**
   * @brief randomizes the values for the wind speed, direction, and weather
   * type in Weather
   */
  void RandomizeWeather();

  /**
   * @brief manually sets the values for the wind speed, direction, and weather
   * type in Weather
   */
  void SetWeather(float windSpeed, float windDirection,
                  std::string weatherType);

  /**
   * @brief calculates the wind vector based upon the new wind speed and
   * direction
   */
  void CalculateWind();

  /**
   * @brief gets the wind in a vector form
   * @return the wind vector with it's direction and magnitude
   */
  Vector3 GetWind();
};

#endif
