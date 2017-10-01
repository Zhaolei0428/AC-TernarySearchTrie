#include "ac3.h"
FILE *resultfp;

int main()
{

	AC_STRUCT *ACTree = ac_alloc();
	ACTree->psize = 0;

    char sline[100];
    FILE *fp, *strfp;
    if((fp = fopen("C:/Users/zhaol/Desktop/strsearch/pattern.txt","r")) == NULL)
    {
    	printf("file pattern.txt open failed!\n");
    	return;
	}
    int i=1,j,len;
    while(!feof(fp))
    {
    	fgets(sline,100,fp);
    	len=strlen(sline)-1;     //去掉换行符'\n' 
    	Pa p=malloc(sizeof(Pattern));
		for(j=0;j<len;j++)
		   p->P[j]=sline[j];
		p->P[len]='\0';
		p->length = len;
		Patterns[i]=p;
		
		ac_add_string(ACTree, sline, len, i);
    	i++;
	}
	
	ac_implement(ACTree);

	if((strfp = fopen("C:/Users/zhaol/Desktop/strsearch/string.txt","r")) == NULL)
    {
    	printf("file string.txt open failed!\n");
    	return;
	}
	if((resultfp = fopen("C:/Users/zhaol/Desktop/strsearch/result.txt","w")) == NULL)
    {
    	printf("file result.txt open failed!\n");
    	return;
	}
	char str[10000];
	while(!feof(strfp))
	{
		fgets(str,10000,strfp);
		search_init(ACTree, strlen(str)-1, str);
	    ac_search(ACTree);
	}
	fclose(fp);
	fclose(resultfp);
	fclose(strfp);
	printf("程序结束...\n");
	return 0;
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
AC_STRUCT * ac_alloc()
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

//搜索之前对AC自动机初始化
void search_init(AC_STRUCT* node, long cNum, char* S)
{
	node->startPoint = node->startPoint + node->cNum;
	node->cNum = cNum;
	node->S = S;
	node->currentPoint = 0;
} 


//AC搜索
int ac_search(AC_STRUCT* node)
{
	long i;
	int flag;
	char* S = node->S;
	TSTree currentState,child;
	if(node->startPoint == 0)
		node->currentState = node->root;
	currentState = node->currentState;
	for(i=0; i<node->cNum; i++)
	{
		child=currentState->next;
//		flag=1;
		while(currentState!=node->root || child!=NULL)
		{
			if(child==NULL)
			{
				currentState = currentState->faillink;
				child = currentState->next;
				continue;
			}
			
			if(child->data == S[i])
			{
				currentState = child;
				node->currentState = child;
				node->currentPoint = i+1; 
				if(child->stateId!=0)
					Print(node);
				break;
			}
			else if(S[i] > child->data)
				child = child->rchild;
			else
				child = child->lchild;
				
		}
			 
	}
}

void Print(AC_STRUCT* node)
{
	TSTree currentNode;
	currentNode = node->currentState;
	while(currentNode!=NULL)
	{
		fprintf(resultfp,"%s  %d\n",Patterns[currentNode->stateId]->P, node->startPoint+node->currentPoint-Patterns[currentNode->stateId]->length);
		currentNode = currentNode->outlink;
	}
}






