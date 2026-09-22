#ifndef OVR_H
#define OVR_H

typedef enum {
  WEATHER_SNOWY = 0,
  WEATHER_SUNNY,
  WEATHER_RAINY,
  WEATHER_CLOUDY
} weather_class;

struct Features {
  float temperature; // in celsius
  float humidity;    // in percentage
  float wind_speed;  // in km/h
  float cloudiness;  // in percentage
};

weather_class ovr_classify(const struct Features);

#endif
