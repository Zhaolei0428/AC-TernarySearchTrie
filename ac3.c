#include "ac3.h"

main()
{
//    Queue* q=que_init();
//    printf("%d\n",q->head);
	
	AC_STRUCT *ACTree = ac_alloc();
//	printf("%d",sizeof(*ACTree));
	ACTree->psize = 0;
	char* ch = "abcd";
	char* ch1 = "aefg";
	char* ch2 = "chae";
	ac_add_string(ACTree, ch, 4, 1);
	ac_add_string(ACTree, ch1, 4, 2);
	ac_add_string(ACTree, ch2, 4, 3);
	ac_implement(ACTree);
	preorder(ACTree->root->next);
} 


//先根序遍历 ，调试用 
void preorder(TSTree node){
	TSTree currentNode;
	currentNode=node;
	if(currentNode!=NULL)
	{
		printf("%c %d...",currentNode->data,currentNode->stateId);
		if(currentNode->faillink!=NULL)
			printf("%c %d\n",currentNode->faillink->data,currentNode->faillink->stateId);
		else
		    printf("\n");
		preorder(currentNode->lchild);
		preorder(currentNode->next);
		preorder(currentNode->rchild);
	}
}

//存储树节点的队列，辅助实现faillink
Queue* que_init()
{
	Queue* q;
	if((q=malloc(sizeof(Queue))) == NULL)
		return NULL;
	memset(q,0,sizeof(Queue));
	q->head=0;
	q->tail=0;
	return q;
}
//队列判空 
int empty(Queue* q)
{
	if(q->head==q->tail)
		return 1;
	return 0;
}

//入队，成功返回1，否则返回0 
int enqueue(Queue* q, TSTree node)
{
	if((q->head-q->tail+maxn)%maxn==1)
		return 0;
	else
	{
		q->queue[q->tail]=node;
		q->tail=(q->tail+1)%maxn;
		return 1;
    }
} 
//出队，返回出队节点或NULL（队空） 
TSTree dequeue(Queue* q)
{
	if(q->head==q->tail)
		return NULL;
	else
    {
    	int head=q->head;
    	q->head=(head+1)%maxn;
    	return q->queue[head];
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

//对trie树添加faillink，完成outputlink，实现自动机 
int ac_implement(AC_STRUCT* node)
{
	TSTree root,currentNode,preNode,lchild,rchild,state,tempNode;
	root=node->root;
	int flag;
	Queue* q1, *q2, *q3;
	q1=que_init();      //主要队列 
	q2=que_init();      //辅助队列 
	q3=que_init();      //辅助队列 
	if(root->next!=NULL)
	{
		currentNode=root->next;
		if(!enqueue(q2,currentNode))
			return 0;
		while(!empty(q2))
		{
			currentNode=dequeue(q2);
			currentNode->faillink=root;
			enqueue(q1,currentNode);
			lchild=currentNode->lchild;
			rchild=currentNode->rchild;
			if(lchild!=NULL)
				enqueue(q2,lchild);
			if(rchild!=NULL)
				enqueue(q2,rchild);
		}
		
		while(!empty(q1))
		{
			preNode=dequeue(q1);
			if(preNode->next!=NULL)
			{
				enqueue(q2,preNode->next);
				while(!empty(q2))
				{
					currentNode=dequeue(q2);
					lchild=currentNode->lchild;
			    	rchild=currentNode->rchild;
			    	if(lchild!=NULL)
				    	enqueue(q2,lchild);
			    	if(rchild!=NULL)
			     		enqueue(q2,rchild);
			     	enqueue(q1,currentNode);
			     	
			     	//计算currentNode的faillink
			     	flag=1;
			     	state=preNode;
					while(flag)
					{
						state = state->faillink;
						if(state->next!=NULL)
						{
							enqueue(q3,state->next);
							while(!empty(q3))
							{
								tempNode=dequeue(q3);
								lchild=tempNode->lchild;
								rchild=tempNode->rchild;
								if(lchild!=NULL)
									enqueue(q3,lchild);
								if(rchild!=NULL)
									enqueue(q3,rchild);
								if(tempNode->data==currentNode->data)
								{
									currentNode->faillink=tempNode;
									//计算outlink 
									if(tempNode->stateId!=0)
										currentNode->outlink=tempNode;
									flag=0;
								}
							}
						}
						if(state==root&&flag==1)
						{
							currentNode->faillink=root;
							break;
						}
					}//end while(flag) 
				
				}//end q2
			}
			
		}//end q1
	}
	return 1;
} 






