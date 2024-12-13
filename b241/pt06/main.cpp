#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Crossword {
  char m_character;
  bool m_removed;
} crossword_t;

typedef struct Vector {
  crossword_t *p_value;
  size_t m_length;
  size_t m_capacity;
} vec_t;

typedef struct String {
  char *p_value;
  size_t m_length;
  size_t m_capacity;
} string_t;

typedef struct Puzzle {
  vec_t *p_vec;
  size_t m_width;
  size_t m_height;
} puzzle_t;

typedef struct Command {
  // 0 = cross out
  // 1 = print occurances
  // 2 = print result
  int m_cmd;
  string_t *p_str;
} command_t;

typedef struct LongPoint {
  long long x;
  long long y;
} long_point_t;

typedef enum Direction {
  RIGHT = 0,
  BOTTOM = 1,
  LEFT = 2,
  TOP = 3,
  RBOTTOM = 4,
  LBOTTOM = 5,
  LTOP = 6,
  RTOP = 7,
} direction_t;

int init_vec_cap(vec_t *vec, size_t cap) {
  crossword_t *ptr = (crossword_t *)malloc(sizeof(*vec->p_value) * cap);
  if (ptr == NULL) {
    return -1;
  }
  vec->m_length = 0;
  vec->m_capacity = cap;
  vec->p_value = ptr;

  return 0;
}

int set_vec_cap(vec_t *vec, size_t cap) {
  if (cap < vec->m_length)
    return -2;
  crossword_t *ptr =
      (crossword_t *)realloc(vec->p_value, sizeof(*vec->p_value) * cap);

  if (ptr == NULL) {
    return -1;
  }

  vec->p_value = ptr;
  vec->m_capacity = cap;

  return 0;
}

int push_vec(vec_t *vec, crossword_t val) {
  if (vec->m_capacity <= vec->m_length &&
      set_vec_cap(vec, vec->m_capacity * 2) != 0) {
    return -1;
  }

  vec->p_value[vec->m_length] = val;
  vec->m_length += 1;
  return 0;
}

int init_str_cap(string_t *vec, size_t cap) {
  char *ptr = (char *)malloc(sizeof(*vec->p_value) * cap);
  if (ptr == NULL) {
    return -1;
  }
  vec->m_length = 0;
  vec->m_capacity = cap;
  vec->p_value = ptr;

  assert(cap > 0);
  *vec->p_value = '\0';

  return 0;
}

int set_str_cap(string_t *vec, size_t cap) {
  if (cap < vec->m_length)
    return -2;
  char *ptr = (char *)realloc(vec->p_value, sizeof(*vec->p_value) * cap);

  if (ptr == NULL) {
    return -1;
  }

  vec->p_value = ptr;
  vec->m_capacity = cap;
  assert(cap > 0);

  return 0;
}

int append_str(string_t *vec, char val) {
  if (vec->m_capacity - 1 <= vec->m_length &&
      set_str_cap(vec, vec->m_capacity * 2) != 0) {
    return -1;
  }

  vec->p_value[vec->m_length] = val;
  vec->m_length += 1;
  vec->p_value[vec->m_length] = '\0';
  return 0;
}

void clear_str(string_t *vec) {
  *vec->p_value = '\0';
  vec->m_length = 0;
}

int load_crossword(puzzle_t *puzzle) {
  size_t index = 0;
  puzzle->m_width = 0;
  puzzle->m_height = 0;

  while (true) {
    char c = getchar();

    if (c == EOF)
      return 1;

    if (c == '\n') {
      if (index == 0) {
        break;
      }
      if (puzzle->m_height != 0 && index != puzzle->m_width) {
        return 1;
      }
      if (puzzle->m_height == 0) {
        puzzle->m_width = index;
      }

      puzzle->m_height += 1;
      index = 0;
      continue;
    } else {
      if ((c < 'a' || c > 'z') && c != '.') {
        return 1;
      }

      crossword_t character;
      character.m_removed = c == '.';
      character.m_character = c;
      push_vec(puzzle->p_vec, character);
    }

    index += 1;
  }

  if (puzzle->m_height == 0) {
    return 1;
  }

  return 0;
}

int scan_command(command_t *cmd) {
  char command_char = -1;

  int result = scanf("%c", &command_char);
  if (result == EOF) {
    return 2;
  }
  if (result != 1) {
    return 1;
  }

  if (command_char == '?') {
    cmd->m_cmd = 2;
    char c = getchar();
    if (c != EOF && c != '\n')
      return 1;
  } else if (command_char == '#' || command_char == '-') {
    cmd->m_cmd = command_char == '#';

    // Get rid of leading whitespace
    scanf(" ");

    while (true) {
      char c = getchar();

      if (c == EOF) {
        return 1;
      }

      if (c == '.' || (c >= 'a' && c <= 'z')) {
        if (append_str(cmd->p_str, c) != 0)
          return -1;
      } else if (c == '\n') {
        break;
      } else {
        return 1;
      }
    }

    if (cmd->p_str->m_length < 2)
      return 1;

  } else {
    return 1;
  }

  return 0;
}

void print_result(puzzle_t *puzzle) {

  printf("Tajenka:\n");
  long long printed = 0;
  for (size_t y = 0; y < puzzle->m_height; y++) {

    for (size_t x = 0; x < puzzle->m_width; x++) {
      crossword_t *letter = &puzzle->p_vec->p_value[y * puzzle->m_width + x];
      if (!letter->m_removed) {
        printf("%c", letter->m_character);
        if (printed == 59) {
          printed = 0;
          puts("");
        } else {
          printed += 1;
        }
      }
    }
  }
  if (printed != 0)
    puts("");
}

