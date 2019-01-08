#include <stdio.h>
#include <stdlib.h>

//variable names are steven universe themed cuz binary but theyre all gay lol

typedef struct nodeStruct{
	int value;
	int children;
	struct nodeStruct * leftChild; //lesser
	struct nodeStruct * rightChild; //greater
} Node;

void search(Node * root, int query){
	if(!root){
		printf("%s\n", "absent");
	}else{
		if(root->value==query){
			printf("%s\n", "present");
		}else if(root->value>query){
			search(root->leftChild,query);
		}else if(root->value<query){
			search(root->rightChild,query);
		}else{
			printf("%s\n", "absent");
		}
	}
}

Node * insert(Node * root, Node * steven){
	if(!root){
		printf("%s\n", "inserted");
		return(steven);
	}
	if(steven->value==root->value){
		printf("%s\n", "duplicate");
		return(root);
	}else if(steven->value<root->value){
		root->children+=1;
		root->leftChild=insert(root->leftChild,steven);
		return(root);
	}else if(steven->value>root->value){
		root->children+=1;
		root->rightChild=insert(root->rightChild,steven);
		return(root);
	}else{ //this literally cant occur lol
		return(root);
	}
}

void printNode(Node * root){
	if(!root){
		return;
	}
	printf("(");
	printNode(root->leftChild);
	printf("%d",root->value);
	printNode(root->rightChild);
	printf(")");
	return;
}

Node * getMax(Node * root){
	if(!root){
		return NULL;
	}else{
		if(!root->rightChild){
			return(root);
		}else{
			return(getMax(root->rightChild));
		}
	}
}

Node * delete(Node * root, int query,int garnet){
	if(!root){
		printf("%s\n", "absent");
		return(NULL);
	}else{
		if(root->value==query){
			if(garnet==2){
				printf("%s\n", "deleted");
			}
			if((!root->rightChild)&&(!root->leftChild)){
				free(root);
				return(NULL);
				//free(root);
			}else if(!root->rightChild){
				Node * out=root->leftChild;
				free(root);
				return(out);
			}else if(!root->leftChild){
				Node * out=root->rightChild;
				free(root);
				return(out);
			}else{
				Node * amethyst=getMax(root->leftChild);
				root->children-=1;
				int amValue=amethyst->value;
				root=delete(root,amethyst->value,0);
				root->value=amValue;
				return(root);
			}
			//found it uwu
		}else if(root->value>query){
			root->children-=1;
			root->leftChild=delete(root->leftChild,query,garnet);
		}else if(root->value<query){
			root->children-=1;
			root->rightChild=delete(root->rightChild,query,garnet);
		}else{
			printf("%s\n", "absent");
		}
		return(root);
	}
}

void clean(Node * root){
	if(!root){
		return;
	}
	clean(root->rightChild);
	free(root->rightChild);
	clean(root->leftChild);
	free(root->leftChild);
}

int main(int argc, char const *argv[])
{
	FILE * fp;
	if(argc>1){
		fp=fopen(argv[1],"r+");
	}else{
		fp=stdin;
	}

	char inputChar[1];
	Node * root=NULL;
	while(1){
		int value=0;
		if(fscanf(fp," %c",&inputChar[0])!=EOF){
			//printf("inputChar[0] is %c\n", inputChar[0]);
			if(inputChar[0]=='i'){
				fscanf(fp,"%i",&value);
				Node * steven=(Node *) malloc(sizeof(Node));
				steven->value=value;
				steven->children=0;
				steven->leftChild=NULL;
				steven->rightChild=NULL;
				root=insert(root,steven);
			}else if(inputChar[0]=='d'){
				fscanf(fp,"%i",&value);
				root=delete(root,value,2);
			}else if(inputChar[0]=='s'){
				fscanf(fp,"%i",&value);
				/*
				if(isalpha(value)!=0){
					break;
				}
				*/
				search(root,value);
				continue;
			}else if(inputChar[0]=='p'){
				printNode(root);
				printf("\n");
				continue;
			}else{
				//printf("%s\n", "other");
				break;
			}
		}else{
			break;
		}
	}
	//free(inputChar);

	/* code */
	clean(root);
	free(root);
	return 0;
}