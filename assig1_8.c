#include "types.h"
#include "stat.h"
#include "user.h"

char *
to_string(int n)
{
    int nod = 0;
    int tmp = n;
    while (n > 0)
    {
        n /= 10;
        nod++;
    }

    char * s = (char *) malloc(MSGSIZE);
    for (int i = nod - 1; i > -1; --i)
    {
        s[i] = '0' + tmp%10;
        tmp /= 10;
    }

    return s;
}

int
main(int argc, char *argv[])
{
	if(argc< 2){
		printf(1,"Need type and input filename\n");
		exit();
	}
	char *filename;
	filename=argv[2];
	int type = atoi(argv[1]);
	printf(1,"Type is %d and filename is %s\n",type, filename);

	int tot_sum = 0;	
	float variance = 0.0;

	int size=1000;
	short arr[size];
	char c;
	int fd = open(filename, 0);
	for(int i=0; i<size; i++){
		read(fd, &c, 1);
		arr[i]=c-'0';
		read(fd, &c, 1);
	}	
  	close(fd);
  	// this is to supress warning
  	printf(1,"first elem %d\n", arr[0]);
  
  	//----FILL THE CODE HERE for unicast sum and multicast variance

    int parent_pid = getpid();
    char * msg = (char *)malloc(MSGSIZE);
    for (int i = 0; i < 8; i++)
    {
        if (fork() == 0)
        {
            int start = i * (size >> 3);
            int end = start + (size >> 3);
            if (i == 7)
                end += (size - end);

            int sum = 0;
            for (int j = start; j < end; j++)
                sum += arr[j];

            send(getpid(), parent_pid, to_string(sum));
            exit();
        }
    }
    for (int i = 0; i < 8; i++) {
        recv(msg);
        //printf(1,"Recieved %s \n", msg);
	tot_sum += atoi(msg);
    }
    wait();

  	//------------------

  	if(type==0){ //unicast sum
		printf(1,"Sum of array for file %s is %d\n", filename,tot_sum);
	}
	else{ //mulicast variance
		printf(1,"Variance of array for file %s is %d\n", filename,(int)variance);
	}
	exit();
}
