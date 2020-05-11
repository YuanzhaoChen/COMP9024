// Author: Yuanzhao Chen
// Student ID: 5041686
// Platform: (select one of Windows, Unix, Mac): Mac(macOS Mojave)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 1000
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v ){
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree(){
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

typedef struct item{
	int key;
	int value;
}Item;
//--------------------function declaration------------------------
AVLTree *CreateAVLTree(const char *filename);
AVLTree *CloneAVLTree(AVLTree *T);
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2);
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2);
int InsertNode(AVLTree *T, int k, int v);
int DeleteNode(AVLTree *T, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);
void FreeAVLTree(AVLTree *T);
void PrintAVLTree(AVLTree *T);

void CloneAVLTree_Helper(AVLTreeNode *src,AVLTreeNode *dst);
void CloneChildren(AVLTreeNode *src,AVLTreeNode *dst);
void treeToArray(AVLTreeNode *root,Item arr[],int *pos);
void mergeArray(Item a1[],int len1,Item a2[],int len2,Item a3[],int *pos);
void intersectArray(Item a1[],int len1,Item a2[],int len2,Item a3[],int *pos);
AVLTreeNode *arrayToTree(AVLTreeNode *root,Item arr[],int begin,int end);
AVLTreeNode *InsertNode_Helper(AVLTreeNode *root,int k,int v,int res[],AVLTreeNode *newNode);
AVLTreeNode *Search_Helper(AVLTreeNode *root,int k,int v);
AVLTreeNode *DeleteNode_Helper(AVLTreeNode *root,int k,int v);
AVLTreeNode *minNode(AVLTreeNode*root);
void FreeAVLTree_Helper(AVLTreeNode *root);
void PrintAVLTree_Helper(AVLTreeNode *root);
AVLTreeNode *tallerChild(AVLTreeNode*parentNode);
void updateAllHeight(AVLTreeNode *root);
void updateHeightUpwardsFrom(AVLTreeNode *node);
AVLTreeNode *trinodeRestructure(AVLTreeNode*root,AVLTreeNode *newNode);
int isValidHeight(AVLTreeNode *node);
int compareTuple(int k1,int v1,int k2,int v2);
int max(int a,int b);
//--------------------------------------------------------------

// total time complexity: O(NlogN), each insertion takes O(logN) and there're
// N insertions, where N is the number of (key,value) in file or stdin
AVLTree *CreateAVLTree(const char *filename){
	AVLTree *tree=newAVLTree();
	if(strcmp(filename,"stdin")==0){ //read from standard in 
		char s[MAX_LEN];
		while(fgets(s,MAX_LEN,stdin)!=NULL){
			int len=strlen(s);
			int i=0;
			while(i<len){
				if(s[i]=='('){//there's a new item to come
					int key=0,value=0,keySign=1,valueSign=1;
					while(s[i]!=','){ //before ',', all digits belong to key 
						if(s[i]=='-'){keySign=-1;}
						if(s[i]>='0' && s[i]<='9'){key=10*key+s[i]-'0';}
						i+=1;
						if(i==len){return tree;}
					}
					while(s[i]!=')'){ //digites between ',' and ')' belong to key
						if(s[i]=='-'){valueSign=-1;}
						if(s[i]>='0' && s[i]<='9'){value=10*value+s[i]-'0';}
						i+=1;
						if(i==len){return tree;}
					}
					key*=keySign;
					value*=valueSign;
					InsertNode(tree,key,value);
				}
				i+=1;
			}
			if(strstr(s,"end")!=NULL){break;}
		}
	}else{	//read from txt file
		FILE *fp=fopen(filename,"r");//read integer from file
		if(fp==NULL){
			printf("Invalid input!\n");
			return NULL;
		}
		char s[MAX_LEN];
		while(fgets(s,MAX_LEN,fp)!=NULL){
			int len=strlen(s);
			int i=0;
			while(i<len){
				if(s[i]=='('){//there's a new item
					int key=0,value=0,keySign=1,valueSign=1;
					while(s[i]!=','){ //before ',', all digits belong to key 
						if(s[i]=='-'){keySign=-1;}
						if(s[i]>='0' && s[i]<='9'){key=10*key+s[i]-'0';}
						i+=1;
						if(i==len){return tree;}
					}
					while(s[i]!=')'){ //digites between ',' and ')' belong to key
						if(s[i]=='-'){valueSign=-1;}
						if(s[i]>='0' && s[i]<='9'){value=10*value+s[i]-'0';}
						i+=1;
						if(i==len){return tree;}
					}
					key*=keySign;
					value*=valueSign;
					InsertNode(tree,key,value);
				}
				i+=1;
			}
			if(strstr(s,"end")!=NULL){break;}
		}
	}
	return tree;
}

