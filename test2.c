#include <stdio.h>
#include <stdlib.h>
#define N 10
void dummy_fixed_pim_begin(){}
void dummy_fixed_pim_end(){}
void dummy_gen_pim_begin(){}
void dummy_gen_pim_end(){}

void do_y(float* u)
{
	int i;
	for (i = 1; i < N; i++ )
		u[i] = u[i]*1.2;
}

void foo(void)
{
	float x[N+10];
	float y[N+10];
	int i,j;
	time_t t;

	srand((unsigned) time(&t));
	for(i=0; i< N+2; i++)
		x[i] = rand();

	dummy_gen_pim_begin();
	for ( i = 0; i < N+2; i++ ) {
			
		for(j = 3; j < N-1; j++)
		{	
			dummy_fixed_pim_begin();
			y[j] = (x[j] + x[j-1] + x[j+1])/3;
			dummy_fixed_pim_end();
		}
		
		do_y(y);  
	}
	dummy_gen_pim_end();

	
	printf("y[4]=%f\n", y[4]);
	printf("y[N-1]=%f\n", y[N-2]);
	printf("y[5]=%f\n", y[5]);
}


int main(int ac, const char *av[])
{
	foo();
	return 0;
}
