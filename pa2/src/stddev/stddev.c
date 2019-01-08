#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct nodeStruct{
	double value;
	struct nodeStruct * next;
} Node;

int main(int argc, char const *argv[])
{
	FILE * fp;
	if(argc>1){
		fp=fopen(argv[1],"r+");
	}else{
		fp=stdin;
	}
	int count=0;
	double sum=0;
	Node * head=NULL;
	double value=0;
	while(fscanf(fp,"%lf",&value)>0){
		Node * newman=(Node *) malloc(sizeof(Node));
		newman->value=value;
		newman->next=head;
		head=newman;
		sum+=value;
		count+=1;
	}
	if(count==0){
		printf("no data\n");
		return 0;
	}
	double mean=sum/(double)count;
	double variance=0;
	int u=0;
	Node * last=NULL;
	while(u<count){
		u+=1;
		variance+=(mean-head->value)*(mean-head->value);
		last=head;
		head=head->next;
		free(last);
	}
	free(head);
	variance=variance/count;
	double stdv=sqrt(variance);
	printf("mean: %d\n", (int) (mean+0.5));
	printf("stddev: %d\n", (int) (stdv+0.5));
	/* code */
	return 0;
}