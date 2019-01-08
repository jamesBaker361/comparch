#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodeStruct
{
	int gateType;
	int inputQuant;
	int *inputs;
	int outputQuant;
	int * outputs;
	int finished; //if 1, then all the outputs have been did, and this node serves no purpose
	int n; //this only for multiplexors bu its the n
	struct nodeStruct * next;
}Node;

int* addOneArr(int*input,int length);
int getColonX(char*line,int size,int x);
int getFileSize(FILE*f);
int** outputArr(int inputQuant,int outputQuant);
int getInputQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant);
int getOutputQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant);
int getTempQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant,Node*head,int lineCount); //gets quantity of temp variables
void solve(int *arr,int inputs,int outputs); //for a particular set of inputs, solves the thing
void tryAll(int *arr,int start, int inputs,int outputs,FILE*wri);
int fillVarIO(int varQuant, int end,char*line,int*varVals,char**varNames,int * varNameLength,int value,int size);
void printVals(int*varVals,char**varNames,int * varNameLength,int varQuant);
int getIndex(char**varNames,int varQuant,int * varNameLength,char*query,int queryLength); //returns i where varNames[i]=query, i<varQuant, else -1
int getGate(char*gateQuery,int queryLength); //returns the corresponding int that represents what kind of gate (Or, multiplexer,etc) this is 
int tempsFromLine(char*line,int*varVals,char**varNames,int * varNameLength,int varQuant,Node*head,int size);
int getLineInputs(char*line,Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant,int size); //takes a line and the corresponding node and fills the head->inputs
int getLineOutputs(char*line,Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant,int size,int index); //takes a line and the corresponding node and fills the head->outputs
void printNode(Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant);
int getVarIndexFromLine(int size,char**varNames,int varQuant,int * varNameLength,char*line,int index,int*target);
int getTemps();
int getNFromLine(int index,char*line,int size);
int getVarValue(int*varVals,char**varNames,int *varNameLength,int varQuant,char*query,int queryLength); //gets var value, or -1 if not found
int nodeSolve(int size,char**varNames,int*varVals,int varQuant,int * varNameLength,Node*head);
int** inputArr(int inputQuant);
void bigSolve(int size,char**varNames,int varQuant,int*varVals,int * varNameLength,int inputQuant,int outputQuant,Node*root,int nodeQuant,FILE*fil);
char *correct(char*line,int size);
void butt();

int main(int argc, char const *argv[])
{
	FILE*f;
	FILE*wri;
	if(argc==1){
		f=stdin;
	}else{
		f=fopen(argv[1],"r+");
	}
	if(argc==3){
		wri=fopen(argv[2],"w+");
	}else{
		wri=stdout;
	}
	int size=getFileSize(f);
	int lineCount=0;
	char** lines =(char**) malloc(size*sizeof(char*));
	int j=0;	
	for(j=0;j<size;j++){
		lines[j]=(char*) malloc(size*(sizeof(char)));
	}
	while(fgets(lines[lineCount],size,f)!=NULL){
		lineCount++;
	}
	for(j=lineCount;j<size;j++){
		free(lines[j]);
	}
	int s=0;
	for(s=0;s<lineCount;s++){
		//printf("%s\n", lines[s]);
		lines[s]=correct(lines[s],size);
		//printf("%s\n", lines[s]);
	}
	char ** varNames=(char**)malloc(size*sizeof(char*));
	int * varNameLength=(int*)malloc(size*sizeof(int));
	int * varVals=(int*)malloc(size*sizeof(int));

	int varQuant=0;
	int inputs=getInputQuant(lines,size,varNames,varVals,varNameLength,varQuant);
	varQuant+=inputs;
	int outputs=getOutputQuant(lines,size,varNames,varVals,varNameLength,varQuant);
	varQuant+=outputs;
	Node* head=(Node*)malloc(sizeof(Node));
	head->finished=0;
	head->gateType=-1;
	Node* root=head;
	int z= getTempQuant(lines,size,varNames,varVals,varNameLength,varQuant,head,lineCount);
	varQuant+=z;
	int nodeQuant=lineCount-2;
	//printf("z=  %d\n",z);

	while(head->next!=NULL){
		//printNode(head,varNames,varVals,varNameLength,varQuant);
		head=head->next;
	}
	//printVals(varVals,varNames,varNameLength,varQuant);
	bigSolve(size,varNames,varQuant,varVals,varNameLength,inputs,outputs,root,nodeQuant,wri);


	//printVals(varVals,varNames,varNameLength,varQuant);

	//int*arr=(int*)malloc(0);
	//tryAll(arr,0,inputs,outputs,wri);
	
	//cleanup
	//free(arr);
	int fr=0;
	while(fr<lineCount){
		free(lines[fr]);
		fr++;
	}
	free(lines);
	return 0;
}

