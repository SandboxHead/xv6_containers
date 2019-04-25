// #include "types.h"
// #include "stat.h"
// #include "user.h"
#include "ls.c"

int main(void){
	int c1 = create_container();
	int c2 = create_container();
	int c3 = create_container();

	int fid = open("aniket", 512);
	close(fid);

	fid = open("aniket", 0x001);
	char* writing = "xyz";
	write(fid, (void*)writing, 5);
	close(fid);

	int f1 = fork();
	if(f1 == 0){
		printf(1, "Inside Container 1.\n");

		join_container(c1);
		int cid = get_container();
		printf(1, "going forward %d \n", cid);
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

		// int fid = open("aniket", 512);
		// char* writing = "abcde";
		// write(fid, (void*)writing, 5);
		// close(fid);

		fid = open("aniket", 0x002);
		char* writing = "abcde";
		write(fid, (void*)writing, 5);
		close(fid);
		// ls();
		// char *out = "aniket";
		// char *out;
		// char *out1[1] = {out};
		// exec("ls", &out);
		ls(".");
		print_ls();
		// int i = 0;
		// for(i=0;;i++){
		// 	if(out[i] == '\0') break;
		// 	printf(1, "%c ", out[i]);
		// }
		printf(1, "abc\n");

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