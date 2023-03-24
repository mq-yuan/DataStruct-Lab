#include <algorithm>
#include <iostream>


struct Triple
{
    int i, j;
    int e;
    Triple( )
    {
        i = -1;
        j = -1;
        e = 0;
    }
    Triple( int _i, int _j, int _e )
    {
        i = _i;
        j = _j;
        e = _e;
    }
    Triple &operator=( const Triple &T )
    {
        // 重载三元组的赋值运算符
        i = T.i;
        j = T.j;
        e = T.e;
        return *this;
    }
};

bool cmp_Triple( Triple T1, Triple T2 )
{
    // 实现三元组按在矩阵中位置进行比较
    return ( T1.i <= T2.i ) && ( ( T1.i != T2.i ) || ( T1.j <= T2.j ) );
}

class myMatrix
{
public:
    int     mu, nu, tu;
    Triple *data;
    myMatrix( );
    myMatrix( int _mu, int _nu, int _tu, Triple *Data );
    ~myMatrix( );
    bool FastTransposeSMatrix( myMatrix &T );
    void PrintSMatrix( );
};

myMatrix::myMatrix( )
{
    // 空构造函数
    mu   = 0;
    nu   = 0;
    tu   = 0;
    data = NULL;
}

myMatrix::myMatrix( int _mu, int _nu, int _tu, Triple *_data )
{
    mu   = _mu;
    nu   = _nu;
    tu   = _tu;
    data = new Triple[ tu ];
    for ( int i = 0; i < tu; i++ )
        data[ i ] = _data[ i ];  // 完成三元组的copy
    std::sort( data, data + tu, cmp_Triple );
}

myMatrix::~myMatrix( )
{
    // 析构函数
    if ( data != NULL )
    {
        delete[] data;
        data = NULL;
    }
    mu = 0;
    tu = 0;
    nu = 0;
}

void myMatrix::PrintSMatrix( )
{
    printf( "mu:%d, nu:%d, tu:%d\n", mu, nu, tu );  // 输出矩阵的参数
    Triple *p = data;                               // 维护矩阵三元组
    for ( int i = 0; i < mu; i++ )
    {
        for ( int j = 0; j < nu; j++ )
        {
            if ( i == p->i && j == p->j )
            {
                printf( "%4d", p->e );  // 输出三元组
                p++;
            }
            else
                printf( "%4d", 0 );  // 输出0
        }
        printf( "\n" );
    }
}

bool myMatrix::FastTransposeSMatrix( myMatrix &T )
{
    // 快速转置, 需要num数组存放每一列有多少个三元组,
    // 需要apot存放每一列的第一个三元组是原矩阵第几个三元组
    T.mu      = nu;
    T.nu      = mu;
    T.tu      = tu;
    T.data    = new Triple[ tu ];
    int *num  = new int[ nu ];
    int *cpot = new int[ nu ];
    if ( T.tu )
    {
        // 求num
        for ( int col = 0; col < nu; col++ )
            num[ col ] = 0;
        for ( int t = 0; t < tu; ++t )
            ++num[ data[ t ].j ];
        // 求cpot
        cpot[ 0 ] = 0;
        for ( int col = 1; col < nu; ++col )
            cpot[ col ] = cpot[ col - 1 ] + num[ col - 1 ];
        // 完成转置
        for ( int p = 0; p < tu; ++p )
        {
            int col = data[ p ].j;
            int q   = cpot[ col ];
            // 三元组的修改, 行列交换
            T.data[ q ].i = data[ p ].j;
            T.data[ q ].j = data[ p ].i;
            T.data[ q ].e = data[ p ].e;
            ++cpot[ col ];
        }
    }
    return true;
}

int main( )
{
    Triple *Data;
    Data = new Triple[ 4 ]{ Triple( 1, 1, -1 ), Triple( 0, 0, 3 ),
                            Triple( 2, 0, 2 ), Triple( 0, 3, 5 ) };
    myMatrix M( 3, 4, 4, Data );
    myMatrix T;
    M.PrintSMatrix( );
    M.FastTransposeSMatrix( T );
    T.PrintSMatrix( );
    return 0;
}