int getFileSize(FILE*f){
	fseek(f, 0, SEEK_END); // seek to end of file
	int size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET); // seek back to beginning of file
	// proceed with allocating memory and reading the file
	return size;
}

void butt(){
	printf("%s\n", "buttz");
}

int getInputQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant){
	char *line=lines[0];
	int c=5;
	int start=-1;
	int end=-1;
	while(c<size){
		if(start==-1&&line[c]!=' '){
			start=c;
		}else if(start!=-1&&line[c]==' '){
			break;
		}
		c++;
	}
	end=c;
	int digits=end-start;
	char * strQuant=(char *)malloc(digits *sizeof(char));
	int k=start;
	for(k=start;k<end;k++){
		strQuant[k-start]=line[k];
	}
	int value;
	sscanf(strQuant, "%d", &value);
	free(strQuant);
	
	fillVarIO(varQuant,end,line,varVals,varNames, varNameLength, value,size);
	//int fillVarIO(int varQuant, int end,char*line,int*varVals,char**varNames,int * varNameLength,int value,int size);

	
	return value;
}
void printVals(int*varVals,char**varNames,int * varNameLength,int varQuant){
	int q=0;
	for(q=0;q<varQuant;q++){
		printf("%s = ", varNames[q]);
		printf("%d lengt = %d\n", varVals[q],varNameLength[q]);
	}
}

int getIndex(char**varNames,int varQuant,int * varNameLength,  char*query,int queryLength){
	int k=0;
	//printf("lenth of %s is %d\n",query, queryLength);
	for(k=0;k<varQuant;k++){
		//printf("%s and %s\n", varNames[k],query);
		if(queryLength!=varNameLength[k]){
			//printf("%s != %s\n", varNames[k],query);
			//printf("queryLength!=varNameLength[k] %d!=%d \n",queryLength, varNameLength[k]);
			continue;
		}else if(strncmp(query,varNames[k],queryLength)==0){
			//printf("%s = %s\n", query,varNames[k]);
			//printf("k is %d\n", k);
			return k;
		}else{
			//printf("%s != %s\n", varNames[k],query);
		}
	}
	//printf("%s is a bitch\n", query);
	return -1;
}

int fillVarIO(int varQuant, int end,char*line,int*varVals,char**varNames,int * varNameLength,int value,int size){
	int x=0;
	/*for(x=varQuant;x<varQuant+value;x++){
		varVals[x]=-1;
	}*/
	x=varQuant;
	int h=end;
	int nameFound=0;
	//char*name=(char*)malloc(size*sizeof(char));
	int nameCount=0;
	while(h<size&&line[h]!=EOF&&line[h]!='\n'&&line[h]!='\0'){
		if(line[h]!=' '&&line[h]!=':'){
			nameFound=1;
			//name[nameCount]=line[h];
			nameCount+=1;
		}else if((line[h]==' '||line[h]==EOF||line[h]=='\n'||line[h]=='\0')&&nameFound==1){
			nameFound=0;
			varNameLength[x]=nameCount;
			varNames[x]=(char *)malloc((nameCount)*sizeof(char));
			int a=0;
			for(a=0;a<nameCount;a+=1){
				varNames[x][a]=line[a+h-nameCount];
			}
			x++;
			nameCount=0;
			//name=(char*)malloc(size*sizeof(char));
		}
		h++;
	}
	if(x<varQuant+value&&nameCount>0){
		varNameLength[x]=nameCount;
		varNames[x]=(char *)malloc((nameCount)*sizeof(char));
		int a=0;
		for(a=0;a<nameCount;a+=1){
			varNames[x][a]=line[a+h-nameCount];
		}
		//printf("%s\n", varNames[x]);
	}
	return value;
}

int getOutputQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant){
	char *line=lines[1];
	int c=6;
	int start=-1;
	int end=-1;
	while(c<size){
		if(start==-1&&line[c]!=' '){
			start=c;
		}else if(start!=-1&&line[c]==' '){
			break;
		}
		c++;
	}
	end=c;
	int digits=end-start;
	char * strQuant=(char *)malloc(digits *sizeof(char));
	int k=start;
	for(k=start;k<end;k++){
		strQuant[k-start]=line[k];
	}
	int value;
	sscanf(strQuant, "%d", &value);
	free(strQuant);
	fillVarIO(varQuant,end,line,varVals,varNames,varNameLength,value,size);
	return value;
}

