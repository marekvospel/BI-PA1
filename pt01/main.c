#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  double x;
  double y;
} Point;

typedef Point Vector2;

int load_point(Point *point) {
  double x, y;
  char ch;
  int result;

  result = scanf(" [ %lf , %lf %c", &x, &y, &ch);

  if (result != 3) {
    return -1;
  }

  if (ch != ']') {
    return -1;
  }

  point->x = x;
  point->y = y;

  return 0;
}

Point add_vec(Point point, Vector2 vector) {
  Point p;
  p.x = point.x + vector.x;
  p.y = point.y + vector.y;

  return p;
}

Point sub_vec(Point point, Vector2 vector) {
  Point p;
  p.x = point.x - vector.x;
  p.y = point.y - vector.y;

  return p;
}

Vector2 sub_points(Point pointa, Point pointb) {
  Vector2 v;
  v.x = pointa.x - pointb.x;
  v.y = pointa.y - pointb.y;

  return v;
}

bool double_aprox_eq(double a, double b) {
  if (fabs(a - b) < DBL_EPSILON) {
    return true;
  }

  return fabs(a - b) <= 1000 * (fabs(a) + fabs(b)) * DBL_EPSILON;
}

bool same_vector(Vector2 a, Vector2 b) {

  if (double_aprox_eq(0, a.x))
    return double_aprox_eq(0, b.x);
  if (double_aprox_eq(0, b.x))
    return double_aprox_eq(0, a.x);
  if (double_aprox_eq(0, a.y))
    return double_aprox_eq(0, b.y);
  if (double_aprox_eq(0, b.y))
    return double_aprox_eq(0, a.y);

  double coeficient = a.x / b.x;
  double coeficient2 = a.y / b.y;

  bool retval = double_aprox_eq(coeficient2, coeficient);

  return retval;
}

Vector2 invert_vector(Vector2 vec) {
  Vector2 nVec;
  nVec.x = -vec.y;
  nVec.y = vec.x;
  return nVec;
}

/// 0 = rovnobeznik
/// 1 = obdelnik
/// 2 = kosoctverec
/// 3 = ctverec
int calc_type(Point a, Point b, Point d) {
  int out = 0;

  Vector2 vab = sub_points(b, a);
  double ab_len = sqrt(pow(vab.x, 2) + pow(vab.y, 2));
  Vector2 vad = sub_points(d, a);
  double ad_len = sqrt(pow(vad.x, 2) + pow(vad.y, 2));

  // if sides are equal length
  if (double_aprox_eq(ab_len, ad_len))
    out += 2;

  // If has 90degrees
  if (same_vector(vab, invert_vector(vad)))
    out += 1;

  return out;
}

void print_quadrilateral_type(int type) {
  switch (type) {
  case (0):
    printf("rovnobeznik");
    break;
  case (1):
    printf("obdelnik");
    break;
  case (2):
    printf("kosoctverec");
    break;
  case (3):
    printf("ctverec");
    break;
  }
}

int main() {
  Point a, b, c;

  printf("Bod A:\n");
  if (load_point(&a) != 0) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  printf("Bod B:\n");
  if (load_point(&b) != 0) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  printf("Bod C:\n");
  if (load_point(&c) != 0) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  // vector A->B
  Vector2 vab = sub_points(b, a);

  // vector A->C
  Vector2 vac = sub_points(c, a);

  // Aproximate whether point is on our direction vector
  if (same_vector(vab, vac)) {
    printf("Rovnobezniky nelze sestrojit.\n");
    return -1;
  }

  Point newA = add_vec(c, vab);
  int newAType = calc_type(a, b, c);
  Point newB = sub_vec(c, vab);
  int newBType = calc_type(a, b, newB);
  Point newC = sub_vec(b, vac);
  int newCType = calc_type(a, newC, c);

  printf("A': [%.10g,%.10g], ", newA.x, newA.y);
  print_quadrilateral_type(newAType);
  puts("");

  printf("B': [%.10g,%.10g], ", newB.x, newB.y);
  print_quadrilateral_type(newBType);
  puts("");

  printf("C': [%.10g,%.10g], ", newC.x, newC.y);
  print_quadrilateral_type(newCType);
  puts("");

  return 0;
}
