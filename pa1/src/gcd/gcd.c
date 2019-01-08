#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

int euclid(int first,int second);

int euclid(int first, int second)
{
	if(first<0){
		first=-1*first;
	}
	if(second<0){
		second=-1*second;
	}
	int small=first;
	int big=second;
	if(first>second){
		small=second;
		big=first;
	}
	if(small==0){
		return(big);
	}else{
		return(euclid(second,first%second));
	}
}

int main(int argc, char const *argv[])
{
	if (argc==1||argc==2||argc>=4){
		return 0;
	}else{
		printf("%d\n", euclid(atoi(argv[1]),atoi(argv[2])));
	}
	/* code */
	return 0;
}