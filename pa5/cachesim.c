#include <stdio.h>
#include <stdlib.h>

typedef struct cacheLine
{
	int alive; //alive =1 once something has been written to the cache line, otherwise 0
	long int tag; //the tag associated with this cache
}cacheLine;

typedef struct cacheSet
{
	//int size; //quantity of cachelines
	int nextOpenLine; //starts out as 0, next avvailable cache, increments up  by 1
	int * scores; //every time we do a thing if its fifo, we add +1 to every cache in score; if its 
	//lru we set the target line to 0 and +1 everything else. whenever the set is full, we replace the one with the highest score
	cacheLine** cLines;
}cacheSet;

int powerTwo(int n);
int getSetSize(const char*a);
cacheSet** makeSets(int setCount,int setSize);
int printcLine(cacheLine*cLine);
void printSet(cacheSet*cSet,int setSize);
void printWholeCache(cacheSet**sets,int setCount,int setSize);
void insertTagBrute(cacheSet*cSet,long int tag,int index,int fifo,int count);
void insertTagSmart(cacheSet*cSet,int setSize,long int tag,int fifo,int count);
int getHit(cacheSet*cSet,int setSize,long int tag,int fifo,int pre,int count);

int main(int argc, char const *argv[])
{

	if(argc!=7){
		return 0;
	}
	int csize=atoi(argv[1]);
	//printf("csize =%d\n", csize);
	//printf("assoc =%d\n", assoc);
	int pref;
	pref=argv[3][1]-'0';
	//printf("pref= %d\n", pref);
	//int rep=atoi(argv[4]);
	//printf("rep = %d\n", rep);
	int bsize=atoi(argv[5]);
	int blog=-1;
	while(bsize>0){
		blog++;
		bsize=bsize>>1;
	}
	bsize=atoi(argv[5]);
	//printf("bsize= %d\n", bsize);
	FILE*fp=fopen(argv[6],"r+");

	int fifo=1;
	if(argv[4][0]!='f'){
		fifo=0;
	}
	//printf("fifo is %d\n",fifo);

	int lines=csize/bsize;
	int setSize;
	if(argv[2][0]=='d'){
		setSize=1;
	}else if(getSetSize(argv[2])==-1){
		setSize=lines;
	}else{
		setSize=getSetSize(argv[2]);
	}

	if(powerTwo(bsize)!=1||powerTwo(csize)!=1||powerTwo(setSize)!=1){
		return 0;
	}

	//printf("setSize = %d\n", setSize);
	int setCount=lines/setSize;
	int dumbSize=setCount;
	int slog=-1;
	while(dumbSize>0){
		slog++;
		dumbSize=dumbSize>>1;
	}
	//printf("setCount= %d\n", setCount);
	//printf("lines = %d \n", lines);

	//printf("there are %d lines each which has 2^%d= %d bits, and 2^%d=%d sets of size %d, \n", lines,blog,bsize,slog,setCount,setSize);

	cacheSet**sets=makeSets(setCount,setSize);
	cacheSet**setsPre=makeSets(setCount,setSize);

	char*str=(char*)malloc(64*sizeof(char));

	int misses=0;
	int hits=0;
	int reads=0;
	int writes=0;

	int count=0;

	while(fgets(str,64,fp)!=NULL){
		if(str[0]=='#'){
			break;
		}
		//printf("str is %s and count is %d\n",str,count);
		count++;
		/*
		if(fifo==1){
			int k;
			for(k=0;k<setCount;k++){
				printf("k= %d\n", k);
				int d;
				printf("sets[k]->nextOpenLine %d\n",sets[k]->nextOpenLine);
				for(d=0;d<sets[k]->nextOpenLine;d++){
					printf("d=%d\n", d);
					sets[k]->cLines[d]+=1;
					printf("yeeha\n");
				}
				printf("oof\n");
			}
			printf("more oof\n");
		}
		printf("even more oof\n");
		*/
		char *pc=(char *)malloc(32*sizeof(char));
		char*op=(char *)malloc(2*sizeof(char));
		long int tag;
		sscanf(str,"%s %s %lx",pc, op,&tag);
		long int tagPlus=tag+bsize;
		//printf("tag is %lx+%d=%lx \n", tag,blog,tagPlus);
		//printf("tag is %lx\n", tag);
		long int setTarget=(tag>>blog)&(setCount-1);
		long int setTargetPlus=(tagPlus>>blog)&(setCount-1);
		//printf("setTarget is %lx setTargetPlus is %lx\n", setTarget,setTargetPlus);
		tag=tag>>(blog+slog);
		tagPlus=tagPlus>>(blog+slog);
		//printf("tag is %lx tagPlus is %lx\n", tag,tagPlus);
		//(setCount-1)&tag;
		if(op[0]=='W'){
			writes++;
		}
		if(pref==0){
		if(getHit(sets[setTarget],setSize,tag,fifo,1,count)==-1){
			misses++;
			insertTagSmart(sets[setTarget],setSize,tag,fifo,count);
			reads++;
		}else{
			hits++;
		}
		}
		if(pref==1){
			if(getHit(setsPre[setTarget],setSize,tag,fifo,0,count)==-1){
				insertTagSmart(setsPre[setTarget],setSize,tag,fifo,count);
				reads++;
				misses++;
				count++;
				if(getHit(setsPre[setTargetPlus],setSize,tagPlus,fifo,1,count)==-1){
					insertTagSmart(setsPre[setTargetPlus],setSize,tagPlus,fifo,count);
					reads++;
				}
				}else{
				hits++;
			}
		}
		/*
		//pref=1;
		if(pref==1){
		if(getHit(setsPre[setTarget],setSize,tag,fifo,0)==-1){
			misses++;
			insertTagSmart(setsPre[setTarget],setSize,tag,fifo,count);
			long int tagPlus=tag+bsize;
			if(getHit(setsPre[setTarget],setSize,tagPlus,fifo,1)==-1){
				insertTagSmart(setsPre[setTarget],setSize,tagPlus,fifo,count);
			}
		}else{
			hits++;
		}
	}*/
	}
	//printWholeCache(sets,setCount,setSize);
	//printWholeCache(setsPre,setCount,setSize);

	printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n", reads,writes,hits,misses);
	
	return 0;
}

