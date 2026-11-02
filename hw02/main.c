#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// w faps

#define ARG_FLOAT_ELEMENTS 6
#define ARG_FLOAT_H 0
#define ARG_FLOAT_W 1
#define ARG_FLOAT_CELL_SIZE 2
#define ARG_FLOAT_X 3
#define ARG_FLOAT_Y 4
#define ARG_FLOAT_ALPHA 5

int raycast(double angle_deg, double distance_m, double cell_size_m,
            int ***out_cells, int *out_count);

int main(int argc, char *argv[]) {
  if (argc != ARG_FLOAT_ELEMENTS + 1) {
    printf("Invalid command-line arguments");
    return 103;
  }
  float arg_floats[ARG_FLOAT_ELEMENTS]; // indexing follows the order of
                                        // arguments from courseware
  for (int i = 0; i < ARG_FLOAT_ELEMENTS; i++) {
    arg_floats[i] = strtof(argv[i + 1], NULL);
    if (arg_floats[i] == HUGE_VALF || arg_floats[i] == 0.0F) {
      printf("Invalid command-line arguments");
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
      occ_grid[i][j] = 0.5f;
    }
  }

  while (1) {
    float angle, distance;
    int ret = scanf("%f %f", &angle, &distance);

    if (ret != 2) {
      fprintf(stderr, "Invalid sensor data");
      return 102;
    }
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