// return a cloned tree of T, assume T is balanced so height is never checked
// time complexity: O(N), each node is accessed once
AVLTree *CloneAVLTree(AVLTree *T){
	assert(T!=NULL);
	AVLTree*T2=newAVLTree();
	if(T->root==NULL){return T2;}
	T2->size=T->size;
	T2->root=newAVLTreeNode(T->root->key,T->root->value);
	T2->root->height=T->root->height;
	CloneAVLTree_Helper(T->root,T2->root);
	return T2;
}

// helper method of CloneAVLTree() 
// time complexity: O(N), each node is accessed once
void CloneAVLTree_Helper(AVLTreeNode *src,AVLTreeNode *dst){
	//if current node is null or have no childre we don't need to clone
	if(src==NULL||(src->left==NULL&&src->right==NULL)){return;}
	CloneChildren(src,dst);
	CloneAVLTree_Helper(src->left,dst->left);
	CloneAVLTree_Helper(src->right,dst->right);
}

// clone the children of src to dst, so dst has the same children 
// time complexity: O(1), the run time cost is fixed regardless of 
// what *src and *dst are.
void CloneChildren(AVLTreeNode*src,AVLTreeNode*dst){
	assert(src!=NULL);
	if(src->left!=NULL){// if there is a left child to clone, clone it 
		AVLTreeNode *leftChild=newAVLTreeNode(src->left->key,src->left->value);
		leftChild->height=src->left->height;
		leftChild->parent=dst;
		dst->left=leftChild;
	}
	if(src->right!=NULL){// if there is a right child to clone, clone it 
		AVLTreeNode *rightChild=newAVLTreeNode(src->right->key,src->right->value);
		rightChild->height=src->right->height;
		rightChild->parent=dst;
		dst->right=rightChild;
	}
}

// complexity of treeToArray(T1->root,a1,pos1): O(N), where N is the size of T1
// complexity of treeToArray(T2->root,a2,pos2): O(M), where M is the size of T2
// complexity of mergeArray(a1,T1->size,a2,T2->size,a3,pos3): O(M+N)
// complexity of arrayToTree(unionedTree->root,a3,0,unionedTree->size-1): O(M+N)
// complexity of free(a1),free(a2),free(a3): O(1)
// total time complexity: O(N)+O(M)+O(M+N)+O(M+N)+3*O(1)=O(M+N), runs linearly
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2){
	AVLTree *unionedTree=newAVLTree();
	Item*a1=(Item*)malloc(sizeof(Item)*T1->size);
	Item*a2=(Item*)malloc(sizeof(Item)*T2->size);
	Item*a3=(Item*)malloc(sizeof(Item)*(T1->size+T2->size));
	int pos1[1]={0};//the index of array a1
	int pos2[1]={0};//the index of array a2
	int pos3[1]={0};//the index of array a3
	treeToArray(T1->root,a1,pos1);//fill a1
	treeToArray(T2->root,a2,pos2);//fill a2
	mergeArray(a1,T1->size,a2,T2->size,a3,pos3);//fill a3
	unionedTree->size=pos3[0];
	unionedTree->root=arrayToTree(unionedTree->root,a3,0,unionedTree->size-1);
	updateAllHeight(unionedTree->root);
	free(a1);
	free(a2);
	free(a3);
	return unionedTree;
}

