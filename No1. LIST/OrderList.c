#include <stdio.h>
#include <stdlib.h>
#define ERROR 2147483647


typedef int Elementtype;
typedef int Position;
typedef struct LNode *List;


struct LNode
{
    Elementtype *Data;
    Position LastPos;  // 顺序表最后一个元素下标
    Position ListSize; // 顺序表最大保存元素个数
};


List InitializeList(Position ListSize);
void DestroyList(List L);
bool InsertList(List L, Elementtype x, Position P);
Elementtype DeleteElement(List L, Position P);
Elementtype FindPosition(List L, Position P);
Position FindElement(List L, Elementtype x);
void ReverseList(List L);
void PrintList(List L);
bool IsEmptyList(List L);
bool IsFullList(List L);
void SwapListPosition(List L, Position P1, Position P2);


List InitializeList(Position ListSize)
{
    /*
     * 返回一个设置最大存储元素数量为ListSize+1的空顺序表。
     * 空顺序表最后一个元素的位置记为-1。
     */
    List L;
    L = (List)malloc(sizeof(struct LNode));
    L->Data = (Elementtype *)malloc(ListSize * sizeof(Elementtype));
    L->LastPos = -1;
    L->ListSize = ListSize;
    return L;
}

bool IsEmptyList(List L)
{
    /*
     * 利用空顺序表最后一个元素的位置记为-1，判断L是否为空。
     */
    return (L->LastPos == -1);
}

bool IsFullList(List L)
{
    /*
     * 利用顺序表的LastPos与ListSize来判断顺序表是否为满。
     * 由于顺序表的Position是从0开始计数，故当LastPos+1==ListSize时，即为满。
     */
    return (L->LastPos + 1 == L->ListSize);
}

bool InsertList(List L, Elementtype x, Position P)
{
    /*
     * 在顺序表L的第P+1个数据元素的位置插入值为x的数据元素，L的长度+1。
     * 先判断L是否已满，已满则什么都不做返回false。
     * 返回bool类型判断插入是否成功。
     */
    if (IsFullList(L)) return false; // 判断L是否已满
    // 将第P+1个及以后所有数据元素都向后移动一位。
    for(Position i = L->LastPos; P <= i; i--){
        L->Data[i+1] = L->Data[i];
    }
    L->Data[P] = x; // 赋值语句
    L->LastPos++; // 表长+1
    return true;
}

Elementtype DeleteElement(List L, Position P)
{
    /*
     * 删除顺序表L的第P+1个位置的数据元素，L的长度-1。
     * 并返回被删除数据元素的值。
     */
    if (L->LastPos < P || P < 0) return false; // 判断非法输入
    Elementtype res = L->Data[P]; // 记录被删除元素的值
    // 将第P+1个及以后所有数据元素都向前移动一位
    while (P < L->LastPos)
    {
        L->Data[P] = L->Data[P + 1];
        P++;
    }
    L->LastPos--; // 顺序表长度-1
    return res; // 返回被删除元素的值
}

Position FindElement(List L, Elementtype x)
{
    /*
     * 返回顺序表L的第1个值为x的数据元素的位置。
     */
    Position res = 0; //记录元素位置
    // 遍历顺序表寻找第一个值为x的数据元素
    while (res <= L->LastPos)
    {
        if (L->Data[res] == x) return res; // 找到则返回位置
        res++;
    }
    return -1; // 无法找到返回-1记录错误
}

Elementtype FindPosition(List L, Position P)
{
    /*
     * 返回顺序表L的第P+1个数据元素的值
     */
    if (P > L->LastPos) return ERROR; // 判断非法输入
    return L->Data[P]; // 返回值
}

void PrintList(List L)
{
    /*
     * 按序输出顺序表L的所有元素的值
     */
    Position P = 0;
    while(P < L->LastPos) printf("%d ", L->Data[P++]);
    printf("%d\n", L->Data[P]); // 最后一个元素输出后，带着分行符
}

void DestroyList(List L)
{
    /*
     * 销毁顺序表L
     */
    free(L->Data); // 销毁数组
    L->Data = NULL;
    free(L); // 销毁顺序表
}

void ReverseList(List L)
{
    /*
     * 在原空间上倒序排列顺序表L的所有元素
     */
    if (IsEmptyList(L)) return; // 空顺序表什么也不做
    Position P;
    for (P = 0; P < (L->LastPos + 1) / 2; P++) SwapListPosition(L, P, L->LastPos - P); // 倒序排列顺序表L
}

void SwapListPosition(List L, Position P1, Position P2)
{
    /*
     * 交换顺序表L第P1+1个数据元素和第P2+1个数据元素的值。
     */
    if (P1 < 0 || P2 < 0 || P1 > L->LastPos || P2 > L->LastPos) return; // 判断非法输入
    // 交换元素的值
    Elementtype tmp = L->Data[P1];
    L->Data[P1] = L->Data[P2];
    L->Data[P2] = tmp;
}

int main()
{
    // 创建两个最大存储数量为100的顺序表A和B
    List A, B;
    A = InitializeList(100);
    B = InitializeList(100);
    // 在A和B中插入一些数据元素
    for(int i = 1; i <= 10; i++)
    {
        InsertList(A, 2 * i + 7, A->LastPos+1); // 每次在A的最后一个位置插入元素
        InsertList(B, 5 * i - 10, B->LastPos+1); // 每次在B的最后一个位置插入元素
    }
    // 顺序表A插入后数据元素的值9 11 13 15 17 19 21 23 25 27
    // 顺序表B插入后数据元素的值-5 0 5 10 15 20 25 30 35 40
    PrintList(A); // 输出判断顺序表A的插入过程是否有误
    PrintList(B); // 输出判断顺序表A的插入过程是否有误
    // 在顺序表A和B中随机位置插入数据
    InsertList(A, 222, 5);
    InsertList(B, 123, 6);
    // 在A和B的第一个位置插入元素
    InsertList(A, -200, 0);
    InsertList(B, -100, 0);
    // 在A和B的最后一个位置插入元素
    InsertList(A, 389,  A->LastPos+1);
    InsertList(B, 983, B->LastPos+1);
    // 顺序表A插入后数据元素的值-200 9 11 13 15 17 222 19 21 23 25 27 389
    // 顺序表B插入后数据元素的值-100 -5 0 5 10 15 20 123 25 30 35 40 983
    // 输出判断顺序表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 在A和B随机位置删除元素
    DeleteElement(A, 1);
    DeleteElement(B, 10);
    // 顺序表A删除数据元素的值-200 11 13 15 17 222 19 21 23 25 27 389
    // 顺序表B删除数据元素的值-100 -5 0 5 10 15 20 123 25 30 40 983
    // 输出判断顺序表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 倒序排列顺序表A和B的所有元素
    ReverseList(A);
    ReverseList(B);
    // 顺序表A倒序排列数据元素后的值389 27 25 23 21 19 222 17 15 13 11 -200
    // 顺序表B倒序排列数据元素后的值983 40 30 25 123 20 15 10 5 0 -5 -100
    // 输出判断顺序表A和B的插入是否有误
    PrintList(A);
    PrintList(B);
    // 检测顺序表A的按值查找和按位置查找操作
    printf("%d, %d\n", FindElement(A, 222), FindPosition(A, 7));
    DestroyList(A); // 销毁顺序表A
    // 检测顺序表B的按值查找和按位置查找操作
    printf("%d, %d\n", FindElement(B, 389), FindPosition(B, 7));
    DestroyList(B); // 销毁顺序表B
    return 0;
}
