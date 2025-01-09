#include <cstring>
#include <stdio.h>
#include <stdlib.h>

void allocerr() {
  printf("Alloc error.\n");
  abort();
}

typedef struct GameBoard {
  size_t m_size;
  char *p_value;
} game_board_t;

typedef struct Vector {
  size_t m_length;
  size_t m_capacity;
  size_t *p_value;
} vec_t;

void init_vec(vec_t *vec, size_t capacity) {
  vec->p_value = (size_t *)malloc(capacity * sizeof *vec->p_value);
  if (vec->p_value == NULL)
    allocerr();

  vec->m_capacity = capacity;
  vec->m_length = 0;
}

void extend_vec(vec_t *vec) {
  size_t *ptr = (size_t *)realloc(vec->p_value,
                                  vec->m_capacity * 2 * sizeof *vec->p_value);
  if (ptr == NULL)
    allocerr();

  vec->p_value = ptr;
  vec->m_capacity = 2 * vec->m_capacity;
}

void push_vec(vec_t *vec, size_t value) {
  if (vec->m_capacity <= vec->m_length)
    extend_vec(vec);

  vec->p_value[vec->m_length] = value;
  vec->m_length += 1;
}

typedef struct DoubleLinkedListNode {
  size_t m_value;
  struct DoubleLinkedListNode *p_previous;
  struct DoubleLinkedListNode *p_next;
} linked_list_node_t;

typedef struct {
  linked_list_node_t *p_start;
  linked_list_node_t *p_end;
  size_t m_length;
} linked_list_t;

linked_list_t create_linked_list() {
  linked_list_t list;
  list.p_start = NULL;
  list.p_end = NULL;
  list.m_length = 0;

  return list;
}

void linked_list_push(linked_list_t *list, size_t value) {
  linked_list_node_t *previous = list->p_end;

  linked_list_node_t *node =
      (linked_list_node_t *)malloc(sizeof(linked_list_node_t));
  if (!node)
    allocerr();

  node->m_value = value;
  node->p_previous = previous;
  node->p_next = NULL;

  if (previous) {
    previous->p_next = node;
  }

  list->m_length += 1;
  list->p_end = node;
  if (list->p_start == NULL)
    list->p_start = node;
}

void linked_list_pop(linked_list_t *list) {
  linked_list_node_t *del = list->p_end;
  if (del == NULL)
    return;

  linked_list_node_t *last = del->p_previous;
  if (last) {
    last->p_next = NULL;
  } else {
    list->p_start = NULL;
  }

  list->p_end = last;
  list->m_length -= 1;
  free(del);
}

void linked_list_delete_node(linked_list_node_t *node) {
  if (node->p_next != NULL)
    linked_list_delete_node(node->p_next);
  // Technically not thread safe, as p_previous of next node isn't NULL, but
  // we'll ignore that
  free(node);
}

void linked_list_delete(linked_list_t *list) {
  linked_list_node_t *node = list->p_start;
  if (node)
    linked_list_delete_node(node);
}

int init_game_size(game_board_t *game, size_t size) {
  game->p_value = (char *)calloc(size * size, sizeof game->p_value);
  if (game->p_value == NULL)
    return -1;

  game->m_size = size;

  return 0;
}

int load_game(game_board_t *game, vec_t *white) {
  size_t size;

  printf("Velikost hraci plochy:\n");

  if (scanf(" %lu", &size) != 1 || size < 3 || size > 26)
    return 1;

  if (init_game_size(game, size) != 0)
    return -1;

  printf("Pozice kamenu:\n");

  while (true) {
    int scan = -1;
    int scan2 = -1;
    char color = 0;
    char col = 0;
    int row = 0;

    int result = scanf(" %c %n %c %d%n", &color, &scan, &col, &row, &scan2);
    if (result == -1 && (scan == -1 || scan2 != -1))
      break;
    if (result == -1)
      return 1;

    if (color != 'w' && color != 'W' && color != 'b' && color != 'B')
      return 1;

    col -= 'a';
    row -= 1;

    if (col < 0 || col >= (char)size || row < 0 || row >= (int)size)
      return 1;

    bool isFieldBlack = (row + col) % 2 == 0;
    if (!isFieldBlack)
      return 1;

    size_t index = (row * size) + col;
    if (game->p_value[index] != 0)
      return 1;

    game->p_value[index] = color;
    if (color == 'w' || color == 'W')
      push_vec(white, index);
  }

  return 0;
}

void print_path(linked_list_t *path, game_board_t *game) {
  size_t i = 0;
  linked_list_node_t *node = path->p_start;
  while (node != NULL) {
    if (i != 0)
      printf(" -> ");

    size_t index = node->m_value;
    printf("%c%d", 'a' + (char)(index % game->m_size),
           1 + (int)(index / game->m_size));
    i++;
    node = node->p_next;
  }
}

