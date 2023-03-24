#include <iostream>

typedef int* ElementTable;
typedef int  ElementType;
typedef int  Position;


int BinarySearch( ElementTable T,
                  ElementType  Key,
                  Position     First,
                  Position     Last )
{
    while ( First <= Last )
    {
        Position Mid = ( First + Last ) / 2;
        if ( T[ Mid ] == Key )
            return Mid;
        else if ( T[ Mid ] < Key )
            First = Mid + 1;
        else
            Last = Mid - 1;
    }
    return -1;
}


int main( )
{
    int a[ 11 ] = { 5, 13, 19, 21, 37, 56, 64, 75, 80, 88, 92 };
    int b[ 11 ] = { 5, 13, 19, 21, 37, 56, 64, 75, 80, 88, 92 };
    std::cout << BinarySearch( a, 21, 0, 10 ) + 1 << std::endl;
    std::cout << BinarySearch( b, 70, 0, 10 ) + 1 << std::endl;
    return 0;
}
