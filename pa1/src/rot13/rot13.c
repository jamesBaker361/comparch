#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"

int main(int argc, char const *argv[])
{
  char before[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char after[]="nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";
	/* code */
	if(argc==2){
      		char out[strlen(argv[1])];
		//		printf("%zu\n", strlen(out));
		//printf("%zu\n",strlen(argv[1]));
		//printf("%s\n", argv[1]);
		int i=0;
		for (i = 0; i < strlen(argv[1]); i=i+1){
			//printf("%c\n", argv[1][i]);
			char target=argv[1][i];
			int index=-1;
			int x=0;
			for(x=0;x<strlen(before);x=x+1){
				if(target==before[x]){
					index=x;
					break;
				}
			}
			char add='h';
			if(index!=-1){
				//out[i]=after[index];
				add=after[index];
			}else{
				//out[i]=target;
				add=target;
			}
			out[i]=add;
			//printf("%s\n", out);
		}
		//printf("%s\n", out);
		int y=0;
		for(y=0;y<strlen(argv[1]);y=y+1){
		  printf("%c",out[y]);
		}
		printf("\n");
		return 0;
	}else{
	  //		printf("%s\n", "heck");
		return 0;
	}
}
