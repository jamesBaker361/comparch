#include <stdio.h>
#include <stdlib.h>

int ** matLab(int ** first, int ** second, int k){
	//k is length
	int ** out;
	out=(int **) malloc(k*sizeof(int*));
	int y=0;
	for(y=0;y<k;y+=1){
		out[y]=(int *) malloc(k*sizeof(int));
	}
	int f=0;
	int s=0;
	for(f=0;f<k;f+=1){
		for(s=0;s<k;s+=1){
			int sum=0;
			int c=0;
			for(c=0;c<k;c+=1){
				int product=first[c][f]*second[s][c];
				//printf("+ %d * %d = %d ", first[c][f],second[s][c],product);
				sum=sum+product;
			}
			out[s][f]=sum;
			//printf("\n sum is %d\n", sum);
		}
	}
	return(out);
}

int ** copyMatrix(int **matrix, int k){
	int ** out;
	out=(int **) malloc(k*sizeof(int*));
	int y=0;
	for(y=0;y<k;y+=1){
		out[y]=(int *) malloc(k*sizeof(int));
	}
	int a=0;
	int b=0;
	for(a=0;a<k;a+=1){
		for(b=0;b<k;b+=1){
			out[a][b]=matrix[a][b];
		}
	}
	return(out);
}


int ** enterMatrix(int k,FILE * fp){

	int **matrix;
	matrix=(int **) malloc(k*sizeof(int*));
	int y=0;
	for(y=0;y<k;y+=1){
		matrix[y]=(int *) malloc(k*sizeof(int));
	}
	int a=0;
	int b=0;
	for(a=0;a<k;a+=1){
		for(b=0;b<k;b+=1){
			fscanf(fp,"%d",&matrix[a][b]);
		}
	}
	return(matrix);
}

void printMatrix(int ** matrix, int k){
	int a=0;
	int b=0;
	for(a=0;a<k;a+=1){
		for(b=0;b<k;b+=1){
			printf("%d",matrix[a][b]);
			if(b<k-1){
				printf(" ");
			}
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[])
{
	/* code */
	int k;
	FILE * fp;
	if(argc>1){
		fp=fopen(argv[1],"r+");
	}else{
		fp=stdin;
	}
	fscanf(fp,"%d",&k);
	int ** matrix;
	matrix=enterMatrix(k,fp);

	int expo=0;
	fscanf(fp,"%d",&expo);

	int b=0;
	int ** redPill;
	redPill=copyMatrix(matrix,k);
	if(expo>0){
	while(b<expo-1){
		redPill=matLab(matrix,redPill,k);
		b+=1;
	}
}else if(expo==0){
	int a=0;
	int b=0;
	for(a=0;a<k;a+=1){
		for(b=0;b<k;b+=1){
			if(a==b){
				redPill[a][b]=1;
			}else{
				redPill[a][b]=0;
			}
		}
	}
}else{
	return 0;
}

	printMatrix(redPill,k);
	free(matrix);
	free(redPill);
	return 0;
}