#include <stdio.h>
#include <stdlib.h>
void dummy_fixed_pim_begin(){}

void dummy_fixed_pim_end(){}

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
		
		dummy_fixed_pim_begin();
		y[i] = (x[i] + x[i-1] + x[i+1])/3;  
		dummy_fixed_pim_end();
	}

	printf( "Value of i = %d , x[i] = %d , y[i] =%d \n ",i-1,x[i-1], y[i-1]);
}

int main(int ac, const char *av[])
{
	foo();
	return 0;
}