int getTempQuant(char** lines,int size,char**varNames,int*varVals,int * varNameLength,int varQuant,Node*head,int lineCount){
	int out=0;
	int h=2; //first two lines are just input output
	//Node*clone=head;
	for(h=2;h<lineCount;h+=1){
		//printf("%s\n", lines[h]);
		out+=tempsFromLine(lines[h],varVals,varNames,varNameLength,varQuant+out,head,size);
		int index= getLineInputs(lines[h],head,varNames,varVals,varNameLength,varQuant+out,size);
		int q=getLineOutputs(lines[h],head,varNames,varVals,varNameLength,varQuant+out,size,index);
		if(q==-10){
			return 0;
		}
		head->next=(Node *)malloc(sizeof(Node));
		head->next->finished=0;
		head=head->next;
	}
	return out;
}

void solve(int *arr,int inputs,int outputs){
	//int length=inputs+outputs;
	int count=0;
	int a=0;
	while(a<inputs){
		count+=arr[a];
		a++;
	}
	int b=0;
	while(b<outputs){
		arr[b+inputs]=count++;
		b++;
	}
}

int getLineInputs(char*line,Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant,int size){
	int index=0;
	if(head->gateType==2||head->gateType==10){
		head->inputQuant=1;
		head->inputs=(int*)malloc(sizeof(int));
		//2-NOT i : o
		if(head->gateType==2){
			index=3;
		}
		//10-PASS i : o
		if(head->gateType==10){
			index=4;
		}
		index=getVarIndexFromLine(size,varNames,varQuant,varNameLength,line,index,head->inputs);
	}else if(head->gateType==8||head->gateType==9){
		//8-DECODER n : i1 · · ·in : o0 · · · o2n−1
		if(head->gateType==8){
			index=7;
		}
		//9-MULTIPLEXER n : i1 · · ·i2^n : s1 ... sn : i
		if(head->gateType==9){
			index=11;
		}
		int value=getNFromLine(index,line,size);
		head->n=value;
		if(head->gateType==9){
			value+=(1 << value);
		}
		index=getColonX(line,size,1);
		head->inputQuant=value;
		head->inputs=(int*)malloc(value*sizeof(int));
		int a=0;
		while(a<value){
			index=getVarIndexFromLine(size,varNames,varQuant,varNameLength,line,index,head->inputs+a);
			a+=1;
		}
	}else{
		head->inputQuant=2;
		head->inputs=(int*)malloc(2*sizeof(int));
		//3-AND i1 i2 : o
		//6-NOR i1 i2 : o
		//7-XOR i1 i2 : o
		if(head->gateType==3||head->gateType==6||head->gateType==7){
			index=3;
		}
		//4-OR i1 i2 : o
		if(head->gateType==4){
			index=2;
		}
		//5-NAND i1 i2 : o
		if(head->gateType==5){
			index=4;
		}
		int a=0;
		while(a<2){
			index=getVarIndexFromLine(size,varNames,varQuant,varNameLength,line,index,head->inputs+a);
			a+=1;
		}
	}
	return index;
}

int getLineOutputs(char*line,Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant,int size,int index){
	index=-1;
	if(head->gateType==8){
		//8-DECODER n : i1 · · ·in : o1 · · · o2^n
		index=7;
		int value=getNFromLine(index,line,size);
		value=(1 << value);
		index=getColonX(line,size,2);
		head->outputQuant=value;
		head->outputs=(int*)malloc(value*sizeof(int));
		int a=0;
		while(a<value){
			index=getVarIndexFromLine(size,varNames,varQuant,varNameLength,line,index,head->outputs+a);
			a+=1;
		}
		//int n=-1;
	}else{
		head->outputQuant=1;
		head->outputs=(int*)malloc(sizeof(int));
		//9-MULTIPLEXER n : i1 · · ·i2^n : s1 ... sn : i/
		if(head->gateType==9){
			index=getColonX(line,size,3);
		}else{
			index=getColonX(line,size,1);
		}
		/*	2-NOT i : o
		3-AND i1 i2 : o
		4-OR i1 i2 : o
		5-NAND i1 i2 : o
		6-NOR i1 i2 : o
		7-XOR i1 i2 : o
		10-PASS i : o*/
		//printf("index = %d\n", index);
		index=getVarIndexFromLine(size,varNames,varQuant,varNameLength,line,index,head->outputs);
		
	}
	return 1;
}

