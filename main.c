#include "calculator.h"
#include <stdio.h>

int main() {
  int addition = add(2, 4);
  printf("result of addition is %d\n", addition);
  int multiplication = multiply(2, 4);
  printf("result of multiplication is %d\n", multiplication);
}