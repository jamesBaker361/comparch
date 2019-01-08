#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char ** boardFetus(){
	char ** board;
	board= (char**)malloc(8*sizeof(char*));
	int vert=0;
	int horz=0;
	for(vert=0;vert<8;vert+=1){
		board[vert]=(char *)malloc(8*sizeof(char));
		for (horz=0; horz < 8; horz+=1){
			board[vert][horz]='.';
			//horz=horz;
		}
	}
	return(board);
}

void printBoard(char ** matrix){
	int a=0;
	int b=0;
	for(a=0;a<8;a+=1){
		for(b=0;b<8;b+=1){
			printf("%c",matrix[a][b]);
		}
		printf("\n");
	}
}

char ** boardFill(FILE * fp, char** board){
	int vert=0;
	int horz=0;
	for(vert=0;vert<8;vert+=1){
		for(horz=0; horz<8; horz+=1){
			fscanf(fp," %c ",&board[vert][horz]);
		}
	}
	return(board);
}

char ** cloneBoard(char ** thot){
	char ** board;
	board= (char**)malloc(8*sizeof(char*));
	int vert=0;
	int horz=0;
	for(vert=0;vert<8;vert+=1){
		board[vert]=(char *)malloc(8*sizeof(char));
		for (horz=0; horz < 8; horz+=1){
			board[vert][horz]=thot[vert][horz];
			//horz=horz;
		}
	}
	return(board);
}

void abortBoard(char ** board){
	int a=0;
	for(a=0;a<8;a+=1){
		free(board[a]);
	}
	free(board);
}

int validSquare(char ** board,int y,int x,int warrior){
	if(warrior==1){ 
		/*
		we need to test
		x+2,y+1
		x+2,y-1
		x-2,y+1,
		x-2,y-1
		y+2,x+1
		y+2,x-1
		y-2,x+1
		y-2,x-1
		*/
		/*
		int yArr[5]={y,y,y,y,y};
		int xArr[5]={x,x,x,x,x};
		int p=0;
		int q=0;
		for(p=0;p<5;p+=1){
			for(q=0;q<5;q+=1){
				if(board[yArr[p]][xArr[q]]=='Q'||board[yArr[p]][xArr[q]]=='W')
			}
		}
		*/
		if(y+2<8){
			if(x+1<8){
				if(board[y+2][x+1]=='Q'||board[y+2][x+1]=='W'){
					return 0;
				}
			}
			if(x-1>-1){
				if(board[y+2][x-1]=='Q'||board[y+2][x-1]=='W'){
					return 0;
				}
			}
		}
		if(y-2>-1){
			if(x+1<8){
				if(board[y-2][x+1]=='Q'||board[y-2][x+1]=='W'){
					return 0;
				}
			}
			if(x-1>-1){
				if(board[y-2][x-1]=='Q'||board[y-2][x-1]=='W'){
					return 0;
				}
			}
		}
		if(y+1<8){
			if(x+2<8){
				if(board[y+1][x+2]=='Q'||board[y+1][x+2]=='W'){
					return 0;
				}
			}
			if(x-2>-1){
				if(board[y+1][x-2]=='Q'||board[y+1][x-2]=='W'){
					return 0;
				}
			}
		}
		if(y-1>-1){
			if(x+2<8){
				if(board[y-1][x+2]=='Q'||board[y-1][x+2]=='W'){
					return 0;
				}
			}
			if(x-2>-1){
				if(board[y-1][x-2]=='Q'||board[y-1][x-2]=='W'){
					return 0;
				}
			}
		}
	}
		int a=0; //check horizontally
		for(a=0;a<8;a+=1){
			if((board[y][a]=='Q'||board[y][a]=='W')&&a!=x){
				return 0;
			}
		}
		int b=0;
		for(b=0;b<8;b+=1){
			if((board[b][x]=='Q'||board[b][x]=='W')&&b!=y){
				return 0;
			}
		}
		int neX=x+1; //northeast we checking diagonally up to the right
		int neY=y-1;
		while(neX<8&&neY>-1){
			if(board[neY][neX]=='Q'||board[neY][neX]=='W'){
				return 0;
			}
			neX+=1;
			neY-=1;
		}
		int nwX=x-1;
		int nwY=y-1;
		while(nwX>-1&&nwY>-1){
			if(board[nwY][nwX]=='Q'||board[nwY][nwX]=='W'){
				return 0;
			}
			nwX-=1;
			nwY-=1;
		}
		int swX=x-1;
		int swY=y+1;
		while(swX>-1&&swY<8){
			if(board[swY][swX]=='Q'||board[swY][swX]=='W'){
				return 0;
			}
			swY+=1;
			swX-=1;
		}
		int seX=x+1;
		int seY=y+1;
		while(seY<8&&seX<8){
			if(board[seY][seX]=='Q'||board[seY][seX]=='W'){
				return 0;
			}
			seY+=1;
			seX+=1;
		}
	
	return 1;
}