long long add_index(long long index, long_point_t add, puzzle_t *puzzle) {
  long_point_t point;
  point.x = (index % puzzle->m_width) + add.x;
  point.y = (index / puzzle->m_width) + add.y;

  if (point.x < 0 || point.x >= (long long)puzzle->m_width)
    return -1;
  if (point.y < 0 || point.y >= (long long)puzzle->m_height)
    return -1;

  return point.y * puzzle->m_width + point.x;
}

long long next_index(long long index, direction_t direction, long long times,
                     puzzle_t *puzzle) {
  long_point_t point;
  point.x = 0;
  point.y = 0;

  switch (direction) {
  case RIGHT:
    point.x = times;
    break;
  case BOTTOM:
    point.y = times;
    break;
  case LEFT:
    point.x = -times;
    break;
  case TOP:
    point.y = -times;
    break;
  case RBOTTOM:
    point.x = times;
    point.y = times;
    break;
  case LBOTTOM:
    point.x = -times;
    point.y = times;
    break;
  case LTOP:
    point.x = -times;
    point.y = -times;
    break;
  case RTOP:
    point.x = times;
    point.y = -times;
    break;
  }

  return add_index(index, point, puzzle);
}

int find_direction(puzzle_t *puzzle, string_t *str, bool cross_out,
                   long long start_index, direction_t direction) {
  int occurances = 0;

  long long index = start_index;
  while (next_index(index, direction, str->m_length - 1, puzzle) != -1) {
    bool valid = true;
    for (size_t i = 0; i < str->m_length; i++) {
      crossword_t *c =
          &puzzle->p_vec->p_value[next_index(index, direction, i, puzzle)];

      if (c->m_character != str->p_value[i]) {
        valid = false;
        break;
      }
    }

    if (valid) {
      occurances += 1;

      for (size_t i = 0; i < str->m_length; i++) {
        crossword_t *c =
            &puzzle->p_vec->p_value[next_index(index, direction, i, puzzle)];

        c->m_removed = cross_out || c->m_removed;
      }
    }

    index = next_index(index, direction, 1, puzzle);
  }

  return occurances;
}

int find_results(puzzle_t *puzzle, string_t *str, bool cross_out) {

  int occurances = 0;

  long long start_index = 0;
  // Left wall
  do {
    occurances += find_direction(puzzle, str, cross_out, start_index, RIGHT);
    occurances += find_direction(puzzle, str, cross_out, start_index, RTOP);
    occurances += find_direction(puzzle, str, cross_out, start_index, RBOTTOM);
  } while ((start_index = next_index(start_index, BOTTOM, 1, puzzle)) != -1);
  start_index = 0;
  // Top wall
  do {
    occurances += find_direction(puzzle, str, cross_out, start_index, BOTTOM);
    // Top left
    if (start_index != 0) {
      occurances +=
          find_direction(puzzle, str, cross_out, start_index, RBOTTOM);
    }
    // Top right
    if (start_index != (long long)puzzle->m_width - 1) {
      occurances +=
          find_direction(puzzle, str, cross_out, start_index, LBOTTOM);
    }
  } while ((start_index = next_index(start_index, RIGHT, 1, puzzle)) != -1);

  start_index = puzzle->m_width * puzzle->m_height - 1;
  // Right wall
  do {
    occurances += find_direction(puzzle, str, cross_out, start_index, LEFT);
    occurances += find_direction(puzzle, str, cross_out, start_index, LTOP);
    occurances += find_direction(puzzle, str, cross_out, start_index, LBOTTOM);
  } while ((start_index = next_index(start_index, TOP, 1, puzzle)) != -1);

  start_index = puzzle->m_width * puzzle->m_height - 1;
  // Bottom wall
  do {
    occurances += find_direction(puzzle, str, cross_out, start_index, TOP);

    // Bottom left
    if (start_index !=
        ((long long)puzzle->m_height - 1) * (long long)puzzle->m_width) {
      occurances += find_direction(puzzle, str, cross_out, start_index, RTOP);
    }
    // Bottom right
    if (start_index !=
        (long long)puzzle->m_height * (long long)puzzle->m_width - 1) {
      occurances += find_direction(puzzle, str, cross_out, start_index, LTOP);
    }
  } while ((start_index = next_index(start_index, LEFT, 1, puzzle)) != -1);

  return occurances;
}

int main() {
  int exitno = 0;

  vec_t vec;
  puzzle_t puzzle;
  puzzle.p_vec = &vec;

  string_t command_string;

  if (init_vec_cap(puzzle.p_vec, 16) != 0 ||
      init_str_cap(&command_string, 16) != 0) {
    printf("Alloc error.\n");
    return -1;
  }

  printf("Osmismerka:\n");

  int result = load_crossword(&puzzle);
  if (result == 1) {
    printf("Nespravny vstup.\n");
    exitno = -1;
    goto cleanup;
  } else if (result == -1) {
    printf("Alloc error.\n");
    exitno = -1;
    goto cleanup;
  }

  command_t cmd;
  cmd.p_str = &command_string;
  while (true) {
    clear_str(cmd.p_str);
    int result = scan_command(&cmd);

    if (result == -1) {
      printf("Alloc error.\n");
      exitno = -1;
      goto cleanup;
    } else if (result == 1) {
      printf("Nespravny vstup.\n");
      exitno = -1;
      goto cleanup;
    } else if (result == 2) {
      break;
    } else if (result == 3) {
      printf("%s: 0x\n", cmd.p_str->p_value);
      continue;
    }

    if (cmd.m_cmd == 2) {
      print_result(&puzzle);
    } else {
      int results = find_results(&puzzle, &command_string, cmd.m_cmd == 0);
      printf("%s: %dx\n", command_string.p_value, results);
    }
  }

cleanup:
  free(command_string.p_value);
  free(puzzle.p_vec->p_value);
  return exitno;
}