// fill items in tree into arr[], in sorted order
// time complexity: O(N), each node is only visted once
void treeToArray(AVLTreeNode *root,Item arr[],int *pos){
	if(root==NULL){return;}
	//in-order traversal, so arr[] is in sorted order
	treeToArray(root->left,arr,pos);
	arr[*pos].key=root->key;
	arr[*pos].value=root->value;
	pos[0]+=1;
	treeToArray(root->right,arr,pos);
}

// merge a1 and a2 into a3, with a3 in sorted order
// total time complexity: O(M+N), where M is length of a1 and N is length of a2
void mergeArray(Item a1[],int len1,Item a2[],int len2,Item a3[],int *pos){
	int i=0,j=0;
	while(i<len1&&j<len2){
		int cmp=compareTuple(a1[i].key,a1[i].value,a2[j].key,a2[j].value);
		if(cmp<0){	//a1[i] is smaller than a2[j], append a1[i] to a3
			a3[*pos]=a1[i];
			i+=1;
			*pos+=1;
		}else if(cmp>0){ //a1[i] is bigger than a2[j], append a2[i] to a3
			a3[*pos]=a2[j];
			j+=1;
			*pos+=1;
		}else{	//they are the same, add either one
			a3[*pos]=a1[i];
			i+=1;
			j+=1;
			*pos+=1;
		}
	}
	while(i<len1){	//still got item in a1
		a3[*pos]=a1[i];
		i+=1;
		*pos+=1;
	}
	while(j<len2){	//still got item in a2
		a3[*pos]=a2[j];
		j+=1;
		*pos+=1;
	}
}

// a3 is the intersection of  a1 and a2 
void intersectArray(Item a1[],int len1,Item a2[],int len2,Item a3[],int *pos){
	int i=0,j=0;
	while(i<len1&&j<len2){
		int cmp=compareTuple(a1[i].key,a1[i].value,a2[j].key,a2[j].value);
		if(cmp<0){
			i+=1;
		}else if(cmp>0){
			j+=1;
		}else{ //duplicates found, append to a3
			a3[*pos]=a1[i];
			i+=1;
			j+=1;
			*pos+=1;
		}
	}
}

// convert an sorted array to AVLTreeNodes, pick the node the same order as 
// binary search to ensure the result is balanced
// time complexity: O(M+N),each element in arr[] is assigned to a place in root
AVLTreeNode*arrayToTree(AVLTreeNode*root,Item arr[],int begin,int end){
	if(begin>end){return NULL;}
	int mid=(int)(ceil(begin*0.5+end*0.5));
	root=newAVLTreeNode(arr[mid].key,arr[mid].value);
	AVLTreeNode *leftChild=arrayToTree(root->left,arr,begin,mid-1);
	AVLTreeNode *rightChild=arrayToTree(root->right,arr,mid+1,end);
	root->left=leftChild;
	if(leftChild!=NULL){leftChild->parent=root;}
	root->right=rightChild;
	if(rightChild!=NULL){rightChild->parent=root;}
	return root;
}

// time complexity of creating intersectedTree,a1,a2,a3: O(1)
// time complexity of treeToArray(): O(N)
// time complexity of intersectArray(): O(M+N),where M is T1 size and N is T2 size
// time complexity of arrayToTree(): O(N)
// time complexity of updateAllHeight: O(N)
// time complexity of free(): O(1)
// total time complexity: 4*O(1)+O(N)+O(M+N)+O(N)+O(N)+O(1)=O(M+N)
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2){
	AVLTree *intersectedTree=newAVLTree();
	Item*a1=(Item*)malloc(sizeof(Item)*T1->size);
	Item*a2=(Item*)malloc(sizeof(Item)*T2->size);
	Item*a3=(Item*)malloc(sizeof(Item)*(T1->size+T2->size));
	int pos1[1]={0};//the index of array a1
	int pos2[1]={0};//the index of array a2
	int pos3[1]={0};//the index of array a3
	treeToArray(T1->root,a1,pos1);//fill a1
	treeToArray(T2->root,a2,pos2);//fill a2
	intersectArray(a1,T1->size,a2,T2->size,a3,pos3);
	intersectedTree->size=*pos3;
	intersectedTree->root=arrayToTree(intersectedTree->root,a3,0,intersectedTree->size-1);
	updateAllHeight(intersectedTree->root);
	free(a1);
	free(a2);
	free(a3);
	return intersectedTree;
}

