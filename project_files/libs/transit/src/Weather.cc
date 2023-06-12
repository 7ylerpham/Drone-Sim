#include "Weather.h"
#include <time.h>

Weather* Weather::instance = NULL;

Weather::Weather() { RandomizeWeather(); }

Weather* Weather::GetWeather() {
  if (instance == nullptr) {
    instance = new Weather();
  }
  return instance;
}

float Weather::GetWindSpeed() { return windSpeed; }

float Weather::GetWindDirection() { return windDirection; }

std::string Weather::GetWeatherType() { return weatherType; }

void Weather::RandomizeWeather() {
//   srand(time(0));
  unsigned int seed = time(NULL);
  int weathI = rand_r(&seed) % 10;
  this->weatherType =
      weathI < 3
          ? "sunny"
          : (weathI < 6 ? "breezy" : (weathI < 9 ? "high wind" : "tornado"));
  if (weatherType == "sunny") {
    this->windSpeed = rand_r(&seed) % 10;
  } else if (weatherType == "breezy") {
    this->windSpeed = 10 + (rand_r(&seed) % 15);
  } else if (weatherType == "high wind") {
    this->windSpeed = 25 + (rand_r(&seed) % 30);
  } else {
    this->windSpeed = 100 + (rand_r(&seed) % 100);
  }

  this->windDirection = rand_r(&seed) % 360;

//   std::cout << "WEATHER TYPE IS " << this->weatherType << std::endl;
//   std::cout << "WEATHER SPEED IS " << this->windSpeed << std::endl;
//   std::cout << "WEATHER DIRECTION IS " << this->windDirection << std::endl;
  CalculateWind();
}

void Weather::SetWeather(float windSpeed, float windDirection,
                         std::string weatherType) {
  this->windSpeed = windSpeed;
  this->windDirection = windDirection;
  this->weatherType = weatherType;
  CalculateWind();
}

void Weather::CalculateWind() {
  float x = windSpeed * cos(windDirection * 3.14 / 180);
  float y = windSpeed * sin(windDirection * 3.14 / 180);
  wind = Vector3(x, y, 0);
}

Vector3 Weather::GetWind() { return wind; }
