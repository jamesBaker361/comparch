#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int error(){
  printf("error");
  return 0;
}

int main(int argc, char const *argv[])
{
  if(argc!=2){
    return 0;
  }
  int r=0;
  int argLength=(int) strlen(argv[1]);
  for(r=0;r<argLength;r+=1){
    switch(argv[1][r]){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      printf("error\n");
      return 0;
    }
  }
  char * out;
  int length=0;
  out= (char*)malloc(sizeof(char)*argLength);
  char current=argv[1][0];
  int charCount=1;
  if(argLength==2&&argv[1][0]==argv[1][1]){
    printf("%c",argv[1][0]);
    printf("%c\n",'2');
    return 0;
  }
  int h=1;
  for(h=1;h<argLength;h=h+1){
    if(argv[1][h]==current){
      charCount+=1;
    }else{
      int addStrLen=2;
      if(charCount>9){
	addStrLen+=1;
      }
      if(charCount>99){
	addStrLen+=1;
      }
      if(charCount>999){
	addStrLen+=1;
      }
      if(charCount>9999){
	addStrLen+=1;
      }
      if(charCount>99999){
	addStrLen+=1;
      } 
      if(charCount>999999){
	addStrLen+=1;
      } 
      if(charCount>9999999){
	addStrLen+=1;
      } 
      //addStr=(char *)malloc(sizeof(char)*addStrLen);
      length+=addStrLen;
      if(length>argLength){
	printf("%s\n",argv[1]);
	return 0;
      }
      out[length-addStrLen]=current;
      sprintf(out+1+length-addStrLen,"%d",charCount);
      //out[1+length-addStrLen]= itoa(charCount);
      current=argv[1][h];
      charCount=1;
    }
  }
  int addStrLen=2;
  if(charCount>9){
    addStrLen+=1;
  }
  if(charCount>99){
    addStrLen+=1;
  }
  if(charCount>999){
    addStrLen+=1;
  }
  if(charCount>9999){
    addStrLen+=1;
  }
  if(charCount>99999){
    addStrLen+=1;
  }
  if(charCount>999999){
    addStrLen+=1;
  }
  if(charCount>9999999){
    addStrLen+=1;
  }
  //addStr=(char *)malloc(sizeof(char)*addStrLen);                                                                                                                
  length+=addStrLen;
  if(length>=argLength){
    printf("%s\n",argv[1]);
    return 0;
  }
  out[length-addStrLen]=current;
  sprintf(out+1+length-addStrLen,"%d",charCount);
  printf("%s\n",out);
  /* code */
  return 0;
}