// time complexity of newAVLTreeNode(): O(1)
// time complexity of InsertNode_Helper(): O(logN)  
// time complexity of updateHeightUpwardsFrom(): O(logN)
// time complexity of trinodeRestructure: O(logN)
// total time complexity: O(1)+3*O(logN)=O(logN)  
int InsertNode(AVLTree *T, int k, int v){
	assert(T!=NULL);
	int res[1];
	AVLTreeNode *newNode=newAVLTreeNode(k,v);
	T->root=InsertNode_Helper(T->root,k,v,res,newNode);
	if(res[0]==0){	//if it's a duplicate then no need to keep it
		free(newNode); 
		return 0;
	} 
	T->size+=1;
	//from newNode, update height upwards
	updateHeightUpwardsFrom(newNode);
	//updateAllHeight(T->root);//after insertion is done, we also need to update height
	T->root=trinodeRestructure(T->root,newNode);//restructure trinode if necessary
	//updateAllHeight(T->root); //update height again
	return 1;
}

// time complexity: insert into a tree structure only takes O(logN) 
AVLTreeNode *InsertNode_Helper(AVLTreeNode *root,int k,int v,int res[],AVLTreeNode*newNode){
	if(root==NULL){	//find a new sopt to insert
		res[0]=1;
		return newNode;
	}
	int cmp=compareTuple(root->key,root->value,k,v);
	if(cmp==0){	//it's a duplicate, nothing to insert
		res[0]=0;
	}else if(cmp>0){
		root->left=InsertNode_Helper(root->left,k,v,res,newNode);
		root->left->parent=root;	//connect parent-child
	}else{
		root->right=InsertNode_Helper(root->right,k,v,res,newNode);
		root->right->parent=root;	//connect parent-child
	}
	return root;
}

// compare tuple (k1,v1) with (k2,v2)
// if (k1,v1) is bigger then return 1, if equal, return 0, else return -1
// time complexity: O(1)
int compareTuple(int k1,int v1,int k2,int v2){
	if(k1==k2&&v1==v2){return 0;}	//it's a duplicate
	if(k1>k2){return 1;}
	if(k1==k2&&v1>v2){return 1;}
	return -1;
}

// update the height of every node in the tree
// time complexity: O(N), where N is root size
void updateAllHeight(AVLTreeNode *root){
	if(root==NULL){return;}
	if(root->left==NULL&&root->right==NULL){
		root->height=0;
	}else if(root->left==NULL&&root->right!=NULL){
		updateAllHeight(root->right);
		root->height=1+root->right->height;
	}else if(root->left!=NULL&&root->right==NULL){
		updateAllHeight(root->left);
		root->height=1+root->left->height;
	}else{	//both children are not null
		updateAllHeight(root->left);
		updateAllHeight(root->right);
		root->height=1+max(root->left->height,root->right->height);
	}
}

// update the height nodes in the tree from *node
// keep updating until at some point new height of a node doesn't change
// the height of its parent
// time complexity: let N be the tree size, since this function keep tracking parent upwards
// it's max number of operations is not larger than tree height. Hence it's O(logN) !!!
void updateHeightUpwardsFrom(AVLTreeNode*node){
	AVLTreeNode*ref=node->parent;
	while(ref!=NULL&&ref->height!=1+tallerChild(ref)->height){
		ref->height=1+tallerChild(ref)->height;
		ref=ref->parent;
	}
}

// time complexity: O(1)
int max(int a,int b){
	if(a>b){return a;}
	return b;
}

