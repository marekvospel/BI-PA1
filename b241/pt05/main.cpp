#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  long long *p_value;
  size_t m_capacity;
  size_t m_length;
} vec_t;

int init_vec(vec_t *vec, size_t exponent) {
  size_t cap = 1 << exponent;
  vec->p_value = (long long *)malloc((cap) * sizeof(*vec->p_value));
  if (vec->p_value == NULL)
    return -1;
  vec->m_capacity = cap;
  vec->m_length = 0;

  return 0;
}

int extend_vec(vec_t *vec, size_t capacity) {
  long long *newptr =
      (long long *)realloc(vec->p_value, capacity * sizeof(*vec->p_value));
  if (newptr == NULL) {
    return -1;
  }
  vec->m_capacity = capacity;
  vec->p_value = newptr;

  return 0;
}

int push_vec(vec_t *vec, long long value) {
  if (vec->m_length + 1 >= vec->m_capacity &&
      extend_vec(vec, vec->m_capacity << 1) != 0) {
    return -1;
  }
  vec->p_value[vec->m_length] = value;
  vec->m_length += 1;

  return 0;
}

typedef struct {
  char m_op;
  long long m_start;
  long long m_end;
} input_t;

int parse_input(input_t *input, size_t max_index) {
  input->m_op = -1;
  int scanner = -1;

  int result = scanf(" %c %lld%n", &input->m_op, &input->m_start, &scanner);
  if (result == EOF && input->m_op == -1)
    return -1;

  if (scanner == -1) {
    return 1;
  }

  if (input->m_op == '+')
    return input->m_start <= 0;

  if (input->m_op == '?') {
    if (scanf(" %lld", &input->m_end) != 1)
      return 1;

    return input->m_start > input->m_end || input->m_start < 0 ||
           (size_t)input->m_end > max_index;
  }

  return 1;
}

long long *pmax(long long *a, long long *b) { return *a >= *b ? a : b; }
long long *pmin(long long *a, long long *b) { return *a <= *b ? a : b; }

typedef struct {
  bool m_present;
  long long *p_lowest;
  long long *p_highest;
  long long *p_profit_start;
  long long *p_profit_end;
  long long *p_loss_start;
  long long *p_loss_end;
} segment_tree_node_t;

typedef struct {
  segment_tree_node_t *p_value;
  vec_t *p_vec;
  size_t m_capacity;
} segment_tree_t;

int init_segtree(segment_tree_t *tree, vec_t *vec) {
  tree->p_value = (segment_tree_node_t *)malloc(0 * sizeof(*tree->p_value));
  if (tree->p_value == 0) {
    return -1;
  }

  tree->p_vec = vec;
  tree->m_capacity = 0;
  return 0;
}

void copy_segtree_node(segment_tree_node_t *node, segment_tree_node_t *src) {
  node->m_present = src->m_present;
  node->p_lowest = src->p_lowest;
  node->p_highest = src->p_highest;
  node->p_profit_start = src->p_profit_start;
  node->p_profit_end = src->p_profit_end;
  node->p_loss_start = src->p_loss_start;
  node->p_loss_end = src->p_loss_end;
}

