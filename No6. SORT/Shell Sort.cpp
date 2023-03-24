#include <iostream>

typedef int* ElementTable;
typedef int  ElementType;
typedef int  Position;


void ShellSort( ElementType A[], int N )
{
    int         Si, D, P, i;
    ElementType Tmp;

    int Sedgewick[] = { 929, 505, 209, 109, 41, 19, 5, 1, 0 };

    for ( Si = 0; Sedgewick[ Si ] >= N; Si++ )
        ;

    for ( D = Sedgewick[ Si ]; D > 0; D = Sedgewick[ ++Si ] )
        for ( P = D; P < N; P++ )
        {
            Tmp = A[ P ];
            for ( i = P; i >= D && A[ i - D ] > Tmp; i -= D )
                A[ i ] = A[ i - D ];
            A[ i ] = Tmp;
        }
}


int main( )
{
    ElementType a[ 6 ] = { 21, 25, 49, 25, 16, 8 };
    for ( int i = 0; i < 6; i++ )
    {
        std::cout << a[ i ] << ' ';
    }
    std::cout << std::endl;
    ShellSort( a, 6 );
    for ( int i = 0; i < 6; i++ )
    {
        std::cout << a[ i ] << ' ';
    }
    std::cout << std::endl;
}
