#include <stdio.h>
#include <stdlib.h>
#define ERROR NULL
#define ELEMENTERROR 2147483647


typedef int Elementtype;
typedef struct LNode* PrtToLNode;
typedef PrtToLNode List;
typedef int Position;

struct LNode{
    Elementtype Data; // 结点元素的值
    PrtToLNode Next; // 下一个结点的位置
};


PrtToLNode CreateLNode(Elementtype x);
List InitializeList();
void DestoryList(List L);
bool InsertHeadList(List L, Elementtype x);
bool InsertLastList(List L, Elementtype x);
bool InsertList(List L, Position P, Elementtype x);
Elementtype DeleteElement(List L, Position P);
PrtToLNode FindListElement(List L, Elementtype x);
PrtToLNode FindListPosition(List L, Position P);
void InSortList(List L);
void ReverseList(List L);
void PrintList(List L);
bool IsEmptyList(List L);
void SwapNodeData(PrtToLNode P1, PrtToLNode P2);
void merge(List A, List B);
// 归并排序的函数声明
void mergeSortList(List head);
void mergeList(List L1,List L2);

PrtToLNode CreateLNode(Elementtype x)
{
    // 创建一个值为x的新结点
    PrtToLNode L = (PrtToLNode) malloc (sizeof(LNode));
    L->Data = x; L->Next = NULL;
    return L;
}

List InitializeList()
{
    // 创建一个新链表返回头结点
    List L = (List) malloc (sizeof(LNode));
    L->Data = ELEMENTERROR; L->Next = NULL; // 头结点的值为ELEMENTERROR
    return L;
}

void DestoryList(List L)
{
    // 销毁链式表L
    PrtToLNode pre, P = L->Next;
    // 释放链式表所有结点
    while(P)
    {
        pre = P; P = P->Next; free(pre);
    }
    free(L); L = NULL; // 释放头结点
}

bool InsertHeadList(List L, Elementtype x)
{
    // 在链式表L的头结点后插入一个值为x的新结点，返回bool类型代表插入是否成功。
    if(L == NULL) return false; // 判断非法输入
    PrtToLNode P = L->Next;
    PrtToLNode tmp = CreateLNode(x); // 创建一个新结点
    L->Next = tmp; tmp->Next = P; // 实现插入
    return true;
}

bool InsertLastList(List L, Elementtype x)
{
    // 在链式表L的最后一个结点后插入一个值为x的新结点，返回bool类型代表插入是否成功。
    if(L == NULL) return false; // 判断发非法输入
    PrtToLNode P = L->Next;
    PrtToLNode tmp = CreateLNode(x); // 创建新结点
    // 判断空链表的特殊化处理
    if(IsEmptyList(L))
    {
        L->Next = tmp; // 实现插入
        return true;
    }
    for(; P->Next != NULL; P = P->Next); // 找到最后一个结点
    P->Next = tmp; // 实现插入
    return true;
}

bool InsertList(List L, Position P, Elementtype x)
{
    // 在链式表L的第P+1个结点处插入一个值为x的新结点，返回bool类型代表插入是否成功。
    PrtToLNode tmp, pre = L;
    Position i=0;
    tmp = CreateLNode(x); // 创建新结点
    // 找到应插入的位置
    while((i < P) & (pre != NULL))
    {
        pre = pre->Next;
        i++;
    }
    if(pre == NULL) return false; // 无法找到，返回false
    tmp->Next = pre->Next; pre->Next = tmp; // 实现插入
    return true;
}

Elementtype DeleteElement(List L, Position P)
{
    // 删除链式表L的第P+1个结点，返回该结点的数据元素的值。
    if(IsEmptyList(L)) return ELEMENTERROR;
    Position i=0;
    PrtToLNode tmp, pre = L;
    Elementtype res;
    // 找到第P+1个结点
    while((i < P) & (pre != NULL))
    {
        pre = pre->Next;
        i++;
    }
    if(pre == NULL || pre->Next == NULL) return ELEMENTERROR; // 无法找到，返回ELEMENTERROR
    tmp = pre->Next; pre->Next = tmp->Next; res = tmp->Data; // 删除
    free(tmp); // 释放空间
    return res;
}

