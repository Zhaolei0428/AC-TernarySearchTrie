#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define maxn 1000000

//TernarySearchTrie�������Ľṹ 
typedef struct TSNode{
	char data;  //�ڵ�洢���ֽ�����
	struct TSNode *lchild, *rchild;     //ͬ���ֽ�TSNode 
	struct TSNode* next;    //��һ�ֽڶ�Ӧ��TSNode 
	struct TSNode* faillink;       //ʧЧ����
	struct TSNode* outlink;      //�������
	 
	int stateId;     //״̬id
	 
} TSNode, *TSTree;

typedef struct{
	TSTree queue[maxn];
	int head,tail;
} Queue;

Queue* que_init();
//��ӣ��ɹ�����1�����򷵻�0 
int enqueue(Queue* q, TSTree node);
TSTree dequeue(Queue* q);
int empty(Queue* q);

typedef struct {
  TSTree root;     //���������ڵ� 
  long startpoint, cNum;       //�ļ�λ����ʼ�㣬���������� 
  int psize;      //ģʽ������ 
  char* T;        //���������� 
  TSTree currentNode, outlink;        //��ǰ�������Ľڵ�,������ڵ� 
  
} AC_STRUCT;

AC_STRUCT *ac_alloc(void);
int ac_add_string(AC_STRUCT *node, char *P, int M, int id); 
void preorder(TSTree node);

//��trie�����faillink�����outputlink��ʵ���Զ��� 
//���ִ��󷵻�0�����򷵻�1 
int ac_implement(AC_STRUCT* node); 
