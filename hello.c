

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



int
main(int argc,char *argv[]){
	char arr[1000000];
	read(0,arr,10000);
	printf("%s\n",arr);

	printf("hello\n");
	return 0;

}
