#include "ovr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_DATASET "/home/wintape/javid/repos/cpl/task2/test.csv"
#define FEATURE_COUNT 4
#define CLASS_BUF 16
void populate_sample_info(const char *line, struct Features *sample_features,
                          char *sample_class);

int main(void) {
  FILE *fp;
  fp = fopen(TEST_DATASET, "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open the test dataset \"%s\"\n", TEST_DATASET);
    return 1;
  }
  ssize_t nread;
  char *line = NULL;
  size_t line_size = 0;
  getline(&line, &line_size, fp);
  int correct = 0;
  int mistakes = 0;
  while ((nread = getline(&line, &line_size, fp)) != -1) {
    struct Features sample_features;
    char sample_class[CLASS_BUF];
    populate_sample_info(line, &sample_features, sample_class);
    char *ovr_result = ovr_classify(sample_features);
    if (!strcmp(ovr_result, sample_class)) {
      correct++;
    } else {
      mistakes++;
    }
  }
  (void)nread;
  fclose(fp);
  if (mistakes == 0) {
    printf("Your OvR model's accuracy: 100%%\n");
  } else if (correct == 0) {
    printf("Your OvR model's accuracy: 0%%\n");
  } else {
    float accuracy = 100.0f * (float)correct / ((float)(correct + mistakes));
    printf("Your OvR model's accuracy: %f%%\n", accuracy);
  }

  // print out accuracy
  return 0;
}

void populate_sample_info(const char *line, struct Features *sample_features,
                          char *sample_class) {
  float values[FEATURE_COUNT];
  int field = 0;
  char buf[32];
  int bi = 0;

  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == ',' || line[i] == '\n') {
      buf[bi] = '\0';
      if (field < FEATURE_COUNT) {
        values[field++] = atof(buf);
      } else if (!strcmp(buf, WEATHER_SNOWY) || !strcmp(buf, WEATHER_SUNNY) ||
                 !strcmp(buf, WEATHER_RAINY) || !strcmp(buf, WEATHER_CLOUDY)) {
        strcpy(sample_class, buf);
      }
      bi = 0;
    } else if (bi < (int)sizeof(buf) - 1) {
      buf[bi++] = line[i];
    }
  }

  sample_features->temperature = values[0];
  sample_features->humidity = values[1];
  sample_features->wind_speed = values[2];
  sample_features->cloudiness = values[3];
}
