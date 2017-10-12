#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

#define maxn 1000000
#define maxp 100
#define maxpnum 2000000
#define buffersize 50000000

typedef struct{
	char *P;
	int length;
}Pattern, *Pa;

Pa Patterns[maxpnum];
char* sline[1500000];
char buffer[buffersize];     //�洢��������buffer 

//************TernarySearchTrie�������Ľṹ******************** 
typedef struct TSNode{
	char data;                          //�ڵ�洢���ֽ�����
	struct TSNode *lchild, *rchild;     //ͬ���ֽ�TSNode 
	struct TSNode* next;                //��һ�ֽڶ�Ӧ��TSNode 
	struct TSNode* faillink;            //ʧЧ����
	struct TSNode* outlink;             //�������
	int stateId;                        //״̬id
} TSNode, *TSTree;


//************AC�Զ����ṹ******************* 
typedef struct {
  TSTree root;                         //���������ڵ� 
  long startPoint, cNum, currentPoint;               //�ļ�λ����ʼ�㣬���������� ,Ŀǰ��λ�� 
  int psize;                           //ģʽ������ 
  char* S;                             //���������� 
  TSTree currentState, outState;         //��ǰ�������Ľڵ�,������ڵ� 
} AC_STRUCT;


//***********ΪAC�Զ��������ڴ�**************** 
AC_STRUCT *ac_alloc();
//ΪAC�Զ������ģʽ��
int ac_add_string(AC_STRUCT *node, char *P, int M, int id);
/*
��trie�����faillink�����outputlink��ʵ���Զ��� 
���ִ��󷵻�0�����򷵻�1 
*/ 
int ac_implement(AC_STRUCT* node);
//����֮ǰ��AC�Զ�����ʼ��
void search_init(AC_STRUCT* node, long cNum, char* S); 
//AC����
int ac_search(AC_STRUCT* node);

//***********���нṹ************************** 
typedef struct{
	TSTree queue[maxn];
	int head,tail;
} Queue;
//���г�ʼ�� 
Queue* que_init();
//��ӣ��ɹ�����1�����򷵻�0 
int enqueue(Queue* q, TSTree node);
//���Ӳ��� 
TSTree dequeue(Queue* q);
//�пղ��� 
int empty(Queue* q);

//���� 
void quickSort(int left, int right); 
//��ģʽ��ϴ�� 
void flushP(int left,int right);

//*************�����������������************* 
//void preorder(TSTree node);

//�������
//void Print(AC_STRUCT* node); 

