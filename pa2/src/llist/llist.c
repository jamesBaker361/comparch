#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodeStruct{
	int value;
	struct nodeStruct * next;
} Node;

int count=0;

Node * insert(Node * first, Node * add){
	count=count+1;
	if(!first){
		return(add);
	}
	if(add->value<first->value){
		add->next=first;
		return add;
	}else if(first->value==add->value){
		count-=1;
		return(first);
	}else if(!first->next)
	{
		first->next=add;
		return(first);
	}else{
		Node * boi=first;
		Node * nextBoi=first->next;
		if(nextBoi->value==add->value||boi->value==add->value){
			count-=1;
			return(first);
		}
		while(nextBoi->value<add->value){
			if(boi->value==add->value||nextBoi->value==add->value){
				count-=1;
				return(first);
			}
			boi=boi->next;
			nextBoi=nextBoi->next;
			if(!nextBoi){
				if(boi->value==add->value){
					count-=1;
					return(first);
				}else{
					boi->next=add;
					return(first);
				}
			}else if(nextBoi->value==add->value||boi->value==add->value){
				count-=1;
				return(first);
			}

		}
		if(boi->value==add->value||nextBoi->value==add->value){
			count-=1;
			return(first);
		}
		add->next=nextBoi;
		boi->next=add;
		return(first);
	}
	return(first);
}

Node * delete(Node * first,int target){
	if(!first){
		return NULL;
	}else if(!first->next){
		if(first->value==target){
			count=count-1;
			return NULL;
		}else{
			return first;
		}
	}else{
		Node * boi=first;
		Node * nextBoi=first->next;
		if(first->value==target){
			//printf("yeehonk\n");
			first=first->next;
			count=count-1;
		}
			while(nextBoi){
				if(nextBoi->value==target){
					boi->next=nextBoi->next;
					nextBoi=boi->next;
					count=count-1;
				}else{
				boi=boi->next;
				nextBoi=nextBoi->next;
			}
			}
	}
	return first;
}

void printList(Node * first){
	printf("%i :", count);
	if(!first){
		printf("\n");
		return;
	}
	Node * boi=first;
	int h=1;
	while(h<count){
		printf(" %i", boi->value);
		boi=boi->next;
		h=h+1;
	}
	printf(" %i\n", boi->value);
}

void clean(Node * first){
	if(!first){
		return;
	}
	Node * navi=first->next;
	while(navi){
		free(first);
		first=navi;
		navi=navi->next;
	}
	if(first){
		free(first);
	}
	if(navi){
		free(navi);
	}
}

int main(int argc, char const *argv[])
{
	/*Node arrayList[size];
	int u=0;
	for(u=0;u<size;u+=1){
		arrayList[u].value=0;
		arrayList[u].next=NULL;
	}
	*/
	Node * first=NULL;

	int inputs=10; //does not matter tbh
	char inputArrayList[inputs];

	while(1){
		//printf(" once more %s\n", inputArrayList);
		int target=0;
		if(scanf("%s",&inputArrayList[0])!=EOF){
		//printf(" twice more %s\n", inputArrayList);
		if(inputArrayList[0]=='d'){
			scanf("%i", &target);
			first=delete(first,target);
			printList(first);
		}else if(inputArrayList[0]=='i'){
			//printf("%s\n", "insertion ;)");
			scanf("%i", &target);
			Node * zelda=(Node *) malloc(sizeof(Node));
			zelda->value=target;
			zelda->next=NULL;
			/*
			arrayList[indexList].value=target;
			arrayList[indexList].next=NULL;
			*/
			first=insert(first,zelda);
			printList(first);
		}else{
			clean(first);
			return(0);
		}
	}else{
		clean(first);
		return 0;
	}
	}
	clean(first);
	//printf("%s\n", "rawr");
	/* code */
	return 0;
}