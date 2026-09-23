#ifndef OVR_H
#define OVR_H

#define WEATHER_SNOWY "snowy"
#define WEATHER_SUNNY "sunny"
#define WEATHER_RAINY "rainy"
#define WEATHER_CLOUDY "cloudy"

struct Features {
  float temperature; // in celsius
  float humidity;    // in percentage
  float wind_speed;  // in km/h
  float cloudiness;  // in percentage
};

// Classifies a sample by using One vs Rest classification
char *ovr_classify(const struct Features);
#endif