PrtToLNode FindListElement(List L, Elementtype x)
{
    // 查找链式表L的第一个数据元素的值为x的结点，返回该结点。
    if(IsEmptyList(L)) return ERROR; // 链式表为空，返回ERROR
    PrtToLNode P = L->Next;
    for(; (P->Data != x) & (P != NULL); P = P->Next); // 查找第一个值为x的结点
    if(P == NULL) return ERROR; // 无法找到返回ERROR
    return P;
}

PrtToLNode FindListPosition(List L, Position P)
{
    // 返回链式表L的第P+1个结点的数据元素的值
    if(IsEmptyList(L)) return NULL; // 链式表为空，返回NULL
    PrtToLNode tmp = L->Next;
    Position i=0;
    // 查找第P+1个结点
    while((i < P) & (tmp != NULL))
    {
        tmp = tmp->Next;
        i++;
    }
    if(tmp == NULL) return ERROR; // 无法找到，返回ERROR
    return tmp;
}

void InSortList(List L)
{
    // 在原空间上对链式表L进行递增排序
    if(IsEmptyList(L)) return; // 链式表为空什么都不做
    PrtToLNode P = L->Next, MinP = L->Next, tmp = NULL;
    while(P->Next != NULL)
    {
        // 利用选排，先找到最小的元素
        MinP = P;
        for(tmp = P->Next; tmp != NULL; tmp = tmp->Next)
        {
            if(tmp->Data < MinP->Data) MinP = tmp;
        }
        SwapNodeData(P, MinP); // 把最小的元素放到本次查找的最前方
        P = P->Next; // 移动指针P，修改本次查找的起始位置
    }
}

void ReverseList(List L)
{
    // 在原空间上对链式表L进行倒序排列
    if(IsEmptyList(L)) return; // 链式表为空，什么都不做
    PrtToLNode tmp = L, pre = L->Next;
    tmp->Next = NULL;
    // 倒序排列的主体部分，把第一个结点放头结点后第一个位置
    while(pre != NULL)
    {
        tmp = pre; pre = pre->Next;
        tmp->Next = L->Next; L->Next = tmp;
    }
}

void PrintList(List L)
{
    // 按序输出链式表L的所有结点的数据元素的值。
    if(IsEmptyList(L)) return; // 链式表为空，什么都不做
    PrtToLNode P = L->Next;
    // 遍历链式表
    while(P->Next != NULL)
    {
        printf("%d ", P->Data);
        P = P->Next;
    }
    printf("%d\n", P->Data); // 最后一个输出时，输出换行符
}

bool IsEmptyList(List L)
{
    // 判断链式表是否为空，通过头结点
    return (L->Next == NULL);
}

void SwapNodeData(PrtToLNode P1, PrtToLNode P2)
{
    // 交换结点P1和结点P2的数据元素的值。
    if(P1 == NULL || P2 == NULL || P1->Data == ELEMENTERROR || P2->Data == ELEMENTERROR) return; // 非法输入，什么都不做
    Elementtype tmp = P1->Data;
    P1->Data = P2->Data;
    P2->Data = tmp;
}

void merge(List A, List B)
{
    // 先对链式表A和B进行递增排序，再将A和B归并为一个按元素值递减次序排列的单链表，利用原来两个单链表的结点存放归并后的单链表,结果存在A线性表中。
    // 对链式表进行递增排序
    InSortList(A);
    InSortList(B);
    List L = A;
    List p1, p2, p;
    p1 = A->Next; p2 = B->Next;
    L->Next = NULL;
    // 双指针遍历A和B
    while((p1 != NULL) & (p2 != NULL))
    {
        // 将A中结点插入到A后
        if(p1->Data < p2->Data)
        {
            p = p1;
            p1 = p1->Next;
            p->Next = L->Next;
            L->Next = p;
        }
        // 将B中结点插入到A中
        else
        {
            p = p2;
            p2 = p2->Next;
            p->Next = L->Next;
            L->Next = p;
        }
    }
    // 将未遍历的链式表A与B的结点插入到A后
    while(p1)
    {
        p = p1;
        p1 = p1->Next;
        p->Next = L->Next;
        L->Next = p;
    }
    while(p2)
    {
        p = p2;
        p2 = p2->Next;
        p->Next = L->Next;
        L->Next = p;
    }
    free(B); // 释放链式表B的头结点
}

