#include <iostream>

class myStr
{
public:
    char *data; /* data[0]为有效字符串长度不包括'\0', len(data) =
                 * 实际字符串长度 + 1 = data[0] + 2, data[-1] = '\0'
                 */
    int *next;  /* 只有在data[0]>0时, next才存在.
                 * next[0]为有效字符串长度不包括'\0', len(next) = len(data) - 1,
                 * 因为next不用存储最后的'\0'
                 */
    myStr( );
    myStr( char *s );
    ~myStr( );
    void PrintStr( );
    int  StrReplace( myStr *T, myStr *V );
    void GetNext( );
    void PrintNext( );
    int  KMP( myStr *T );
    void GetNextVal( );
};

myStr::myStr( )
{
    // 构造函数, 空构造函数, 初始化data指针, data[0] = 0, next=NULL
    data  = new char[ 2 ];
    *data = 0;
    next  = new int[ 2 ];
    *next = 0;
}

myStr::myStr( char *s )
{
    // 构造函数，进行初始化，若data本身已有值，则先进行析构，再重新初始化
    int length = 0;
    while ( s[ length ] != '\0' )
        length++;
    data      = new char[ length + 2 ];
    next      = new int[ length + 1 ];
    data[ 0 ] = length;
    next[ 0 ] = length;
    for ( int i = 0; i <= data[ 0 ]; i++ )
    {
        data[ i + 1 ] = s[ i ];
    }
    GetNext( );  // 完成next数组的构建
}

myStr::~myStr( )
{
    // 析构函数，释放分配的空间
    delete[] data;
    data = NULL;
    delete[] next;
    next = NULL;
}

void myStr::GetNext( )
{
    // 生成next数组
    int i, j;
    i         = 1;
    j         = 0;
    next[ 1 ] = 0;  // 设置算法初值, 从第一个字符开始, 设置j=0
    while ( i < data[ 0 ] )  // 从字符串头开始遍历
    {
        // 相等就更新
        if ( j == 0 || data[ i ] == data[ j ] )
        {
            i++;
            j++;
            next[ i ] = j;
        }
        else
            j = next[ j ];  // 不等就递归
    }
    next[ 0 ] = data[ 0 ];
}

void myStr::GetNextVal( )
{
    // 生成优化后的next数组
    int i, j;
    i = 1;
    j = 0;
    next[ 1 ] = 0;  // 设置算法初值, 从第一个字符开始, 设置j=0, next[1] = 0
    while ( i < data[ 0 ] )
    {
        if ( j == 0 || data[ i ] == data[ j ] )  // 相等就更新
        {
            i++;
            j++;
            if ( data[ i ] != data[ j ] )
                next[ i ] = j;
            else
                next[ i ] = next[ j ];  // 相同则递归赋值
        }
        else
            j = next[ j ];  // 不等就递归
    }
}

void myStr::PrintStr( )
{
    // 打印字符串
    char *p = data;
    p++;
    while ( *p != '\0' )  // 维护字符串指针, 到'\0'停止
    {
        std::cout << *p++;
    }
    std::cout << std::endl;
}

void myStr::PrintNext( )
{
    // 打印next数组
    for ( int i = 1; i <= next[ 0 ]; i++ )  // 遍历数组
    {
        std::cout << next[ i ];
    }
    std::cout << std::endl;
}

int myStr::KMP( myStr *T )
{
    // KMP算法
    int i, j;
    i = 1;
    j = 1;  // 设置初值

    while ( i <= data[ 0 ] &&
            j <= T->data[ 0 ] )  // 遍历字符串, 主串和子串都不能超出本身
    {
        if ( j == 0 || data[ i ] == T->data[ j ] )  // 相等则更新
        {
            i++;
            j++;
        }
        else
            j = T->next[ j ];  // 不等则递归
    }

    if ( j > T->data[ 0 ] )
        return i - T->data[ 0 ];  // 成功定位, 返回头字符位置
    else
        return -1;  // 查找失败, 返回-1
}

int myStr::StrReplace( myStr *T, myStr *V )
{
    // 完成字符串匹配替换
    int pos = KMP( T );  // 确定子串位置

    if ( pos == -1 )
        return 0;  // 不存在子串返回0

    if ( T->data[ 0 ] == V->data[ 0 ] )  // 替换后字符串长度未发生变化
    {
        for ( int i = 0; i < V->data[ 0 ]; i++ )
        {
            data[ pos + i ] = V->data[ 1 + i ];
        }
    }

    else if ( T->data[ 0 ] < V->data[ 0 ] )  //替换后字符串变长
    {
        int   i      = 0;
        int   length = data[ 0 ] - T->data[ 0 ] + V->data[ 0 ];
        char *p      = new char[ length + 2 ];  // 重新分配空间

        p[ 0 ] = data[ 0 ] - T->data[ 0 ] + V->data[ 0 ];  // 维护字符串长度

        for ( int j = 1; j <= pos; j++ )  // pos前部分的copy
        {
            p[ j ] = data[ j ];
        }

        for ( ; i < V->data[ 0 ]; i++ )  // 替换字符串的copy
        {
            p[ pos + i ] = V->data[ 1 + i ];
        }

        for ( int j = 0; i + pos <= p[ 0 ]; i++, j++ )  // 子串之后的copy
        {
            p[ pos + i ] = data[ pos + j + T->data[ 0 ] ];
        }

        p[ pos + i ] = '\0';    // 维护字符串尾
        next[ 0 ]    = p[ 0 ];  // 维护next长度
        delete[] data;
        data = p;
    }

    else  // 替换后字符串变短
    {
        int i      = 0;
        int length = data[ 0 ] - T->data[ 0 ] + V->data[ 0 ];

        for ( ; i < V->data[ 0 ]; i++ )  // 在data上完成替换
        {
            data[ pos + i ] = V->data[ 1 + i ];
        }

        char *p = new char[ length + 2 ];  // 重新分配空间, 减少空间浪费
        p[ 0 ]    = length;                // 维护字符串长度
        next[ 0 ] = p[ 0 ];                // 维护next长度

        for ( int j = 1; j < pos + i; j++ )  // 完成copy
        {
            p[ j ] = data[ j ];
        }

        for ( int j = 0; i + pos <= p[ 0 ]; i++, j++ )
        {
            p[ pos + i ] = data[ pos + T->data[ 0 ] + j ];
        }

        p[ pos + i ] = '\0';  // 维护字符串尾部
        delete[] data;        // 释放原空间
        data = p;
    }
    GetNext( );  // 维护next数组
    return 1;
}

int main( )
{
    myStr *S = new myStr( ( char * )"ababaabab" );
    myStr *T = new myStr( ( char * )"aab" );
    myStr *V = new myStr( ( char * )"cedds" );
    S->PrintStr( );
    S->PrintNext( );
    S->GetNextVal( );
    S->PrintNext( );
    S->PrintStr( );
    std::cout << S->KMP( T ) << std::endl;
    S->StrReplace( T, V );
    S->PrintStr( );
    S->PrintNext( );
    S->StrReplace( V, T );
    S->PrintStr( );
    S->PrintNext( );
    return 0;
}
