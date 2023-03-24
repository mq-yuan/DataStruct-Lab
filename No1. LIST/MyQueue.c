#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100
#define OVERFLOW 0
#define ERROR 0x0fffffff

typedef int ElementType;
typedef struct QNode* PrtToQNode;
typedef PrtToQNode Queue;
typedef int Position;
typedef enum{EMPTY, FULL, NORM} QTag;

struct QNode{
    ElementType* data;
    Position front;
    Position rear;
    QTag tag;
};


Queue CreateQueue();
void InitQueue(Queue Q);
void DestoryQueue(Queue Q);
bool EnQueue(Queue Q);
bool EnQueueX(Queue Q, ElementType x);
ElementType DeQueue(Queue Q);
ElementType TopQueue(Queue Q);
void ViewQueue(Queue Q);


Queue CreateQueue()
{
    Queue Q = (Queue) malloc (sizeof(QNode));
    Q->data = (ElementType*) malloc (MAX_SIZE * sizeof(ElementType));
    Q->tag = EMPTY;
    if(!(Q->data)) exit(OVERFLOW);
    InitQueue(Q);
    int num;
    do
    {
        printf("输入元素个数");
        scanf("%d", &num);
    }while(num < 1 && printf("请输入大于0的数字!\n"));
    for(; num--; EnQueue(Q));
    return Q;
}


void InitQueue(Queue Q)
{
    Q->front = 0;
    Q->rear = 0;
    Q->tag = EMPTY;
}


void DestoryQueue(Queue Q)
{
    InitQueue(Q);
    free(Q->data);
    free(Q);
}


bool EnQueue(Queue Q)
{
    ElementType x;
    printf("请输入一个数字");
    scanf("%d", &x);
    if(Q->tag == FULL && printf("发生了溢出,入队失败\n")) return false;
    Q->tag = NORM;
    Q->data[Q->rear++] = x;
    Q->rear = Q->rear % MAX_SIZE;
    if(Q->rear == Q->front) Q->tag = FULL;
    return true;
}


bool EnQueueX(Queue Q, ElementType x)
{
    if(Q->tag == FULL && printf("发生了溢出,入队失败\n")) return false;
    Q->tag = NORM;
    Q->data[Q->rear++] = x;
    Q->rear = Q->rear % MAX_SIZE;
    if(Q->rear == Q->front) Q->tag = FULL;
    return true;
}


ElementType TopQueue(Queue Q)
{
    if(Q->tag == EMPTY && printf("队列为空!\n")) return ERROR;
    ElementType x = Q->data[Q->front];
    return x;
}


ElementType DeQueue(Queue Q)
{
    ElementType x;
    x = TopQueue(Q);
    if(x == ERROR && printf("出队失败!\n")) return x;
    Q->tag = NORM;
    Q->front = (Q->front + 1) % MAX_SIZE;
    if(Q->front == Q->rear) Q->tag = EMPTY;
    return x;
}


void ViewQueue(Queue Q)
{
    Position P0 = Q->front, P1 = Q->rear;
    if(Q->tag == EMPTY && printf("队列为空!\n")) return ;
    if(Q->tag == FULL) P1 = P1 + MAX_SIZE;
    for(; P0 < P1; printf("%d ", Q->data[(P0++) % MAX_SIZE]));
    printf("\n");
}


void YANGHUI(int n)
{
    Queue Q = (Queue) malloc (sizeof(QNode));
    Q->data = (ElementType*) malloc (MAX_SIZE * sizeof(ElementType));
    Q->tag = EMPTY;
    if(!(Q->data)) exit(OVERFLOW);
    InitQueue(Q);
    EnQueueX(Q, 1); EnQueueX(Q, 1);
    int s = 0;
    for(int i=0; i<=n; i++)
    {
        printf("\n");
        EnQueueX(Q, 0);
        for(int j=0; j<=i+2; j++)
        {
            int t = DeQueue(Q);
            EnQueueX(Q, s + t);
            s = t;
            if(j != i+2) printf("%d ", s);
        }
    }
}


void test1()
{
    printf("进行Queue的测试");
    Queue Q = CreateQueue();
    ViewQueue(Q);
    DeQueue(Q);
    EnQueue(Q);
    ViewQueue(Q);
    printf("初始化队列\n");
    InitQueue(Q);
    printf("判断空队列的出队与访问\n");
    DeQueue(Q);
    ViewQueue(Q);
    printf("测试一次入队和一次出队操作\n");
    EnQueue(Q);
    DeQueue(Q);
    printf("此时队列为空\n");
    for(int i=0; i<5; EnQueue(Q), i++);
    ViewQueue(Q);
    DestoryQueue(Q);
}


void test2()
{
    printf("进行杨辉三角的测试");
    int n;
    scanf("%d", &n);
    YANGHUI(n);
}


int main()
{
    test1();
    test2();
    return 0;
}
