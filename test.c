#include "types.h"
#include "stat.h"
#include "user.h"


int main(void){
	int c1 = create_container();
	int c2 = create_container();
	int c3 = create_container();

	int f1 = fork();
	if(f1 == 0){
		printf(1, "Inside Container 1.\n");

		join_container(c1);
		// printf(1, "going forward\n" );
		ps();
		exit();
		// destroy_container(c1);
	}
	else if(fork() == 0){
		printf(1, "Inside Container 2.\n" );
		join_container(c2);
		ps();
		exit();
		// destroy_container(c2);
	}
	else if(fork() == 0){
		printf(1, "Inside Container 3.\n" );

		join_container(c3);
		ps();
		exit();
		// destroy_container(c3);
	}
	else{
		for(int i = 0; i < 3; i++)
			wait();

		printf(1, "Outside Container.\n" );

		ps();
		exit();
	}
}