// restructure trinode, same as lecture note, return the root
// total time complexity: O(logN), since we also update height in the function
AVLTreeNode* trinodeRestructure(AVLTreeNode*root,AVLTreeNode *newNode){
	AVLTreeNode *z=newNode;
	AVLTreeNode *y;
	AVLTreeNode *x;
	while(z!=NULL && isValidHeight(z)==1){z=z->parent;}
	if(z==NULL){return root;} //height is already valid, not need to go further
	y=tallerChild(z);
	x=tallerChild(y);
	if(compareTuple(x->key,x->value,y->key,y->value)==1&&compareTuple(x->key,x->value,z->key,z->value)==1){
		//x is the biggest node among x,y,z
		if(compareTuple(y->key,y->value,z->key,z->value)==1){
			//y is the second biggest: x>y>z
			y->parent=z->parent;
			//if z was root, new root should be y, else assign y to z's parent
			if(z->parent!=NULL){
				if(z->parent->right==z){ 
					y->parent->right=y;
				}else{
					y->parent->left=y;
				}
			}else{
				root=y;
			}
			z->parent=y;
			if(y->left!=NULL){
				y->left->parent=z;
			}
			z->right=y->left;
			y->left=z;
			//update height correspondingly
			if(tallerChild(z)!=NULL){z->height=1+tallerChild(z)->height;}
			else{z->height=0;}
			y->height=1+tallerChild(y)->height;
			//keep updating height upwards
			updateHeightUpwardsFrom(y);
		}
	}else if(compareTuple(y->key,y->value,x->key,x->value)==1&&compareTuple(y->key,y->value,z->key,z->value)==1){
		//y is the biggest node among x,y,z
		if(compareTuple(x->key,x->value,z->key,z->value)==1){
			//x is the second biggest: y>x>z
			x->parent=z->parent;
			//we have to figure out whether z was right child of left
			if(z->parent!=NULL){
				if(z->parent->right==z){ 
					z->parent->right=x;
				}else{
					z->parent->left=x;
				}
			}else{
				root=x;
			}
			z->parent=x;
			z->right=x->left;
			if(x->left!=NULL){
				x->left->parent=z;
			}
			y->left=x->right;
			if(x->right!=NULL){
				x->right->parent=y;
			}
			x->right=y;
			y->parent=x;
			//update height correspondingly
			if(tallerChild(z)!=NULL){z->height=1+tallerChild(z)->height;}
			else{z->height=0;}
			if(tallerChild(y)!=NULL){y->height=1+tallerChild(y)->height;}
			else{y->height=0;}
			x->height=1+tallerChild(x)->height;
			updateHeightUpwardsFrom(x);
		}
	}else{
		//z is the biggest node among x,y,z
		if(compareTuple(x->key,x->value,y->key,y->value)==1){
			//x is the second biggest: z>x>y
			x->parent=z->parent;
			if(z->parent!=NULL){
				if(z->parent->right==z){
					z->parent->right=x;
				}else{
					z->parent->left=x;
				}
			}else{
				root=x;
			}
			z->parent=x;
			y->parent=x;
			if(x->left!=NULL){x->left->parent=y;}
			y->right=x->left;
			if(x->right!=NULL){x->right->parent=z;}
			z->left=x->right;
			x->right=z;
			x->left=y;
			//update height correspondingly
			if(tallerChild(y)!=NULL){y->height=1+tallerChild(y)->height;}
			else{y->height=0;}
			if(tallerChild(z)!=NULL){z->height=1+tallerChild(z)->height;}
			else{z->height=0;}
			x->height=1+tallerChild(x)->height;
			updateHeightUpwardsFrom(x);
		}else{
			//y is the second biggest: z>y>x
			y->parent=z->parent;
			if(z->parent!=NULL){
				if(z->parent->right==z){
					z->parent->right=y;
				}else{
					z->parent->left=y;
				}
			}else{ //z was root
				root=y;
			}
			z->parent=y;
			if(y->right!=NULL){
				y->right->parent=z;
			}
			z->left=y->right;
			y->right=z;
			//update height 
			if(tallerChild(z)!=NULL){z->height=1+tallerChild(z)->height;}
			else{z->height=0;}
			y->height=1+tallerChild(y)->height;
			updateHeightUpwardsFrom(y);
		}
	}
	return root;
}