int getGate(char*gateQuery,int queryLength){
	/*
	0-INPUT n : i1 · · ·in
	1-OUTPUT n : o1 · · · on
	2-NOT i : o
	3-AND i1 i2 : o
	4-OR i1 i2 : o
	5-NAND i1 i2 : o
	6-NOR i1 i2 : o
	7-XOR i1 i2 : o
	8-DECODER n : i1 · · ·in : o0 · · · o2n−1
	9-MULTIPLEXER n : i1 · · ·i2^n : s0 ... sn : o
	10-PASS i : o
	*/
	if(queryLength==5){
		return 0;
	}else if(queryLength==6){
		return 1;
	}else if(queryLength==2){
		return 4;
	}else if(queryLength==7){
		return 8;
	}else if(queryLength==11){
		return 9;
	}else if(queryLength==4){
		//NAND-5
		//PASS-10
		if(gateQuery[0]=='N'){
			return 5;
		}else if(gateQuery[0]=='P'){
			return 10;
		}
	}else if(queryLength==3){
		//XOR-7
		// NOR-6
		//AND-3
		//NOT-2
		if(gateQuery[0]=='X'){
			return 7;
		}else if(gateQuery[2]=='R'){
			return 6;
		}else if(gateQuery[2]=='D'){
			return 3;
		}else if(gateQuery[2]=='T'){
			return 2;
		}

	}else{
		return -1;
	}
	return -1;
}

int tempsFromLine(char*line,int*varVals,char**varNames,int * varNameLength,int varQuant,Node*head,int size){
	if(head==NULL){
		head=(Node*)malloc(sizeof(Node));
		//head->gateType=-1;
	}
	int out=0;
	int start=0;
	for(start=0;start<size;start+=1){
		if(line[start]==' '){
			break;
		}
	}
	char*gateQuery=(char *)malloc(start*sizeof(char));
	int s=0;
	for(s=0;s<start;s+=1){
		gateQuery[s]=line[s];
	}
	int queryLength=start;
	head->gateType=getGate(gateQuery,queryLength);

	int word=0;
	int wstart=-1;
	int wend=-1;
	int c=start;
	while(c<size&&line[c]!=EOF&&line[c]!='\n'&&line[c]!='\0'){
		if(word==0&&line[c]!=' '&&line[c]!=':'){
			word=1;
			wstart=c;
		}else if(word==1&&(line[c]==EOF||line[c]=='\n'||line[c]=='\0'||line[c]==' '||line[c]==':')){
			word=0;
			wend=c;
			int length=wend-wstart;
			//char*query=(char *)malloc(length*sizeof(char));
			char query[length];
			int q=0;
			for(q=0;q<length;q++){
				query[q]=line[q+wstart];
			}
			//printf("%s\n", query);
			if(getIndex(varNames,varQuant+out,varNameLength,query,length)==-1){
				//strcpy(varNames[varQuant+out], &query);
				//strcpy(varNames[varQuant+out],query);
				varNames[varQuant+out]=(char *)malloc(length*sizeof(char));
				int k=0;
				for(k=0;k<length;k++){
					varNames[varQuant+out][k]=query[k];
				}
				//printf("%s\n", varNames[varQuant+out]);
				varVals[varQuant+out]=-1;
				varNameLength[varQuant+out]=length;
				//printf("we have added %s\n", varNames[varQuant+out]);
				out+=1;
			}else{
				//printf("getIndex(varNames,varQuant+out,varNameLength,query,length)= %d\n",getIndex(varNames,varQuant+out,varNameLength,query,length));
			}
			wstart=-1;
			wend=-1;
		}
		c+=1;
	}
	if(word==1&&wend==-1&&wstart!=-1){
		int k=0;
		for(k=wstart;k<size;k++){
			if(line[k]==EOF||line[k]=='\n'||line[k]==' '||line[k]==':'||line[k]=='\0'){
				break;
			}
		}
		wend=k;
		//printf("wend- %d wstart %d\n", wend,wstart);
			int length=wend-wstart;
			//char*query=(char *)malloc(length*sizeof(char));
			char query[length];
			int q=0;
			for(q=0;q<length;q++){
				query[q]=line[q+wstart];
			}
			//printf("silly boy%s\n", query);
			if(getIndex(varNames,varQuant+out,varNameLength,query,length)==-1){;
				//strcpy(varNames[varQuant+out], &query);
				//strcpy(varNames[varQuant+out],query);
				varNames[varQuant+out]=(char *)malloc(length*sizeof(char));
				int k=0;
				for(k=0;k<length;k++){
					varNames[varQuant+out][k]=query[k];
				}
				//printf("%s\n", varNames[varQuant+out]);
				varVals[varQuant+out]=-1;
				varNameLength[varQuant+out]=length;
				out+=1;
			}
	}

	free(gateQuery);
	return out;
}

