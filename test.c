// #include "types.h"
// #include "stat.h"
// #include "user.h"
#include "ls1.c"

char buf[512];

void
cat_util(int fd)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
cat(char *argv)
{
  	int fd;
	if((fd = open(argv, 0x000)) < 0){
	  printf(1, "cat: cannot open %s\n", argv);
	  exit();
	}
	cat_util(fd);
	close(fd);
 	exit();
}

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
		// ls(".");
		// print_ls();
		fid = open("aniket", 0x002);
		char* writing = "abcde";
		write(fid, (void*)writing, 5);
		close(fid);
		// char *args[14];
		char a[6] ="aniket";
		// strcpy(a , "aniket");
		// args[0] = a;
		// printf(1, "%s\n", a);
		cat(a);
		// ls();
		// char *out = "aniket";
		// char *out;
		// char *out1[1] = {out};
		// exec("ls", &out);
		// ls(".");
		// print_ls();
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