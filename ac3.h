#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//TernarySearchTrie�������Ľṹ 
typedef struct TSNode{
	char data;  //�ڵ�洢���ֽ�����
	struct TSNode *lchild, *rchild;     //ͬ���ֽ�TSNode 
	struct TSNode* next;    //��һ�ֽڶ�Ӧ��TSNode 
//	TSNode* failLink; 
	int stateId;     //״̬id
	 
} TSNode, *TSTree;



typedef struct {
  TSTree root;     //���������ڵ� 
  int cNum;       //���������� 
  int psize;      //ģʽ������ 
  char* T;        //���������� 
  TSTree currentNode;        //��ǰ�������Ľڵ� 
  
} AC_STRUCT;

AC_STRUCT *ac_alloc(void);
int ac_add_string(AC_STRUCT *node, char *P, int M, int id); 
void preorder(TSTree node);