int getVarValue(int*varVals,char**varNames,int * varNameLength,int varQuant,char*query,int queryLength){
	int h=getIndex(varNames,varQuant,varNameLength,query,queryLength);
	if(h==-1){
		return -1;
	}else{
		return(varVals[h]);
	}
	return -1;
}

void printNode(Node*head,char**varNames,int*varVals,int * varNameLength,int varQuant){
	printf("gateType is ");
	if(head->gateType==0){
		printf("%s\n", "INPUT");
	}else if(head->gateType==1){
		printf("%s\n", "OUTPUT");
	}else if(head->gateType==2){
		printf("%s\n", "NOT");
	}else if(head->gateType==3){
		printf("%s\n", "AND");
	}else if(head->gateType==4){
		printf("%s\n", "OR");
	}else if(head->gateType==5){
		printf("%s\n", "NAND");
	}else if(head->gateType==6){
		printf("%s\n", "NOR");
	}else if(head->gateType==7){
		printf("%s\n", "XOR");
	}else if(head->gateType==8){
		printf("%s\n", "DECODER");
	}else if(head->gateType==9){
		printf("%s\n", "MULTIPLEXER");
	}else if(head->gateType==10){
		printf("%s\n", "PASS");
	}
	printf("my %d inputs are ", head->inputQuant);
	int h=0;
	for(h=0;h<head->inputQuant;h++){
		printf(" %s =%d", varNames[head->inputs[h]],head->inputs[h]);
	}
	printf("my %d outputs are ", head->outputQuant);
	for(h=0;h<head->outputQuant;h++){
		printf(" %s = %d ", varNames[head->outputs[h]],head->outputs[h]);
	}
	printf("\n");
}

void tryAll(int *arr,int start, int inputs,int outputs,FILE*wri){
	if(start==inputs){
		int*arrPlus=(int *)malloc((inputs+outputs)*sizeof(int));
		int b=0;
		for(b=0;b<inputs;b++){
			arrPlus[b]=arr[b];
		}
		arr=arrPlus;
		solve(arr,inputs,outputs);
		int i=0;
		for(i=0;i<inputs;i++){
			fprintf(wri, "%d ", arr[i]);
		}
		fprintf(wri, "|");
		for(i=inputs;i<inputs+outputs;i++){
			fprintf(wri, " %d", arr[i]);
		}
		fprintf(wri, "\n");
		free(arrPlus);
	}else{
		int* out0=(int*)malloc((start+1)*sizeof(int));
		int* out1=(int*)malloc((start+1)*sizeof(int));
		int b=0;
		for(b=0;b<start;b++){
			out0[b]=arr[b];
			out1[b]=arr[b];
		}
		out0[start]=0;
		tryAll(out0,start+1,inputs,outputs,wri);
		free(out0);
		out1[start]=1;
		tryAll(out1,start+1,inputs,outputs,wri);
		free(out1);
	}
}

int** inputArr(int inputQuant){
	int combos=(1<<inputQuant);
	int ** out=(int **)malloc(combos*sizeof(int*));
	int u=0;
	for(u=0;u<combos;u++){
		out[u]=(int *)malloc(inputQuant*sizeof(int));
	}
	int flip=combos;
	int z=0;
	for(z=0;z<inputQuant;z++){
		flip=flip/2;
		int digit=0;
		int b=0;
		for(b=combos-1;b>-1;b--){
			if(b%flip==0){
				if(digit==1){
					digit=0;
				}else{
					digit=1;
				}
			}
			out[b][z]=digit;
		}
	}
	return out;
}

int* addOneArr(int*input,int length){
	int x;
	for(x=length-1;x>-1;x--){
		if(input[x]==1){
			input[x]=0;
		}else if(input[x]==0){
			input[x]=1;
			break;
		}
	}
	return input;
}

int** outputArr(int inputQuant,int outputQuant){
	int combos=(1<<inputQuant);
	int ** out=(int **)malloc(combos*sizeof(int*));
	int u=0;
	for(u=0;u<combos;u++){
		out[u]=(int *)malloc(outputQuant*sizeof(int));
	}
	return out;
}

