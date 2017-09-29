#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//TernarySearchTrie三叉树的结构 
typedef struct TSNode{
	char data;  //节点存储的字节数据
	struct TSNode *lchild, *rchild;     //同级字节TSNode 
	struct TSNode* next;    //下一字节对应的TSNode 
//	TSNode* failLink; 
	int stateId;     //状态id
	 
} TSNode, *TSTree;



typedef struct {
  TSTree root;     //三叉树根节点 
  int cNum;       //搜索串长度 
  int psize;      //模式串个数 
  char* T;        //搜索串序列 
  TSTree currentNode;        //当前搜索到的节点 
  
} AC_STRUCT;

AC_STRUCT *ac_alloc(void);
int ac_add_string(AC_STRUCT *node, char *P, int M, int id); 
void preorder(TSTree node);