ssize_t add_index(size_t index, ssize_t x, ssize_t y, game_board_t *game) {
  ssize_t new_x = (ssize_t)(index % game->m_size) + x;
  ssize_t new_y = (ssize_t)(index / game->m_size) + y;

  if (new_x < 0 || new_y < 0 || new_x >= (ssize_t)game->m_size ||
      new_y >= (ssize_t)game->m_size)
    return -1;

  return (new_y * (ssize_t)game->m_size) + new_x;
}

int opposite_direction(int direction) {
  switch (direction) {
  case (1):
    return 4;
  case (2):
    return 3;
  case (3):
    return 2;
  case (4):
    return 1;
  default:
    return 0;
  }
}

long long find_moves(game_board_t *game, size_t index, linked_list_t *visited,
                     bool *taken, int from_direction);

long long find_direction(game_board_t *game, size_t index,
                         linked_list_t *visited, bool *taken, int direction,
                         int from_direction) {
  if (opposite_direction(from_direction) == direction)
    return 0;

  ssize_t add_x = direction == 1 || direction == 3 ? 1 : (ssize_t)-1;
  ssize_t add_y = direction == 1 || direction == 2 ? 1 : (ssize_t)-1;

  int total = 0;

  ssize_t next = 0;
  ssize_t i = 1;
  while ((next = add_index(index, add_x * i, add_y * i, game)) != -1) {
    // Rocks can move only one
    if (taken == NULL && i > 1)
      break;

    if (game->p_value[next] == 0 || (taken && taken[next]) ||
        (visited->p_start != NULL &&
         visited->p_start->m_value == (size_t)next)) {
      // Length <= 0
      if (visited->p_start == visited->p_end) {
        total += 1;
        linked_list_push(visited, next);
        print_path(visited, game);
        puts("");
        linked_list_pop(visited);
      } else if (taken == NULL)
        break;

    } else if ((taken == NULL || taken[next] == false) &&
               (game->p_value[next] == 'b' || game->p_value[next] == 'B')) {

      bool *next_taken = NULL;
      if (taken != NULL) {
        next_taken = (bool *)malloc(game->m_size * game->m_size * sizeof(bool));
        if (next_taken == NULL)
          allocerr();
        memcpy(next_taken, taken, game->m_size * game->m_size * sizeof(bool));
        next_taken[next] = true;
      }

      ssize_t target = -1;
      i += 1;
      while ((target = add_index(index, add_x * i, add_y * i, game)) != -1 &&
             (game->p_value[target] == 0 ||
              (taken != NULL && taken[target] == true) ||
              (visited->p_start != NULL &&
               visited->p_start->m_value == (size_t)target))) {
        total += 1;
        linked_list_push(visited, target);
        print_path(visited, game);
        printf(" +%lu\n", visited->m_length - 1);

        total += find_moves(game, target, visited, next_taken, direction);

        linked_list_pop(visited);

        if (taken == NULL)
          break;
        i += 1;
      }

      free(next_taken);
      break;

    } else {
      break;
    }

    i++;
  }

  return total;
}

long long find_moves(game_board_t *game, size_t index, linked_list_t *visited,
                     bool *taken, int from_direction) {
  int total = 0;
  total += find_direction(game, index, visited, taken, 1, from_direction);
  total += find_direction(game, index, visited, taken, 2, from_direction);
  if (taken != NULL) {
    total += find_direction(game, index, visited, taken, 3, from_direction);
    total += find_direction(game, index, visited, taken, 4, from_direction);
  }

  return total;
}

int main() {
  int exitno = 0;
  game_board_t game;
  vec_t white;

  int result = 0;
  long long moves = 0;
  linked_list_t visited = create_linked_list();

  init_vec(&white, 16);

  result = load_game(&game, &white);

  if (result == 1) {
    printf("Nespravny vstup.\n");
    exitno = -2;
    goto exit;
  }

  for (size_t i = 0; i < white.m_length; i++) {
    linked_list_push(&visited, white.p_value[i]);

    if (game.p_value[white.p_value[i]] == 'w') {
      moves += find_moves(&game, white.p_value[i], &visited, NULL, 0);
    } else if (game.p_value[white.p_value[i]] == 'W') {
      bool *taken_pieces =
          (bool *)calloc(game.m_size * game.m_size, sizeof(bool));
      if (taken_pieces == NULL)
        allocerr();
      moves += find_moves(&game, white.p_value[i], &visited, taken_pieces, 0);
      free(taken_pieces);
    }

    linked_list_pop(&visited);
  }

  printf("Celkem ruznych tahu: %lld\n", moves);

exit:
  free(game.p_value);
  free(white.p_value);
  linked_list_delete(&visited);
  return exitno;
}