int getSetSize(const char*a){
	int y=0;
	while(a[y]!='\0'&&a[y]!='\n'&&a[y]!=EOF){
		y++;
	}
	if(y<7){
		return -1;
	}
	y=6;
	char*num=(char*)malloc(32*sizeof(char));
	while(a[y]!='\0'&&a[y]!='\n'&&a[y]!=EOF){
		num[y-6]=a[y];
		y++;
	}
	num[y]='\0';
	int out=atoi(num);
	free(num);
	return(out);
}

int powerTwo(int n){
	//returns 1 is power of 2, otherwise returns 0;
	if(n==0){
		return 1;
	}
	int y=0;
	while(y<16){
		if(n==(1<<y)){
			return 1;
		}
		y++;
	}
	return 0;
}

cacheSet** makeSets(int setCount,int setSize){
	cacheSet**sets=(cacheSet**)malloc(setCount*sizeof(cacheSet*));
	int h=0;
	for(h=0;h<setCount;h++){
		sets[h]=(cacheSet*)malloc(sizeof(cacheSet));
		sets[h]->nextOpenLine=0;
		sets[h]->scores=(int*)malloc(setSize*sizeof(int));
		sets[h]->cLines=(cacheLine**)malloc(setSize*sizeof(cacheLine*));
		int u=0;
		for(u=0;u<setSize;u++){
			sets[h]->cLines[u]=(cacheLine*)malloc(sizeof(cacheLine));
			sets[h]->cLines[u]->alive=0;
		}
	}
	return sets;
}

int printcLine(cacheLine*cLine){
	if(cLine->alive==0){
		printf("\t|---dead" );
		return 0;
	}else{
		printf("\t|---my tag is %lx ", cLine->tag);
		return 1;
	}
}

void printSet(cacheSet*cSet,int setSize){
	int j=0;
	for(j=0;j<setSize;j++){
		if(printcLine(cSet->cLines[j])==1){
			printf(" score is %d\n",cSet->scores[j]);
		}else{
			printf("\n");
		}
	}
}

void printWholeCache(cacheSet**sets,int setCount,int setSize){
	int s=0;
	for(s=0;s<setCount;s++){
		printf("|--- set %d\n", s);
		printSet(sets[s],setSize);
	}
}

void insertTagSmart(cacheSet*cSet,int setSize,long int tag,int fifo,int count){
	int index=0;
	if(cSet->nextOpenLine<setSize){
		index=cSet->nextOpenLine;
		cSet->nextOpenLine++;
	}else{
		int min=cSet->scores[index];
		int r=0;
		for(r=0;r<setSize;r++){
			//printf("cSet->scores[r]>max %d > %d \n", cSet->scores[r], max);
			if(cSet->scores[r]<min){
				min=cSet->scores[r];
				index=r;
			}
		}
	}
	//printf("to be replaced is %d\n", index);
	insertTagBrute(cSet,tag,index,fifo,count);
}

void insertTagBrute(cacheSet*cSet,long int tag,int index,int fifo,int count){
	cSet->cLines[index]->alive=1;
	cSet->cLines[index]->tag=tag;
	cSet->scores[index]=count;
}

int getHit(cacheSet*cSet,int setSize,long int tag,int fifo,int pre,int count){
	int u=0;
	for(u=0;u<cSet->nextOpenLine;u++){
		if(tag==cSet->cLines[u]->tag){
			if(fifo!=1&&pre==0){ //if pre==1 were prefetching and thus we don't need to change any scores i THINK
				cSet->scores[u]=count;
			}
			/*else if(fifo==1){
				cSet->scores[u]=-1;
			}*/
			return u;
		}
	}
	return -1;
}