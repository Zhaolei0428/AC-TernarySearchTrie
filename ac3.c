#include "ac3.h"
FILE *resultfp;


int main(int argc, char* argv[])
{
	printf("����ʼ\n");
	long lSize=buffersize;
	size_t result;
	DWORD start, end;
	start=GetTickCount();
	AC_STRUCT *ACTree = ac_alloc();
	ACTree->psize = 0;
    FILE *fp, *strfp;
    if((fp = fopen(argv[2],"r")) == NULL)
    {
    	printf("file pattern.txt open failed!\n");
    	return 0;
	}
    int i=1,j;
    while(!feof(fp))
    {
    	char* p = malloc(sizeof(char) * 100);
    	fgets(p,100,fp);
    	p[strlen(p)-1]='\0';     //ȥ�����з� 
    	sline[i++]=p;	
	}
	end=GetTickCount();
	printf("������� %d\n",(end - start)/1000);
	quickSort(1,--i);     //���� 
	end=GetTickCount();
	printf("������� %d\n",(end - start)/1000);
	flushP(1,i);          //ϴ�� 
	end=GetTickCount();
	printf("ϴ�ƽ��� %d\n",(end - start)/1000);
	for(j=1;j<=i;j++)
	{
		ac_add_string(ACTree, Patterns[j]->P, Patterns[j]->length, j);
	}
	fclose(fp);
	end=GetTickCount();
	printf("������� %d\n",(end - start)/1000);
	
	ac_implement(ACTree);
	end=GetTickCount();
    printf("�Զ������ %d\n",(end - start)/1000);
    
	if((strfp = fopen(argv[1],"rb"))== NULL)
    {
    	printf("file string.txt open failed!\n");
    	return 0;
	}
	
	if((resultfp = fopen(argv[3],"w")) == NULL)
    {
    	printf("file result.txt open failed!\n");
    	return 0;
	}
	
//	char str[10000];
	while(!feof(strfp))
	{
//		fgets(str,10000,strfp);
        result = fread(buffer,1,lSize,strfp);
        printf("%u\n",result);
		search_init(ACTree, result, buffer);
	    ac_search(ACTree);
	}
	fclose(resultfp);
	fclose(strfp);
	end=GetTickCount();
	printf("time: %d s\n",(end - start)/1000);
	return 0;
}//end main 

//��ģʽ��ϴ�� 
void flushP(int left,int right)
{
	static int i=1;
	int mid=(left+right)/2;
	Pa p=malloc(sizeof(Pattern));
	p->P=sline[mid];
	p->length = strlen(sline[mid]);
	Patterns[i++]=p;
	if(left<mid)
		flushP(left,mid-1);
	if(mid<right)
		flushP(mid+1,right);
} 

//�洢���ڵ�Ķ��У�����ʵ��faillink
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
//�����п� 
int empty(Queue* q)
{
	if(q->head==q->tail)
		return 1;
	return 0;
}

//��ӣ��ɹ�����1�����򷵻�0 
inline int enqueue(Queue* q, TSTree node)
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
//���ӣ����س��ӽڵ��NULL���ӿգ� 
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
 * Parameters:   node      -  an AC_STRUCT structure
 *               P         -  the sequence
 *               M         -  the sequence length
 *               id        -  the sequence identifier
 */
int ac_add_string(AC_STRUCT *node, char *P, int M, int id)
{
	int i=0,j,flag;
	TSTree currentNode, preNode, newNode; 
	if(id==0||id<=node->psize)
	   return 0;
	
    if(node->psize==0)  //��δ���ģʽ�� 
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
	//���Ѵ��ڣ�����1����id 
	if(i==M)
	{
		preNode->stateId=id;
		node->psize++;
		return 1;
	} 
	
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
	//��������򷵻� 
	if(i==M)
	{
		node->psize++;
		return 1;
	}
		
	//��ʣ���ַ����� 
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

//��trie�����faillink�����outputlink��ʵ���Զ��� 
int ac_implement(AC_STRUCT* node)
{
	TSTree root,currentNode,preNode,lchild,rchild,state,tempNode;
	root=node->root;
	int flag;
	Queue* q1, *q2, *q3;
	q1=que_init();      //��Ҫ���� 
	q2=que_init();      //�������� 
	q3=que_init();      //�������� 
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
			     	
			     	//����currentNode��faillink
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
									//����outlink 
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

//����֮ǰ��AC�Զ�����ʼ��
void search_init(AC_STRUCT* node, long cNum, char* S)
{
	node->startPoint = node->startPoint + node->cNum;
	node->cNum = cNum;
	node->S = S;
	node->currentPoint = 0;
} 


//AC����
int ac_search(AC_STRUCT* node)
{
	long i;
	int flag;
	char* S = node->S;
	TSTree currentState,child,outNode;
	if(node->startPoint == 0)
		node->currentState = node->root;
	currentState = node->currentState;
	for(i=0; i<node->cNum; i++)
	{
		child=currentState->next;
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
				{
					node->outState=outNode=child;
					while(outNode!=NULL)
					{
						fprintf(resultfp,"%s  %d\n",Patterns[outNode->stateId]->P, node->startPoint+node->currentPoint-Patterns[outNode->stateId]->length);
						outNode = outNode->outlink;
					}
				}
				else if(child->outlink!=NULL)
				{
					node->outState=outNode=child->outlink;
					while(outNode!=NULL)
					{
						fprintf(resultfp,"%s  %d\n",Patterns[outNode->stateId]->P, node->startPoint+node->currentPoint-Patterns[outNode->stateId]->length);
						outNode = outNode->outlink;
					}
				} 
				break;
			}
			else if(S[i] > child->data)
				child = child->rchild;
			else
				child = child->lchild;
				
		}
			 
	}
}

//���ţ���ģʽ������ 
void quickSort(int left, int right)
{
	if(left>=right)
	     return;
	int i=left;
	int j=right; 
	int mid=(i+j)/2;
	char* key=sline[mid];
	sline[mid]=sline[left];
	
	while(i<j)
	{
		while(i<j && strcmp(key,sline[j])<=0)
			j--;
		sline[i]=sline[j];
		while(i<j && strcmp(key,sline[i])>=0)
			i++;
		sline[j]=sline[i];
	}
	sline[i]=key;
	quickSort(left,i-1);
	quickSort(i+1,right);
}

//void Print(AC_STRUCT* node)
//{
//	TSTree currentNode = node->outState;
//	while(currentNode!=NULL)
//	{
//		fprintf(resultfp,"%s  %d\n",Patterns[currentNode->stateId]->P, node->startPoint+node->currentPoint-Patterns[currentNode->stateId]->length);
//		currentNode = currentNode->outlink;
//	}
//}

//�ȸ������ �������� 
//void preorder(TSTree node){
//	TSTree currentNode;
//	currentNode=node;
//	if(currentNode!=NULL)
//	{
//		printf("%c %d...",currentNode->data,currentNode->stateId);
//		if(currentNode->faillink!=NULL)
//			printf("%c %d\n",currentNode->faillink->data,currentNode->faillink->stateId);
//		else
//		    printf("\n");
//		preorder(currentNode->lchild);
//		preorder(currentNode->next);
//		preorder(currentNode->rchild);
//	}
//}