int validBoard(char ** board,int warrior){
	int a=0;
	int b=0;
	for(a=0;a<8;a+=1){
		for(b=0;b<8;b+=1){
			if(board[a][b]=='Q'&&validSquare(board,a,b,0)==0){
				printf("%s\n", "Invalid");
				return 0;
			}
			if(warrior==1&&board[a][b]=='W'&&validSquare(board,a,b,1)==0){
				printf("%s\n", "Invalid");
				return 0;
			}
		}
	}
	return 1;
	//printf("%s\n", "Valid!!!");
}

int queenCheck(char ** board,int y, int x){ //returns 1 if its  possible to put a queen on y,x without worrying about any warriors boonking them
		if(y+2<8){
			if(x+1<8){
				if(board[y+2][x+1]=='W'){
					return 0;
				}
			}
			if(x-1>-1){
				if(board[y+2][x-1]=='W'){
					return 0;
				}
			}
		}
		if(y-2>-1){
			if(x+1<8){
				if(board[y-2][x+1]=='W'){
					return 0;
				}
			}
			if(x-1>-1){
				if(board[y-2][x-1]=='W'){
					return 0;
				}
			}
		}
		if(y+1<8){
			if(x+2<8){
				if(board[y+1][x+2]=='W'){
					return 0;
				}
			}
			if(x-2>-1){
				if(board[y+1][x-2]=='W'){
					return 0;
				}
			}
		}
		if(y-1>-1){
			if(x+2<8){
				if(board[y-1][x+2]=='W'){
					return 0;
				}
			}
			if(x-2>-1){
				if(board[y-1][x-2]=='W'){
					return 0;
				}
			}
		}
		return 1;
	}

int allWomen(char ** mom,char ** kid, int warrior){ //returns the quantity of places we putting in a queen
	int a=0;
	int b=0;
	int count=0;
	for(a=0;a<8;a+=1){
		for(b=0;b<8;b+=1){
			if(mom[a][b]!='W'&&mom[a][b]!='Q'&&validSquare(mom,a,b,0)==1){
				kid[a][b]='q';
				if(warrior==1&&validSquare(mom,a,b,1)==1){
					kid[a][b]='w';
				}else if(warrior==1&&queenCheck(mom,a,b)==0){
					kid[a][b]=mom[a][b];
					count-=1;
				}
				count+=1;
			}else{
				kid[a][b]=mom[a][b];
			}
		}
	}
	return(count);
}

int ifSheBreathe(char ** board,char ** newBoard, int warrior){ //this does the +2 thing
	int a=0;
	int b=0;
	for(a=0;a<8;a+=1){
		for(b=0;b<8;b+=1){
			if(newBoard[a][b]!='Q'&&newBoard[a][b]!='.'&&newBoard[a][b]!='W'){ //if newBoard is the same as board, then every char will be one of these three
				char ** thot;
				thot=cloneBoard(board);
				thot[a][b]='Q'; //we will NEVER place a W that onyl makes life harder ya feel
				if(allWomen(thot,thot,warrior)>0){ //so if this > 0 then we can put two whole queens on this board somehwere uwu
					free(thot);
					return 1; 
				}
			}
		}
	}
	return 0;
} 

int main(int argc, char const *argv[])
{
	FILE * fp;
	int pTwo=0;
	int warrior=0;
	int newQueens=0;
	if(argc==1){
		return 0;
	}else if(argc==2){
		fp=fopen(argv[1],"r+");
	}else if(argc==3){
		if(argv[1][0]=='-'){
			warrior=1;
		}else{
			pTwo=1;
		}
		fp=fopen(argv[2],"r+");
	}else if(argc==4){
		pTwo=1;
		warrior=1;
		fp=fopen(argv[3],"r+");
	}
	//printf("%d\n", warrior+pTwo);
	char ** board;
	board=boardFetus();
	board=boardFill(fp,board);
	//printBoard(board);
	if(validBoard(board,warrior)==0){ //part 1; checks to see if valid board and all that
		abortBoard(board);
		return 0;
	}

	char ** newBoard;
	newBoard=boardFetus();

	if(allWomen(board,newBoard,warrior)>0){
		newQueens+=1;
	}



	printBoard(newBoard);

	if(pTwo==1){
		if(ifSheBreathe(board,newBoard,warrior)>0){
			newQueens+=1;
		}
		if(newQueens==0){
			printf("%s\n", "Zero");
		}else if(newQueens==1){
			printf("%s\n", "One");
		}else{
			printf("%s\n", "Two or more");
		}
	}

	abortBoard(board);
	return 0;
}