void bigSolve(int size,char**varNames,int varQuant,int*varVals,int * varNameLength,int inputQuant,int outputQuant,Node*root,int nodeQuant,FILE*fil){
	//int** inputCombos=inputArr(inputQuant);
	//int ** outputCombos=outputArr(inputQuant,outputQuant);
	int combos=(1<<inputQuant);
	int*inputTest=(int *)malloc(inputQuant*sizeof(int));
	int*outputTest=(int*)malloc(outputQuant*sizeof(int));
	int p;
	for(p=0;p<inputQuant;p++){
		inputTest[p]=1;
	}
	int h=0;
	for(h=0;h<combos;h++){
		//int*inputTest=(int *)malloc(inputQuant*sizeof(int));
		inputTest=addOneArr(inputTest,inputQuant);
		Node*head=root;
		int finishedQuant=0;
		while(head->next!=NULL){
			head->finished=0;
			head=head->next;
		}
		int f=0;
		for(f=0;f<inputQuant;f++){
			varVals[f]=inputTest[f];
		}
		int b=0;
		for(b=inputQuant;b<varQuant;b++){
			varVals[b]=-1;
			if(varNameLength[b]==1){
				if(varNames[b][0]=='0'){
					varVals[b]=0;
				}else if(varNames[b][0]=='1'){
					varVals[b]=1;
				}
			}
		}
		int m=0;
		int k;
		for(k=inputQuant+outputQuant;k<varQuant;k++){
			varVals[k]=-1;
		}
		int zero=getIndex(varNames,varQuant,varNameLength,"0",1);
		if(zero!=-1){
			varVals[zero]=0;
		}
		int one=getIndex(varNames,varQuant,varNameLength,"1",1);
		if(one!=-1){
			varVals[one]=1;
		}

		//int bro=0;
		while(finishedQuant<nodeQuant){
			//printf("its everyday br%d\n", bro++);
			head=root;
			while(head->next!=NULL){
				if(nodeSolve(size,varNames,varVals,varQuant,varNameLength,head)==1){
					finishedQuant+=1;
					//printNode(head,varNames,varVals,varNameLength,varQuant);
				}
				head=head->next;
			}
		}
		for(m=0;m<outputQuant;m++){
			outputTest[m]=varVals[m+inputQuant];
		}
		for(f=0;f<inputQuant;f++){
			fprintf(fil,"%d ",inputTest[f]);
		}
			fprintf(fil,"|");
		for(m=0;m<outputQuant;m++){
			fprintf(fil," %d",outputTest[m]);
		}
		fprintf(fil,"\n");
		//free(inputTest);
		//free(outputTest);
	}
}

int getVarIndexFromLine(int size,char**varNames,int varQuant,int * varNameLength,char*line,int index,int*target){
	int y=0;
	int word=0;
	int wend=-1;
	int wstart=-1;
	for(y=index;y<size;y++){
		if(word==0&&line[y]!=EOF&&line[y]!='\n'&&line[y]!=' '&&line[y]!=':'){
			word=1;
			wstart=y;
		}else if(word==1&&(line[y]==EOF||line[y]=='\n'||line[y]==' '||line[y]==':'||line[y]=='\0')){
			word=0;
			wend=y;
			break;
		}
	}
	wend=y;
	int length=wend-wstart;
	char*query=(char*)malloc(length*sizeof(char));
	int q=0;
	for(q=0;q<length;q++){
		query[q]=line[q+wstart];
	}
	//printf("query is %s and length is %d\n", query,length);
	int varIndex=getIndex(varNames,varQuant,varNameLength,query,length);
	if(varIndex==-1){
		//printf("couldnt find %s\n", query);
	}
	*target=varIndex;
	return wend;
}

int getNFromLine(int index,char*line,int size){
	int c=index;
	int start=-1;
	int end=-1;
	while(c<size){
		if(start==-1&&line[c]!=' '){
			start=c;
		}else if(start!=-1&&line[c]==' '){
			break;
		}
		c++;
	}
	end=c;
	int digits=end-start;
	char * strQuant=(char *)malloc(digits *sizeof(char));
	int k=start;
	for(k=start;k<end;k++){
		strQuant[k-start]=line[k];
	}
	int value;
	sscanf(strQuant, "%d", &value);
	free(strQuant);
	return value;
}

int getColonX(char*line,int size,int x){
	int c;
	int k;
	if(x<=1){
		k=0;
	}else{
		k=1+getColonX(line,size,x-1);
	}
	for(c=k;c<size;c++){
		if(line[c]==':'){
			return c;
		}
	} 
	return -1;
}

