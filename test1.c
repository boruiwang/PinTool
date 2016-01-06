#include <stdio.h>
#include <stdlib.h>
void dummy_fixed_pim_begin(){}
void dummy_fixed_pim_end(){}
void dummy_gen_pim_begin(){}
void dummy_gen_pim_end(){}

void foo(void)
{
	int N = 1000;
	int x[N+3];
	int y[N+3];
	int i;
	time_t t;
	
	/* Intializes random number generator */
   	srand((unsigned) time(&t));
	
	for ( i = 0; i < N; i++ ) {
		x[i] = rand() ;
		
		//dummy_fixed_pim_begin();
		y[i] = (x[i] + x[i-1] + x[i+1])/3;  
		//dummy_fixed_pim_end();
	}
}

int main(int ac, const char *av[])
{
	//dummy_gen_pim_begin();
	foo();
	//dummy_gen_pim_end();
	return 0;
}
