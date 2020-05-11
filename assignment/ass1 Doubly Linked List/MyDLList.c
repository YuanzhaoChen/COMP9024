// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define MAX_LEN 100
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

//--------------data structures representing DLList--------
// data type for nodes
typedef struct DLListNode{
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;
//-----------------------------------------------------------

//------------------------function declaration---------------
DLListNode *newDLListNode(int it);
DLList *newDLList();
void insertTail(DLList*L,int num);
int isContainInList(DLList *l,int num);
int stoi(char*s,int n[]);
DLList *CreateDLListFromFileDlist(const char *filename);
DLList *cloneList(DLList *u);
DLList *setUnion(DLList *u, DLList *v);
DLList *setIntersection(DLList *u, DLList *v);
void freeDLList(DLList *L);
void printDLList(DLList *u);
//------------------------------------------------------------

// create a new DLListNode
DLListNode *newDLListNode(int it){
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList(){
	DLList *L;
	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// insert a new node with value num to the tail of DLList
// Time Complexity: O(1)
void insertTail(DLList*L,int num){
	assert(L!=NULL);//this function is always called after newDLList() so shouldn't worry too much about NULL
	DLListNode *currNode=newDLListNode(num); //create a new node that contains the new value
	if(L->size==0){
		L->first=currNode; //if the list is empty than this node is the first and last node of the list
		L->last=currNode;
		L->size+=1;
	}else{
		DLListNode *lastNode=L->last;
		lastNode->next=currNode;
		currNode->prev=lastNode;
		L->last=currNode; //last node is now the new node
		L->size+=1;
	}	
}

// check whether doubly linked list L contains integer num
// if yes, return 1; else return 0 
// Time Comlexity Analysis: O(N)
int isContainInList(DLList *L,int num){
	if(L==NULL||L->size==0){return 0;}
	DLListNode *currNode=L->first;
	while(currNode!=NULL){
		if(currNode->value==num){return 1;}
		currNode=currNode->next;
	}
	return 0;
}

// convet a string into an integer and store it in n[0]
// if the string is invalid, return 0; else return 1
// Time Complexity Analysis: O(N)
int stoi(char*s,int n[]){
	int res=0;
	int sign=1;
	char *p=s;
	while(*p!='\0'){
		if(*p>='0' && *p<='9'){
			res=res*10+(*p-'0');
		}else{
			if(p==s && *p=='-'){
				sign=-1;
			}else{
				return 0;
			}
		}
		p++;
	}
	n[0]=res*sign;
	return 1;
}

// create a DLList from a text file
// put your time complexity analysis for CreateDLListFromFileDlist() here
// Time Complexity Analysis: O(N^2)
DLList *CreateDLListFromFileDlist(const char *filename){
	DLList *L=newDLList();
	if(strcmp(filename,"stdin")==0){
		char str[MAX_LEN];//read integer from stdin
		while(scanf("%s",str)!=EOF && strcmp(str,"end")!=0){
			int n[1];
			int isValid=stoi(str,n);
			if(isValid==0){
				printf("Invalid input!\n");
				int c;
				while ((c=getchar())!='\n' && c!=EOF);//clear input buffer 
				return NULL;
			}else{
				insertTail(L,n[0]);
			}
		}
	}else{
		FILE *fp=fopen(filename,"r");//read integer from file
		if(fp==NULL){
			printf("Invalid input!\n");
			return NULL;
		}
		int num;
		while(fscanf(fp,"%d",&num)==1){
			insertTail(L,num);
		}
		fclose(fp);
	}
  	return L;
}

// clone a DLList
// put your time complexity analysis for cloneList() here
// Time Complexity Analysis: O(N)
DLList *cloneList(DLList *u){
	if(u==NULL){return NULL;}
	DLList *l=newDLList();
	DLListNode *currNode=u->first;
	while(currNode!=NULL){
		insertTail(l,currNode->value);
		currNode=currNode->next;
	}
 	return l;
}

// compute the union of two DLLists u and v
// Time Complexity Analysis: O(N^2)
DLList *setUnion(DLList *u, DLList *v){
	if(u==NULL||u->size==0){ //nothing in u
		return cloneList(v);
	}
	if(v==NULL||v->size==0){ //nothing in v
		return cloneList(u);
	}
	DLList *unionL=cloneList(u);
	DLListNode *currNode=v->first;
	while(currNode!=NULL){
		if(isContainInList(u,currNode->value)==0){
			insertTail(unionL,currNode->value);
		}
		currNode=currNode->next;
	}
	return unionL;
}

// compute the insection of two DLLists u and v
// put your time complexity analysis for intersection() here
// Time Complexity Analysis: O(N^2)
DLList *setIntersection(DLList *u, DLList *v){
	DLList *intersectedList=newDLList();
	if(u==NULL||u->size==0){
		return intersectedList;
	}
	if(v==NULL||v->size==0){
		return intersectedList;
	}
	DLListNode *currNode=u->first;
	while(currNode!=NULL){
		if(isContainInList(v,currNode->value)==1){ //isContainInList() runs linear time
			insertTail(intersectedList,currNode->value);
		}
		currNode=currNode->next;
	}
	return intersectedList;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList() here
// Time Complexity Analysis: O(N)
void freeDLList(DLList *L){
	if(L==NULL){return;} //nothing to free
	DLListNode *currNode=L->first;
	while(currNode!=NULL){
		DLListNode *nextNode=currNode->next; //free each node in doubly linked list
		free(currNode);
		currNode=nextNode;
	}
	free(L); //free doubly linked list after all nodes are free
}

// display items of a DLList
// put your time complexity analysis for printDDList() here
// Time Complexity Analysis: O(N)
void printDLList(DLList *u){
	if(u==NULL||u->size==0){
		printf("Doubly linked list is empty, nothing to print.\n");
		return;
	}
	printf("Printing doubly linked list:\n");
	DLListNode *currNode=u->first;
	while(currNode!=NULL){
		printf(" %d ",currNode->value);
		if(currNode->next!=NULL){
			printf("->");
		}
		currNode=currNode->next;
	}
	printf("\n");
}

int main(){
	DLList *list1, *list2, *list3, *list4;
	
 	list1=CreateDLListFromFileDlist("File1.txt");
 	printDLList(list1);

 	list2=CreateDLListFromFileDlist("File2.txt");
 	printDLList(list2);

	list3=setUnion(list1, list2);
	printDLList(list3);

	list4=setIntersection(list1, list2);
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	
	printf("please type all the integers of list1\n");
	list1=CreateDLListFromFileDlist("stdin");

	printf("please type all the integers of list2\n");
	list2=CreateDLListFromFileDlist("stdin");

	list3=cloneList(list1);
	printDLList(list3);
	list4=cloneList(list2);
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);
	
	return 0; 
}