// check whether height of *node is valid, if yes return 1,else return 0
// time complexity: O(1), each if statement runs only once
int isValidHeight(AVLTreeNode *node){
	assert(node!=NULL);
	if(node->left==NULL&&node->right==NULL){return 1;}
	if(node->left!=NULL&&node->right==NULL){
		if(node->left->height==0){return 1;}
		return 0;
	}
	if(node->left==NULL&&node->right!=NULL){
		if(node->right->height==0){return 1;}
		return 0;
	}
	if(abs(node->left->height-node->right->height)<=1){ //valid if height differ no more than 1
		return 1;
	}
	return 0;
}

// give a node,return a child node with a greater height,if the node has no child then return NULL
// time complexity: O(1), each operation in if statement is access only once
AVLTreeNode *tallerChild(AVLTreeNode*parentNode){
	assert(parentNode!=NULL);
	AVLTreeNode*leftChild=parentNode->left;
	AVLTreeNode*rightChild=parentNode->right;
	if(leftChild==NULL&&rightChild==NULL){
		return NULL;
	}
	if(leftChild==NULL&&rightChild!=NULL){
		return rightChild;
	}
	if(leftChild!=NULL&&rightChild==NULL){
		return leftChild;
	}
	if(leftChild->height>rightChild->height){
		return leftChild;
	}else{
		return rightChild;
	}
} 

// time complexity of Search(): O(logN)
// time complexity of DeleteNode_Helper(): O(logN)
// total time complexity: O(logN)+O(logN)=O(logN)
int DeleteNode(AVLTree *T, int k, int v){
	AVLTreeNode*node=Search(T,k,v);
	if(node==NULL){return 0;}	//if item not found then do nothing
	AVLTreeNode*ref=node->parent; //travel upwards to see whether height is good
	T->root=DeleteNode_Helper(T->root,k,v);
	while(ref!=NULL){
		AVLTreeNode*next=ref->parent;
		if(isValidHeight(ref)==0){
			T->root=trinodeRestructure(T->root,ref);
		}
		ref=next;
	}
	T->size-=1;
	return 1;
}

// time complexity analysis: O(logN), the operation is similar to binary search
AVLTreeNode *DeleteNode_Helper(AVLTreeNode *root,int k,int v){
	if(root==NULL){return NULL;}
	int cmp=compareTuple(root->key,root->value,k,v);
	if(cmp>0){
		root->left=DeleteNode_Helper(root->left,k,v);
		root->height=1+tallerChild(root)->height;//height may be changed
	}else if(cmp<0){
		root->right=DeleteNode_Helper(root->right,k,v);
		root->height=1+tallerChild(root)->height;//height may be changed
	}else{ //root is the node we want to delet
		if(root->left==NULL){
			AVLTreeNode*returnedNode=root->right;
			if(returnedNode!=NULL){
				returnedNode->parent=root->parent;
			}
			free(root);//deleted node is no longer needed,free it
			return returnedNode;
		}else if(root->right==NULL){
			AVLTreeNode*returnNode=root->left;
			if(returnNode!=NULL){
				returnNode->parent=root->parent;
			}
			free(root);
			return returnNode;
		}
		AVLTreeNode*successor=minNode(root->right); //smallest node in right subtree is a good successors
		successor->parent->left=NULL; //disconnect successor node from it's origin parent
		if(tallerChild(successor->parent)!=NULL){ //height of successor's orgin parent may change
			successor->parent->height=1+tallerChild(successor->parent)->height;
		}else{
			successor->parent->height=0;
		}
		successor->left=root->left;
		if(root->left!=NULL){root->left->parent=successor;}
		successor->right=root->right;
		if(root->right!=NULL){root->right->parent=successor;}
		if(root->parent!=NULL&&root->parent->right==root){ //root is right child of its parent
			root->parent->right=successor;
		}else if(root->parent!=NULL&&root->parent->left==root){//root is left child of its parent
			root->parent->left=successor;
		}
		successor->parent=root->parent;
		free(root);
		root=successor;
		if(tallerChild(root)!=NULL){
			root->height=1+tallerChild(root)->height;
		}else{
			root->height=0;
		}
	}
	return root;
}
// find the smallest nodes in root
// time complexity: O(logN), max number of run equals to tree height
AVLTreeNode*minNode(AVLTreeNode*root){
	if(root->left==NULL){return root;}
	return minNode(root->left);
}

