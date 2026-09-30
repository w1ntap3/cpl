#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MEASUREMENT_FORMULA_EXIST(prob)                                        \
  (0.8f * (float)prob) / (0.8f * (float)prob + 0.2 * (1 - (float)prob))

#define EMPTY_MEASUREMENT_FORMULA(prob)                                        \
  (0.2f * (float)prob) / (0.2f * (float)prob + 0.8 * (1 - (float)prob))

typedef struct {
  float prob;
  float next_prob;
} Probability;

static inline uint8_t calculate_next_prob(const float prob, float *next_prob,
                                          const uint8_t measurement);

int main(int argc, char *argv[]) {
  Probability bayesian_prob = {.prob = 0.5f, .next_prob = 0.0f};

  for (int a = 1; a < argc; a++) {
    char *endptr;
    long value = strtol(argv[a], &endptr, 10);

    if (*argv[a] == '\0' || *endptr != '\0' || (value != 0 && value != 1)) {
      fprintf(stderr, "Error: Invalid input.\n");
      return 1;
    }

    uint8_t meas = (uint8_t)value;

    uint8_t err =
        calculate_next_prob(bayesian_prob.prob, &bayesian_prob.next_prob, meas);
    if (err != 0) {
      return 1;
    }
    bayesian_prob.prob = bayesian_prob.next_prob;
    printf("%.5f", bayesian_prob.prob);
    if (a != argc - 1) {
      putchar(' ');
    }
  }
  putchar('\n');
  return 0;
}

static inline uint8_t calculate_next_prob(const float prob, float *next_prob,
                                          const uint8_t measurement) {
  switch (measurement) {
  case 0:
    *next_prob = EMPTY_MEASUREMENT_FORMULA(prob);
    break;
  case 1:
    *next_prob = MEASUREMENT_FORMULA_EXIST(prob);
    break;
  default:
    return 1;
  }
  return 0;
}
