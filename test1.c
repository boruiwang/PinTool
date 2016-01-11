#include <stdio.h>
#include <stdlib.h>
void dummy_fixed_pim_begin(){}
void dummy_fixed_pim_end(){}
void dummy_gen_pim_begin(){}
void dummy_gen_pim_end(){}
void dummy_main_begin(){};
void dummy_main_end(){};

time_t t;
int x[1003];
int y[1003];
int N = 1000;

void foo(void)
{
	int i;

	dummy_fixed_pim_begin();
	for ( i = 0; i < N; i++ )
		y[i] = (x[i] + x[i-1] + x[i+1])/3;  
	dummy_fixed_pim_end();
}

int main(int ac, const char *av[])
{
	/* Intializes random number generator */
	int i;
	srand((unsigned) time(&t));
	for ( i = 0; i < N; i++ )
		x[i] = rand() ;

	dummy_main_begin();
	//dummy_gen_pim_begin();
	foo();
	//dummy_gen_pim_end();
	dummy_main_end();
	return 0;
}
