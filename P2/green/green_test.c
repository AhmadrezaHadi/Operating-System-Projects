#include <stdio.h>
#include <stdlib.h>
#ifdef POSIX
#include "posix.h"
#else
#include "green.h"
#endif
#include <unistd.h>
#include <time.h>
typedef struct my_arg_t{
	int p;
	int max_num;
} my_arg_t;
/*
void print_num(int num)
{
	char digits[20];
	int len=0;
	if(num==0)
		write(1,'0',1);
	else if(num<0){
		write(1,"-",1);
		num=-num;
	}
	while(num>0)
	{
		digits[len++]='0' +num%10;
		num/=10;
	}
	for(len--;len>=0;len--)
		write(1,&digits[len],1);
}
*/
void * test(void * arg){
	my_arg_t args_struct = *(my_arg_t *) arg;
	int loop = args_struct.max_num;
	int p=args_struct.p;
	int *sum=(int *)malloc(sizeof(int));
	// *sum = 0;
	//printf("\nsum : %d",*sum);
	while ( loop > 0 ){
		printf("thread #%d : %d\n" , p , loop );
		
		(*sum)+=loop;
		loop--;
		usleep(100000);
		green_yield();
	}
	printf("thread #%d finished\n",p);

	return (void *)sum;
}
int main (int argc , char * argv[]){
	green_t g[3];
	int thread_args[3]={2,8,5};
	my_arg_t a[3];
	srand(time(0));
	for(int i=0;i<3;i++){
		a[i].p=i;
		a[i].max_num=rand()%10+1;
	}
	
	for(int i=0;i<3;i++)
		green_create(&g[i] ,test ,&a[i]);

	int *retval =(int*)malloc(sizeof(int));
	for(int i=0;i<3;i++){
		green_join(&g[i], (void **)&retval);
		
		printf("Result for thread #%d: %d\n",i,*retval);
	}
	printf("all threads are finished.\n");
		
	return 0 ;
}
