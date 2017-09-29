#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define maxn 1000000

//TernarySearchTrie三叉树的结构 
typedef struct TSNode{
	char data;  //节点存储的字节数据
	struct TSNode *lchild, *rchild;     //同级字节TSNode 
	struct TSNode* next;    //下一字节对应的TSNode 
	struct TSNode* faillink;       //失效链接
	struct TSNode* outlink;      //输出链接
	 
	int stateId;     //状态id
	 
} TSNode, *TSTree;

typedef struct{
	TSTree queue[maxn];
	int head,tail;
} Queue;

Queue* que_init();
//入队，成功返回1，否则返回0 
int enqueue(Queue* q, TSTree node);
TSTree dequeue(Queue* q);
int empty(Queue* q);

typedef struct {
  TSTree root;     //三叉树根节点 
  long startpoint, cNum;       //文件位置起始点，搜索串长度 
  int psize;      //模式串个数 
  char* T;        //搜索串序列 
  TSTree currentNode, outlink;        //当前搜索到的节点,待输出节点 
  
} AC_STRUCT;

AC_STRUCT *ac_alloc(void);
int ac_add_string(AC_STRUCT *node, char *P, int M, int id); 
void preorder(TSTree node);

//对trie树添加faillink，完成outputlink，实现自动机 
//出现错误返回0，否则返回1 
int ac_implement(AC_STRUCT* node); 