void mergeSortList(List head)
{
    if(IsEmptyList(head) || head->Next->Next == NULL) return; // 若链表只有头结点或只有一个元素则返回本身
    List slow = head->Next;    //慢指针
    List fast = head->Next;    //快指针
    // 找到链表的中间节点
    while(fast->Next && fast->Next->Next)
    {
        slow = slow->Next;
        fast = fast->Next->Next;
    }
    // 断开
    List breakNode = InitializeList(); // 建立新链表头结点
    breakNode->Next = slow->Next;  //断点
    slow->Next=NULL;  //打断结点

    mergeSortList(head); //递归左链表
    mergeSortList(breakNode); //递归右链表

    mergeList(head,breakNode);  //合并链表
}

void mergeList(List L1,List L2)
{
    List p1, p2, p;
    p1 = L1; p2 = L2;
    // 双指针遍历L1和L2
    while((p1->Next != NULL) & (p2->Next != NULL))
    {
        // 将A中结点插入到A后
        if(p1->Next->Data <= p2->Next->Data)
        {
            p1 = p1->Next;
        }
        // 将B中结点插入到A中
        else
        {
            p = p2->Next;
            p2->Next = p->Next;
            p->Next = p1->Next;
            p1->Next = p;
        }
    }
    // 将未遍历的链式表A与B的结点插入到A后
    while(p2->Next)
    {
        p = p2->Next;
        p2->Next = p->Next;
        p->Next = p1->Next;
        p1->Next = p;
    }
    free(L2); // 释放链式表B的头结点
}

int main()
{
    // 创建两个带头结点的链式表A和B
    List A, B;
    A = InitializeList();
    B = InitializeList();
    // 在A和B中插入一些数据元素
    for(int i = 1; i <= 10; i++)
    {
        InsertLastList(A, 2 * i + 7); // 每次在A的最后一个位置插入元素
        InsertLastList(B, 5 * i - 10); // 每次在B的最后一个位置插入元素
    }
    // 链式表A插入后数据元素的值9 11 13 15 17 19 21 23 25 27
    // 链式表B插入后数据元素的值-5 0 5 10 15 20 25 30 35 40
    PrintList(A); // 输出判断链式表A的插入过程是否有误
    PrintList(B); // 输出判断链式表A的插入过程是否有误
    // 在链式表A和B中随机位置插入数据
    InsertList(A, 5, 222);
    InsertList(B, 6, 123);
    // 在A和B的第一个位置插入元素
    InsertHeadList(A, -200);
    InsertHeadList(B, -100);
    // 在A和B的最后一个位置插入元素
    InsertLastList(A, 389);
    InsertLastList(B, 938);
    // 链式表A插入后数据元素的值-200 9 11 13 15 17 222 19 21 23 25 27 389
    // 链式表B插入后数据元素的值-100 -5 0 5 10 15 20 123 25 30 35 40 983
    // 输出判断链式表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 在A和B随机位置删除元素
    DeleteElement(A, 1);
    DeleteElement(B, 10);
    // 链式表A删除数据元素的值-200 11 13 15 17 222 19 21 23 25 27 389
    // 链式表B删除数据元素的值-100 -5 0 5 10 15 20 123 25 30 40 983
    // 输出判断链式表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 倒序排列链式表A和B的所有元素
    ReverseList(A);
    ReverseList(B);
    // 链式表A倒序排列数据元素后的值389 27 25 23 21 19 222 17 15 13 11 -200
    // 链式表B倒序排列数据元素后的值983 40 30 25 123 20 15 10 5 0 -5 -100
    // 输出判断链式表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 检测链式表A的按值查找和按位置查找操作
    printf("%d, %d\n", FindListPosition(A, 10)->Data, FindListElement(A, 17)->Data);
    // 在原空间上对链式表A和B进行递增排序
    InSortList(A);
    InSortList(B);
    // 链式表A递增排列数据元素后的值-200 11 13 15 17 19 21 23 25 27 222 389
    // 链式表B递增排列数据元素后的值-100 -5 0 5 10 15 20 25 30 40 123 938
    // 输出判断链式表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 合并A和B
    merge(A, B);
    // 链式表A和B合并并递减排列数据元素后的值938 389 222 123 40 30 27 25 25 23 21 20 19 17 15 15 13 11 10 5 0 -5 -100 -200
    // 输出判断合并后表A是否有误
    PrintList(A);
    // 测试归并排序链式表A归并排序后值为-200 -100 -5 0 5 10 11 13 15 15 17 19 20 21 23 25 25 27 30 40 123 222 389 938
    mergeSortList(A);
    PrintList(A);
    DestoryList(A); // 销毁链式表A
    return 0;
}