// tmie complexity of Search_Helper()
// total time complexity: O(logN) 
AVLTreeNode *Search(AVLTree *T, int k, int v){
	return Search_Helper(T->root,k,v);
}

// time complexity: O(logN), binary search
AVLTreeNode *Search_Helper(AVLTreeNode *root,int k,int v){
	if(root==NULL){return NULL;}
	int cmp=compareTuple(root->key,root->value,k,v);
	if(cmp==0){return root;}//item is found
	if(cmp>0){return Search_Helper(root->left,k,v);}//root is larger, search left subTree
	else{return Search_Helper(root->right,k,v);}//root is smaller, searcg right subTree
}

// tmie complexity of FreeAVLTree_Helper: O(N)
// time complexity of free(T): O(1)
// total time complexity: O(N)+O(1)=O(N)
void FreeAVLTree(AVLTree *T){
	FreeAVLTree_Helper(T->root);
	free(T);
}

// N node is accessed and finally root is free so,
// time complexity: O(N), remove all nodes 
void FreeAVLTree_Helper(AVLTreeNode *root){
	if(root==NULL){return;}
	FreeAVLTree_Helper(root->left);
	FreeAVLTree_Helper(root->right);
	free(root);
}

// time complexity: O(N), since all elements in T will be accessed only once
void PrintAVLTree(AVLTree *T){
	PrintAVLTree_Helper(T->root);
}

// time complexity: O(N), since all elements in T will be accessed only once
void PrintAVLTree_Helper(AVLTreeNode *root){
	if(root==NULL){return;}
	PrintAVLTree_Helper(root->left);
	if(root->parent==NULL){
		printf("(%d,%d),height=%d it's root\n",root->key,root->value,root->height);
	}else{
		printf("(%d,%d),height=%d it's parent is (%d,%d)\n",root->key,root->value,root->height,root->parent->key,root->parent->value);
	}
	PrintAVLTree_Helper(root->right);
}

int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 printf("-----------------tree1 from stdin------------\n");
 PrintAVLTree(tree1);
 printf("---------------------------------------------\n");
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
 printf("---------------tree2 from file---------------\n");
 PrintAVLTree(tree2);
 printf("---------------------------------------------\n");
 tree3=CloneAVLTree(tree2);
 printf("-----------tree3 from clone tree2 -----------\n");
 PrintAVLTree(tree3);
 printf("---------------------------------------------\n");
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  printf("------------------tree 4--------------------\n");
  PrintAVLTree(tree4);
  printf("--------------------------------------------\n");
  node1=Search(tree4,20,20);
  printf("search (20,20) in tree4:\n");
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
	printf("deleting (i,i):\n");  
    j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 //Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 printf("----------------------tree 5-----------------\n");
 PrintAVLTree(tree5);
 printf("---------------------------------------------\n");
 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 printf("-----------------------tree 6----------------\n");
 PrintAVLTree(tree6);
 printf("---------------------------------------------\n");
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 printf("----------------tree 5 intesect tree 6-------\n");
 PrintAVLTree(tree7);
 printf("---------------------------------------------\n");
 printf("----------------tree 5 union tree 6----------\n");
 PrintAVLTree(tree8);
 printf("---------------------------------------------\n");
 return 0; 
}