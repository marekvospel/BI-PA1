#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *p_value;
  size_t m_capacity;
  size_t m_length;
} vec_t;

int init_vec(vec_t *vec, size_t exponent) {
  size_t cap = 1 << exponent;
  vec->p_value = (char *)malloc((cap) * sizeof(*vec->p_value));
  if (vec->p_value == NULL)
    return -1;
  vec->m_capacity = cap;
  vec->m_length = 0;

  return 0;
}

int extend_vec(vec_t *vec, size_t capacity) {
  char *newptr =
      (char *)realloc(vec->p_value, capacity * sizeof(*vec->p_value));
  if (newptr == NULL) {
    return -1;
  }
  vec->m_capacity = capacity;
  vec->p_value = newptr;

  return 0;
}

int push_vec(vec_t *vec, char value) {
  if (vec->m_length + 1 >= vec->m_capacity &&
      extend_vec(vec, vec->m_capacity << 1) != 0) {
    return -1;
  }
  vec->p_value[vec->m_length] = value;
  vec->m_length += 1;

  return 0;
}

int parse_input(vec_t *minefield, size_t *height, size_t *width) {
  *width = 0;
  *height = 0;

  bool firstline = true;
  size_t x_index = 0;
  while (true) {
    char load;
    if (scanf("%c", &load) != 1) {
      if (x_index != 0 && x_index != *width) {
        return -1;
      }
      if (x_index != 0) {
        *height += 1;
      }
      break;
    }

    if (load == '.' || load == '*') {
      x_index += 1;

      if (push_vec(minefield, load) != 0) {
        return 69;
      }
      if (firstline) {
        *width += 1;
      }
      continue;
    }

    if (load == '\n') {
      if (x_index != *width) {
        return -1;
      }
      x_index = 0;
      firstline = false;
      *height += 1;

      continue;
    }

    return -1;
  }

  if (*width == 0 || *height == 0) {
    return -1;
  }

  return 0;
}

int main() {
  printf("Zadejte hraci plochu:\n");
  size_t height;
  size_t width;

  vec_t vec;
  if (init_vec(&vec, 1) != 0) {
    return -1;
  }
  int res = parse_input(&vec, &height, &width);
  if (res == 69) {
    printf("Allocation error.\n");
    free(vec.p_value);
    return -1;
  }
  if (res != 0) {
    printf("Nespravny vstup.\n");
    free(vec.p_value);
    return -1;
  }

  printf("Vyplnena hraci plocha:\n");

  for (size_t row = 0; row < height; row++) {
    for (size_t col = 0; col < width; col++) {
      size_t index = (row * width) + col;
      if (vec.p_value[index] == '*') {
        printf("*");
      } else {

        // TODO: count
        char count = '0';
        int restrain = 0;

        if (index % width == 0) {
          // Left
          restrain |= 1 << 0;
        }
        if (index % width == width - 1) {
          // Right
          restrain |= 1 << 1;
        }
        if (index < width) {
          // Top
          restrain |= 1 << 2;
        }
        if (index > width * (height - 1)) {
          // Bottom
          restrain |= 1 << 3;
        }

        if ((restrain & (1 << 0)) == 0 && vec.p_value[index - 1] == '*') {
          count += 1;
        }
        if ((restrain & (1 << 1)) == 0 && vec.p_value[index + 1] == '*') {
          count += 1;
        }
        if ((restrain & (1 << 2)) == 0 && vec.p_value[index - width] == '*') {
          count += 1;
        }
        if ((restrain & (1 << 3)) == 0 && vec.p_value[index + width] == '*') {
          count += 1;
        }
        if ((restrain & ((1 << 0) | (1 << 2))) == 0 &&
            vec.p_value[index - width - 1] == '*') {
          count += 1;
        }
        if ((restrain & ((1 << 1) | (1 << 2))) == 0 &&
            vec.p_value[index - width + 1] == '*') {
          count += 1;
        }
        if ((restrain & ((1 << 0) | (1 << 3))) == 0 &&
            vec.p_value[index + width - 1] == '*') {
          count += 1;
        }
        if ((restrain & ((1 << 1) | (1 << 3))) == 0 &&
            vec.p_value[index + width + 1] == '*') {
          count += 1;
        }

        if (count == '0')
          printf(".");
        else
          printf("%c", count);
      }
    }
    puts("");
  }

  free(vec.p_value);
  return 0;
}
