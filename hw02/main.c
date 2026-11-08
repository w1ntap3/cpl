#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ARG_FLOAT_ELEMENTS 6
#define ARG_FLOAT_H 0
#define ARG_FLOAT_W 1
#define ARG_FLOAT_CELL_SIZE 2
#define ARG_FLOAT_X 3
#define ARG_FLOAT_Y 4
#define ARG_FLOAT_ALPHA 5

#define BAYESIAN_OCCUPIED(p)                                                    \
  (0.8f * (p)) / (0.8f * (p) + 0.2f * (1.0f - (p)))

#define BAYESIAN_FREE(p)                                                        \
  (0.2f * (p)) / (0.2f * (p) + 0.8f * (1.0f - (p)))

#define PRIOR_PROB 0.5f
#define PROB_TOLERANCE 1e-6

int raycast(double angle_deg, double distance_m, double cell_size_m,
            int ***out_cells, int *out_count);

int main(int argc, char *argv[]) {
  if (argc != ARG_FLOAT_ELEMENTS + 1) {
    fprintf(stderr, "ERROR: invalid arguments\n");
    return 103;
  }

  float arg_floats[ARG_FLOAT_ELEMENTS]; // indexing follows the order of
                                        // arguments from courseware
  for (int i = 0; i < ARG_FLOAT_ELEMENTS; i++) {
    char *endptr;
    arg_floats[i] = strtof(argv[i + 1], &endptr);
    if (endptr == argv[i + 1] || *endptr != '\0' ||
        arg_floats[i] == HUGE_VALF) {
      fprintf(stderr, "ERROR: invalid arguments\n");
      return 103;
    }
  }

  int n_height =
      (int)ceilf(arg_floats[ARG_FLOAT_H] / arg_floats[ARG_FLOAT_CELL_SIZE]);
  int n_width =
      (int)ceilf(arg_floats[ARG_FLOAT_W] / arg_floats[ARG_FLOAT_CELL_SIZE]);

  float occ_grid[n_height][n_width];
  for (int i = 0; i < n_height; i++) {
    for (int j = 0; j < n_width; j++) {
      occ_grid[i][j] = PRIOR_PROB;
    }
  }

  int robot_col =
      (int)(arg_floats[ARG_FLOAT_X] / arg_floats[ARG_FLOAT_CELL_SIZE]);
  int robot_row =
      n_height - 1 -
      (int)(arg_floats[ARG_FLOAT_Y] / arg_floats[ARG_FLOAT_CELL_SIZE]);

  while (1) {
    float s_angle, s_distance;
    int ret = scanf("%f %f", &s_angle, &s_distance);
    if (ret == EOF) {
      break;
    }
    if (ret != 2) {
      fprintf(stderr, "ERROR: invalid sensor data\n");
      return 102;
    }

    float m_angle = arg_floats[ARG_FLOAT_ALPHA] + s_angle;
    int **cells = NULL;
    int count = 0;
    if (!raycast(m_angle, s_distance, arg_floats[ARG_FLOAT_CELL_SIZE], &cells,
                 &count)) {
      fprintf(stderr, "ERROR: raycast failure\n");
      return 101;
    }

    for (int i = 0; i < count; i++) {
      int col = robot_col + cells[i][0];
      int row = robot_row - cells[i][1];
      if (row < 0 || row >= n_height || col < 0 || col >= n_width) {
        continue;
      }
      occ_grid[row][col] = (i == count - 1)
                               ? BAYESIAN_OCCUPIED(occ_grid[row][col])
                               : BAYESIAN_FREE(occ_grid[row][col]);
    }

    for (int i = 0; i < count; i++) {
      free(cells[i]);
    }
    free(cells);
  }

  for (int i = 0; i < n_height; i++) {
    for (int j = 0; j < n_width; j++) {
      if (i == robot_row && j == robot_col) {
        putchar('*');
      } else if (fabs(occ_grid[i][j] - PRIOR_PROB) < PROB_TOLERANCE) {
        putchar('-');
      } else if (occ_grid[i][j] > PRIOR_PROB) {
        putchar('#');
      } else {
        putchar(' ');
      }
    }
    putchar('\n');
  }
  return 0;
}

int raycast(double angle_deg, double distance_m, double cell_size_m,
            int ***out_cells, int *out_count) {
  if (!out_cells || !out_count || distance_m < cell_size_m) {
    return 0;
  }

  double angle_rad = angle_deg * 3.14159265358979323846 / 180.0;
  double end_x = distance_m * cos(angle_rad);
  double end_y = distance_m * sin(angle_rad);
  int x0 = 0, y0 = 0;
  int x1 = (int)(end_x / cell_size_m);
  int y1 = (int)(end_y / cell_size_m);
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  int max_cells = dx + dy + 1;
  int **cells = malloc(max_cells * sizeof(int *));
  if (!cells) {
    return 0;
  }

  int count = 0;

  while (1) {
    cells[count] = malloc(2 * sizeof(int));
    if (!cells[count]) {
      // Clean up previously allocated memory
      for (int i = 0; i < count; i++) {
        free(cells[i]);
        cells[i] = NULL;
      }
      free(cells);
      cells = NULL;
      return 0;
    }
    cells[count][0] = x0;
    cells[count][1] = y0;
    count++;

    if (x0 == x1 && y0 == y1) {
      break;
    }

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }

  *out_cells = cells;
  *out_count = count;
  return 1;
}
