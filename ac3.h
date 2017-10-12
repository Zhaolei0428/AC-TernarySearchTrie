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
char buffer[buffersize];     //存储搜索串的buffer 

//************TernarySearchTrie三叉树的结构******************** 
typedef struct TSNode{
	char data;                          //节点存储的字节数据
	struct TSNode *lchild, *rchild;     //同级字节TSNode 
	struct TSNode* next;                //下一字节对应的TSNode 
	struct TSNode* faillink;            //失效链接
	struct TSNode* outlink;             //输出链接
	int stateId;                        //状态id
} TSNode, *TSTree;


//************AC自动机结构******************* 
typedef struct {
  TSTree root;                         //三叉树根节点 
  long startPoint, cNum, currentPoint;               //文件位置起始点，搜索串长度 ,目前串位置 
  int psize;                           //模式串个数 
  char* S;                             //搜索串序列 
  TSTree currentState, outState;         //当前搜索到的节点,待输出节点 
} AC_STRUCT;


//***********为AC自动机分配内存**************** 
AC_STRUCT *ac_alloc();
//为AC自动机添加模式串
int ac_add_string(AC_STRUCT *node, char *P, int M, int id);
/*
对trie树添加faillink，完成outputlink，实现自动机 
出现错误返回0，否则返回1 
*/ 
int ac_implement(AC_STRUCT* node);
//搜索之前对AC自动机初始化
void search_init(AC_STRUCT* node, long cNum, char* S); 
//AC搜索
int ac_search(AC_STRUCT* node);

//***********队列结构************************** 
typedef struct{
	TSTree queue[maxn];
	int head,tail;
} Queue;
//队列初始化 
Queue* que_init();
//入队，成功返回1，否则返回0 
int enqueue(Queue* q, TSTree node);
//出队操作 
TSTree dequeue(Queue* q);
//判空操作 
int empty(Queue* q);

//快排 
void quickSort(int left, int right); 
//将模式串洗牌 
void flushP(int left,int right);

//*************先序遍历，做调试用************* 
//void preorder(TSTree node);

//输出函数
//void Print(AC_STRUCT* node); 