int nodeSolve(int size,char**varNames,int*varVals,int varQuant,int * varNameLength,Node*head){
	if(head->finished==1){ //returns 0 if already finished, -1 if it CANT be finished
		return 0;
	}
	int k=0;
	for(k=0;k<head->inputQuant;k++){
		if(varVals[head->inputs[k]]==-1){
			return -1;
		}
	}
	//test whether this node has been finished or not
	//test whether inputs are all valid
	//do whatever logic to get output 
	//update whatever variables need to be updated
	//set visited =1
		
	//0-INPUT n : i1 · · ·in
	//1-OUTPUT n : o1 · · · on
	//2-NOT i : o
	if(head->gateType==2){
		if(varVals[head->inputs[0]]==1){
			varVals[head->outputs[0]]=0;
		}
		if(varVals[head->inputs[0]]==0){
			varVals[head->outputs[0]]=1;
		}
	}else if(head->gateType==3){ 	//3-AND i1 i2 : o
		if(varVals[head->inputs[0]]==1&&varVals[head->inputs[1]]==1){
			varVals[head->outputs[0]]=1;
		}else{
			varVals[head->outputs[0]]=0;
		}
	}else if(head->gateType==4){ //4-OR i1 i2 : o
		if(varVals[head->inputs[0]]==0&&varVals[head->inputs[1]]==0){
			varVals[head->outputs[0]]=0;
		}else{
			varVals[head->outputs[0]]=1;
		}
	}else if(head->gateType==5){ //5-NAND i1 i2 : o
		if(varVals[head->inputs[0]]==1&&varVals[head->inputs[1]]==1){
			varVals[head->outputs[0]]=0;
		}else{
			varVals[head->outputs[0]]=1;
		}
	}else if(head->gateType==6){	//6-NOR i1 i2 : o
		if(varVals[head->inputs[0]]==0&&varVals[head->inputs[1]]==0){
			varVals[head->outputs[0]]=1;
		}else{
			varVals[head->outputs[0]]=0;
		}
	}else if(head->gateType==7){ //7-XOR i1 i2 : o
		if(varVals[head->inputs[0]]!=varVals[head->inputs[1]]){
			varVals[head->outputs[0]]=1;
		}else{
			varVals[head->outputs[0]]=0;
		}
	}else if(head->gateType==8){	//8-DECODER n : i1 · · ·in : o0 · · · o2^n−1
		int outBoi=0;
		int dd=0;
		for(dd=0;dd<head->inputQuant;dd++){
			outBoi+=(1<<dd)*varVals[head->inputs[head->inputQuant-(dd+1)]];
		}
		int yy=0;
		for(yy=0;yy<head->outputQuant;yy++){
			if(yy==outBoi){
				varVals[head->outputs[yy]]=1;
			}else{
				varVals[head->outputs[yy]]=0;
			}
		}
	}else if(head->gateType==9){	//9-MULTIPLEXER n : i1 · · ·i2^n : s0 ... sn : o
		//printNode(head,varNames,varVals,varNameLength,varQuant);
		//printf("n=%d\n",head->n);
		//printf("head->inputQuant= %d\n", head->inputQuant);
		int regInp=(1<<head->n);
		int selector=0;
		int uu=regInp;
		//printf("reginp =%d\n", regInp);
		for(uu=0;uu<head->n;uu++){
			selector+=(1<<uu)*varVals[head->inputs[regInp+head->n-(1+uu)]];
		}
		//printf("sellectr = %d\n", selector);
		varVals[head->outputs[0]]=varVals[head->inputs[selector]];
	}else if(head->gateType==10){ 	//10-PASS i : o
		//printf("we did it reddit\n");
		///printf("head->outputs[0] %d\n",head->outputs[0]);
		//printf("head->inputs[0] %d\n", head->inputs[0]);
		//printNode(head,varNames,varVals,varNameLength,varQuant);
		varVals[head->outputs[0]]=varVals[head->inputs[0]];
	}else{
		int p=0;
		for(p=0;p<head->outputQuant;p++){
			varVals[head->outputs[p]]=1;
		}
	}

	head->finished=1;
	return 1; //returns 1 if this time its been finished :)
}