void merge_segtree_nodes(segment_tree_node_t *node, segment_tree_node_t *left,
                         segment_tree_node_t *right) {
  if (left->m_present && !right->m_present) {
    copy_segtree_node(node, left);
    return;
  } else if (!left->m_present && right->m_present) {
    copy_segtree_node(node, right);
    return;
  } else if (!left->m_present && !right->m_present) {
    node->m_present = false;
    return;
  }
  node->m_present = true;
  node->p_lowest = pmin(left->p_lowest, right->p_lowest);
  node->p_highest = pmax(left->p_highest, right->p_highest);

  node->p_profit_start = left->p_profit_start;
  node->p_profit_end = left->p_profit_end;

  long long old_diff = *node->p_profit_end - *node->p_profit_start;
  long long new_diff = *right->p_profit_end - *right->p_profit_start;

  if (new_diff > old_diff ||
      (old_diff == new_diff && right->p_profit_start < node->p_profit_start)) {
    node->p_profit_start = right->p_profit_start;
    node->p_profit_end = right->p_profit_end;
  }

  old_diff = *node->p_profit_end - *node->p_profit_start;
  new_diff = *right->p_highest - *left->p_lowest;

  if (new_diff > old_diff ||
      (old_diff == new_diff && left->p_lowest < node->p_profit_start)) {
    node->p_profit_start = left->p_lowest;
    node->p_profit_end = right->p_highest;
  }

  node->p_loss_start = left->p_loss_start;
  node->p_loss_end = left->p_loss_end;

  old_diff = *node->p_loss_start - *node->p_loss_end;
  new_diff = *right->p_loss_start - *right->p_loss_end;

  if (new_diff > old_diff ||
      (old_diff == new_diff && right->p_loss_start < node->p_loss_start)) {
    node->p_loss_start = right->p_loss_start;
    node->p_loss_end = right->p_loss_end;
  }

  old_diff = *node->p_loss_start - *node->p_loss_end;
  new_diff = *left->p_highest - *right->p_lowest;

  if (new_diff > old_diff ||
      (old_diff == new_diff && left->p_highest < node->p_loss_start)) {
    node->p_loss_start = left->p_highest;
    node->p_loss_end = right->p_lowest;
  }
}

void build_segtree_node(segment_tree_t *tree, size_t node_index,
                        size_t node_start, size_t node_end) {
  segment_tree_node_t *node = &tree->p_value[node_index];
  node->p_loss_end = 0;

  if (node_start == node_end) {
    node->m_present = tree->p_vec->m_length > node_start;
    node->p_lowest = &tree->p_vec->p_value[node_start];
    node->p_highest = &tree->p_vec->p_value[node_start];
    node->p_profit_start = &tree->p_vec->p_value[node_start];
    node->p_profit_end = &tree->p_vec->p_value[node_start];
    node->p_loss_start = &tree->p_vec->p_value[node_start];
    node->p_loss_end = &tree->p_vec->p_value[node_start];

    return;
  }

  size_t node_center = (node_start + node_end) / 2;
  size_t calc_node_index = node_index + 1;
  build_segtree_node(tree, (2 * calc_node_index) - 1, node_start, node_center);
  build_segtree_node(tree, (2 * calc_node_index), node_center + 1, node_end);

  segment_tree_node_t *left = &tree->p_value[2 * calc_node_index - 1];
  segment_tree_node_t *right = &tree->p_value[2 * calc_node_index];

  merge_segtree_nodes(node, left, right);
}

void update_segtree_value(segment_tree_t *tree, size_t node_index,
                          size_t node_start, size_t node_end,
                          size_t update_index) {

  segment_tree_node_t *node = &tree->p_value[node_index];
  if (node_start == node_end) {
    node->m_present = tree->p_vec->m_length > node_start;
    node->p_lowest = &tree->p_vec->p_value[node_start];
    node->p_highest = &tree->p_vec->p_value[node_start];
    node->p_profit_start = &tree->p_vec->p_value[node_start];
    node->p_profit_end = &tree->p_vec->p_value[node_start];
    node->p_loss_start = &tree->p_vec->p_value[node_start];
    node->p_loss_end = &tree->p_vec->p_value[node_start];
    return;
  }

  size_t node_center = (node_start + node_end) / 2;
  size_t calc_node_index = node_index + 1;
  if (update_index <= node_center) {
    update_segtree_value(tree, 2 * calc_node_index - 1, node_start, node_center,
                         update_index);
  } else {
    update_segtree_value(tree, 2 * calc_node_index, node_center + 1, node_end,
                         update_index);
  }

  segment_tree_node_t *left = &tree->p_value[2 * calc_node_index - 1];
  segment_tree_node_t *right = &tree->p_value[2 * calc_node_index];

  merge_segtree_nodes(node, left, right);
}

void print_segtree(segment_tree_t *tree) {
  printf("Tree:\n");
  for (size_t i = 0; i < tree->m_capacity; i++) {
    if (tree->p_value[i].m_present) {
      segment_tree_node_t *node = &tree->p_value[i];
      printf("{ [%ld], %lld, %lld, (%lld - %lld) }, ", i, *node->p_lowest,
             *node->p_highest, *node->p_loss_start, *node->p_loss_end);
    } else {
      printf("{}, ");
    }
  }
  puts("\n");
}

