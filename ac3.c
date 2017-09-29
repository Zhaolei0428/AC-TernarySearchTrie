#include "ac3.h"

main()
{
	AC_STRUCT *ACTree = ac_alloc();
//	printf("%d",sizeof(*ACTree));
	ACTree->psize = 0;
	char* ch = "abcd";
	char* ch1 = "aefg";
	char* ch2 = "abche";
	ac_add_string(ACTree, ch, 4, 1);
	ac_add_string(ACTree, ch1, 4, 2);
	if(ac_add_string(ACTree, ch2, 5, 3)==1)
		preorder(ACTree->root->next);
	else
	    printf("error");
	
} 


//先根序遍历 ，调试用 
void preorder(TSTree node){
	TSTree currentNode;
	currentNode=node;
	if(currentNode!=NULL)
	{
		printf("%c",currentNode->data);
		preorder(currentNode->lchild);
		preorder(currentNode->next);
		preorder(currentNode->rchild);
	}
}



/*
 * ac_alloc
 *
 * Creates a new AC_STRUCT structure and initializes its fields.
 *
 * Parameters:    none.
 *
 * Returns:  A dynamically allocated AC_STRUCT structure.
 */
AC_STRUCT *ac_alloc(void)
{
  AC_STRUCT *node;

  if ((node = malloc(sizeof(AC_STRUCT))) == NULL)
    return NULL;
  memset(node, 0, sizeof(AC_STRUCT));

  if ((node->root = malloc(sizeof(TSNode))) == NULL) {
    free(node);
    return NULL;
  }
  memset(node->root, 0, sizeof(TSNode));

  return node;
}

/*
 * ac_add_string
 *
 * Adds a string to the AC_STRUCT structure's keyword tree.
 *
 * NOTE:  The `id' value given must be unique to any of the strings
 *        added to the tree, and must be a small integer greater than
 *        0 (since it is used to index an array holding information
 *        about each of the strings).
 *
 *        The best id's to use are to number the strings from 1 to K.
 *
 * Parameters:   node      -  an AC_STRUCT structure
 *               P         -  the sequence
 *               M         -  the sequence length
 *               id        -  the sequence identifier
 *
 * Returns:  non-zero on success, zero on error.
 */
int ac_add_string(AC_STRUCT *node, char *P, int M, int id)
{
	int i=0,j,flag;
	TSTree currentNode, preNode, newNode; 
	if(id==0||id<=node->psize)
	   return 0;
	
    if(node->psize==0)  //还未添加模式串 
    {
    	currentNode = node->root;
    	for(i;i<M;i++)
	    {
			if((newNode = malloc(sizeof(TSNode))) == NULL)     
				return -1;
			memset(newNode, 0, sizeof(TSNode));
			newNode->data = P[i];
			if(i == M-1)
				newNode->stateId = id; 
			else
				newNode->stateId = 0;
			currentNode->next = newNode;
			currentNode = currentNode->next;
    	}
    	node->psize++;
    	return 1;
	}
	
	currentNode = preNode = node->root->next;
	while(currentNode!=NULL && i<M)
	{
		if(P[i]==currentNode->data)
		{
			i++;
			preNode = currentNode;
			currentNode = currentNode->next;
			flag=0;
		}
		else if(P[i]<currentNode->data)
		{
			preNode = currentNode;
			currentNode = currentNode->lchild;
			flag = 1;
		} 
		else
		{
			preNode = currentNode;
			currentNode = currentNode->rchild;
			flag = 2;
		} 
	}
	//串已存在，返回0 
	if(i==M)
		return 0; 
	
	if((newNode = malloc(sizeof(TSNode))) == NULL)     
		return -1;
	memset(newNode, 0, sizeof(TSNode));
	newNode->data = P[i];
	if(i==M-1)
		newNode->stateId = id;
	else
		newNode->stateId = 0;
	currentNode = preNode;
	if(flag==0)
	{
		currentNode->next = newNode;
		currentNode = currentNode->next;
		i++;
	}
	else if(flag==1)
	{
		currentNode->lchild = newNode;
		currentNode = currentNode->lchild;
		i++;
	}
	else
	{
		currentNode->rchild = newNode;
		currentNode = currentNode->rchild;
		i++;
	}
	//插入完毕则返回 
	if(i==M)
	{
		node->psize++;
		return 1;
	}
		
	//将剩余字符插入 
	for(i;i<M;i++)  
	{
		if((newNode = malloc(sizeof(TSNode))) == NULL)     
			return -1;
		memset(newNode, 0, sizeof(TSNode));
		newNode->data = P[i];
		if(i == M-1)
			newNode->stateId = id; 
		else
			newNode->stateId = 0;
		currentNode->next = newNode;
		currentNode = currentNode->next;
    }
    node->psize++;
    return 1;
}