char *correct(char*line,int size){
	char* out;
	int n=-1;
	int newLength;
	int colonCount=0;
	int colonsToAdd=0;
	int varsFound;
	int y=0;
	int v=0;
	for(y=0;y<size;y++){
		if(line[y]==':'){
			colonCount++;
		}else if(line[y]=='\n'||line[y]=='\0'||line[y]==EOF){
			break;
		}
	}
	//line=newLine;
	/*
	0-INPUT n : i1 · · ·in
	1-OUTPUT n : o1 · · · on
	2-NOT i : o
	3-AND i1 i2 : o
	4-OR i1 i2 : o
	5-NAND i1 i2 : o
	6-NOR i1 i2 : o
	7-XOR i1 i2 : o
	8-DECODER n : i1 · · ·in : o0 · · · o2n−1
	9-MULTIPLEXER n : i1 · · ·i2^n : s0 ... sn : o
	10-PASS i : o
	*/
	if(line[0]=='M'){
		if(colonCount==3){
			return line;
		}else{
			colonsToAdd=3-colonCount;
		}
		newLength=(colonsToAdd*2)+y;
	}else if(line[0]=='D'){
		if(colonCount==2){
			return line;
		}else{
			colonsToAdd=2-colonCount;
		}
		newLength=(colonsToAdd*2)+y;
	}else{
		if(colonCount==1){
			return line;
		}
		newLength=y;
		varsFound=-1;
		/*
		if(line[0]=='N'&&line[2]=='T'){	//2-NOT i : o
			//index=3;
			//varQuant=2;
		}else if(line[0]=='A'){ //3-AND i1 i2 : o
			//index=3;
			//varQuant=3;
		}else if(line[1]=='R'){//4-OR i1 i2 : o
			//index=2;
			//varQuant=3;
		}else if(line[0]=='N'&&line[2]=='N'){ 	//5-NAND i1 i2 : o
			//index=4;
			//varQuant=3;
		}else if(line[0]=='N'&&line[2]=='R'){	//6-NOR i1 i2 : o
			//index=3;
			//varQuant=3;
		}else if(line[0]=='X'){ 	//7-XOR i1 i2 : o
			//index=3;
			//varQuant=3;
		}else if(line[2]=='S'){
			//index=4;
			//varQuant=2;
		}*/
	}
	if(newLength==-1){
		printf("oof\n");
	}
	out=(char*)malloc((size)*sizeof(char));
	//n bois
	//0-INPUT n : i1 · · ·in
	//1-OUTPUT n : o1 · · · on
	//8-DECODER n : i1 · · ·in : o0 · · · o2n−1
	//9-MULTIPLEXER n : i1 · · ·i2^n : s0 ... sn : o
	if(line[0]=='M'||line[0]=='I'||line[0]=='D'||(line[0]=='O'&&line[1]=='U')){
		varsFound=-2;
		if(line[0]=='M'){
			varsFound=-2;
			n=getNFromLine(11,line,size);
			//index=11;
		}else if(line[0]=='I'){
			n=getNFromLine(5,line,size);
			varsFound=-1;
			//index=5;
		}else if(line[0]=='D'){
			n=getNFromLine(7,line,size);
			//index=7;
		}else{
			varsFound=-1;
			n=getNFromLine(6,line,size);
			//index=6;
		}
	}
	int wStart=-1;
	int wEnd=-1;
	int t;
	for(t=0;t<y;t++){
		if(wStart==-1&&(line[t]!=' ')){
			wStart=t;
			varsFound++;
		}else if(wStart!=-1&&(line[t]==' '||line[t]=='\0'||line[t]=='\n'||line[t]==EOF)){
			wEnd=t;
			if(wEnd==wStart+1&&line[wStart]==':'){
				varsFound-=1;
			}else{
				if(line[2]=='S'||(line[0]=='N'&&line[2]=='T')||(line[0]=='O'&&line[1]=='U')||line[0]=='I'){ 	//2-NOT i : o PASS i:o //1-OUTPUT n : o1 · · · on 0-INPUT n : i1 · · ·in
					if(varsFound==1){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
						break;
					}
				}else if(line[0]=='A'||line[1]=='R'||(line[0]=='N'&&line[2]=='N')||(line[0]=='N'&&line[2]=='R')||line[0]=='X'){ //3-AND i1 i2 : o 4-OR i1 i2 : o 5-NAND i1 i2 : o 6-NOR i1 i2 : o 7-XOR i1 i2 : o
					if(varsFound==2){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
						break;
					}
				}else if(line[0]=='M'){ //9-MULTIPLEXER n : i1 · · ·i2^n : s0 ... sn : o
					if(varsFound==0){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
					}else if(varsFound==(1<<n)){
						//WHERE COLON GO
						out[v]=' ';
						out[v+1]=':';
						v+=2;
					}else if(varsFound==(1<<n)+n){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
						break;
					}
				}else if(line[0]=='D'){ //8-DECODER n : i1 · · ·in : o0 · · · o2n−1
					if(varsFound==0){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
					}else if(varsFound==n){
						out[v]=' ';
						out[v+1]=':';
						v+=2;
						break;
					}
				}
			}
			wStart=-1;
		}
		out[v]= line[t];
		v++;
	}
	while(t<y){
		out[v]=line[t];
		t++;
		v++;
	}
	char*bye=(char*)malloc((v+1)*sizeof(char));
	int u;
	for(u=0;u<v;u++){
		bye[u]=out[u];
	}
	bye[v]='\0';
	free(out);
	return bye;
}