int rebuild_segtree(segment_tree_t *tree) {
  if (tree->m_capacity + 1 <= tree->p_vec->m_length * 2) {
    size_t new_cap = ((tree->m_capacity + 1) << 1) - 1;
    segment_tree_node_t *newptr = (segment_tree_node_t *)realloc(
        tree->p_value, new_cap * sizeof(*tree->p_value));
    if (newptr == NULL)
      return -1;
    // Zero everything new
    memset((char *)newptr, 0, new_cap * sizeof(*tree->p_value));
    tree->p_value = newptr;
    tree->m_capacity = new_cap;
    build_segtree_node(tree, 0, 0, ((tree->m_capacity + 1) >> 1) - 1);
  } else {
    update_segtree_value(tree, 0, 0, ((tree->m_capacity + 1) >> 1) - 1,
                         tree->p_vec->m_length - 1);
  }

  // print_segtree(tree);

  return 0;
}

int push_segtree(segment_tree_t *tree) { return rebuild_segtree(tree); }

void query_segtree(segment_tree_node_t *result, segment_tree_t *tree,
                   size_t node_index, size_t node_start, size_t node_end,
                   size_t query_start, size_t query_end) {
  segment_tree_node_t *node = &tree->p_value[node_index];

  if (query_end < node_start || query_start > node_end) {
    result->m_present = false;
    return;
  }
  if (query_start <= node_start && query_end >= node_end) {
    copy_segtree_node(result, node);
    return;
  }

  size_t calc_node_index = node_index + 1;
  size_t node_center = (node_start + node_end) / 2;
  segment_tree_node_t left;
  segment_tree_node_t right;

  query_segtree(&left, tree, 2 * calc_node_index - 1, node_start, node_center,
                query_start, query_end);
  query_segtree(&right, tree, 2 * calc_node_index, node_center + 1, node_end,
                query_start, query_end);

  merge_segtree_nodes(result, &left, &right);
}

void find_profits(segment_tree_t *tree, size_t start, size_t end) {
  segment_tree_node_t result;
  query_segtree(&result, tree, 0, 0, ((tree->m_capacity + 1) >> 1) - 1, start,
                end);

  if (result.m_present == false) {
    printf("Weird, this shouldnt happen.\n");
    return;
  }

  long long profit = *result.p_profit_end - *result.p_profit_start;
  if (profit) {
    printf("Nejvyssi zisk: %lld (%ld - %ld)\n", profit,
           result.p_profit_start - tree->p_vec->p_value,
           result.p_profit_end - tree->p_vec->p_value);
  } else {
    printf("Nejvyssi zisk: N/A\n");
  }
  long long loss = *result.p_loss_start - *result.p_loss_end;
  if (loss) {
    printf("Nejvyssi ztrata: %lld (%ld - %ld)\n", loss,
           result.p_loss_start - tree->p_vec->p_value,
           result.p_loss_end - tree->p_vec->p_value);
  } else {
    printf("Nejvyssi ztrata: N/A\n");
  }
}

int main() {
  int retval = 0;

  vec_t vec;
  segment_tree_t segtree;
  if (init_vec(&vec, 5) != 0 || init_segtree(&segtree, &vec) != 0) {
    printf("Allocation error.\n");
    retval = 0;
    goto exit;
  }

  printf("Ceny, hledani:\n");

  while (true) {
    input_t op;
    int result = parse_input(&op, vec.m_length - 1);

    if (result == -1)
      break;
    if (result == 1) {
      printf("Nespravny vstup.\n");
      retval = -1;
      goto exit;
    }

    if (op.m_op == '+') {
      if (push_vec(&vec, op.m_start) != 0 || push_segtree(&segtree) != 0) {
        printf("Allocation error.\n");
        retval = -1;
        goto exit;
      }
    }

    if (op.m_op == '?') {
      find_profits(&segtree, (size_t)op.m_start, (size_t)op.m_end);
    }
  }

exit:
  free(vec.p_value);
  free(segtree.p_value);
  return retval;
}
