#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char getNut(char ** board,int y, int x, int h, int w){
	//returns . if unpop or * if pop
	int neighbors=0;
	int upper=y-1;
	if(upper<0){
		upper=h-1;
	}
	int lower=y+1;
	if(lower>=h){
		lower=0;
	}
	int left=x-1;
	if(left<0){
		left=w-1;
	}
	int right=x+1;
	if(right>=w){
		right=0;
	}
	int * yArr;
	int yCount=3;
	if(h==2){
		yCount=2;
		yArr=(int *)malloc(2*sizeof(int));
		yArr[0]=y;
		yArr[1]=upper;
	}else if(h==1){
		yCount=1;
		yArr=(int *)malloc(1*sizeof(int));
		//printf("%s\n", "problemzzz y");
		yArr[0]=y;
	}else{
		yArr=(int *)malloc(3*sizeof(int));
		yArr[0]=y;
		yArr[1]=upper;
		yArr[2]=lower;
	}
	int * xArr;
	int xCount=3;
	if(w==2){
		xCount=2;
		xArr=(int *)malloc(2*sizeof(int));
		xArr[0]=x;
		xArr[1]=left;
	}else if(w==1){
		xCount=1;
		xArr=(int *)malloc(1*sizeof(int));
		xArr[0]=x;
		//printf("%s\n", "problemzzzz x");
	}else{
		xArr=(int *)malloc(3*sizeof(int));
		xArr[0]=x;
		xArr[1]=left;
		xArr[2]=right;
	}
	int a=0;
	int b=0;
	for(a=0;a<yCount;a+=1){
		for(b=0;b<xCount;b+=1){
			if(board[yArr[a]][xArr[b]]=='*'){
				neighbors+=1;
			}
		}
	}
	free(xArr);
	free(yArr);
	if(board[y][x]=='*'){
		if(neighbors>4||neighbors<3){
			return('.');
		}else{
			return('*');
		}
	}else{
		if(neighbors==3){
			return('*');
		}else{
			return('.');
		}
	}
}

char ** boardFetus(int h,int w){
	char ** board;
	board= (char**)malloc(h*sizeof(char*));
	int vert=0;
	int horz=0;
	for(vert=0;vert<h;vert+=1){
		board[vert]=(char *)malloc(w*sizeof(char));
		for (horz=0; horz < w; horz+=1){
			board[vert][horz]='.';
			//horz=horz;
		}
	}
	return(board);
}

void printBoard(char ** matrix, int h,int w){
	int a=0;
	int b=0;
	for(a=0;a<h;a+=1){
		for(b=0;b<w;b+=1){
			printf("%c",matrix[a][b]);
		}
		printf("\n");
	}
}

char ** boardFill(FILE * fp, char** board, int height, int width){
	int vert=0;
	int horz=0;
	for(vert=0;vert<height;vert+=1){
		for(horz=0; horz<width; horz+=1){
			fscanf(fp," %c ",&board[vert][horz]);
		}
	}
	return(board);
}

char ** oneTurn(char ** board,int h,int w){
	char ** out;
	out=boardFetus(h,w);
	int a=0;
	int b=0;
	for(a=0;a<h;a+=1){
		for(b=0;b<w;b+=1){
			//out[a][b]='b';
			out[a][b]=getNut(board,a,b,h,w);
		}
	}
	return(out);
}

void abortBoard(char ** board,int h, int w){
	int a=0;
	for(a=0;a<h;a+=1){
		free(board[a]);
	}
	free(board);
}

int main(int argc, char const *argv[])
{
	int turns=0;
	int h,w;
	FILE * fp;
	if(argc>2){
		turns=atoi(argv[1]);
		fp=fopen(argv[2],"r+");
		fscanf(fp,"%i",&h);
		fscanf(fp,"%i",&w);
		if(w<1||h<1){
			return 0;
		}
		char ** embryo;
		embryo=boardFetus(h,w);
		embryo=boardFill(fp,embryo,h,w);
		while(turns>0){
			char ** twin=embryo;
			embryo=oneTurn(embryo,h,w);
			abortBoard(twin,h,w);
			//printBoard(embryo,h,w);
			turns-=1;
		}
		printBoard(embryo,h,w);
		abortBoard(embryo,h,w);
		/*
		printBoard(embryo,h,w);
		embryo=oneTurn(embryo,h,w);
		printBoard(embryo,h,w);
		embryo=oneTurn(embryo,h,w);
		printBoard(embryo,h,w);
		*/
	}
	/* code */
	//printf("%s\n", "uwu");
	return 0;
}