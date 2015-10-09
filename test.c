#include <stdio.h>
void dummy_fixed_pim_begin()
{
  printf("dummy begin\n");
}

void dummy_fixed_pim_end()
{
  printf("dummy end\n");
}

int main() {
  dummy_fixed_pim_begin();
  printf("hello world!\n");
  dummy_fixed_pim_end();

  return 0;
}
