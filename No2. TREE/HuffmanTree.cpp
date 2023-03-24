#include <iostream>
#include <string>
using namespace std;

#define MAXSIZE 100
#define MINDATA -1000

typedef class TNode *PrtTNode;
class TNode;
class MinHeap;
class HuffmanTree;

class TNode
{
public:
    int      Weight;
    PrtTNode Left;
    PrtTNode Right;
    string   Tag;

    TNode( )
    {
        // 空构造函数
        Weight = 0;
        Left   = NULL;
        Right  = NULL;
    }

    TNode( int _Weight )
    {
        // 根据Weight的构造函数
        Weight = _Weight;
        Left   = NULL;
        Right  = NULL;
    }

    // 重载运算符，根据Weight进行比较
    TNode &operator=( const TNode &Node )
    {
        Weight = Node.Weight;
        Right  = Node.Right;
        Left   = Node.Left;
        Tag    = Node.Tag;
        return *this;
    }

    TNode &operator=( const int P )
    {
        Weight = P;
        return *this;
    }

    bool operator<( const TNode &Node )
    {
        return this->Weight < Node.Weight;
    }

    bool operator>( const TNode &Node )
    {
        return this->Weight > Node.Weight;
    }

    bool operator==( const TNode &Node )
    {
        return this->Weight == Node.Weight;
    }

    bool operator<=( const TNode &Node )
    {
        return this->Weight <= Node.Weight;
    }

    bool operator>=( const TNode &Node )
    {
        return this->Weight >= Node.Weight;
    }

    bool IsLeaf( )
    {
        // 判断是否为叶子结点
        return ( Left == NULL ) && ( Right == NULL );
    }

    void BuildTag( );
    void Huffman2Heap( MinHeap H );
};

class MinHeap
{
public:
    PrtTNode Data;
    int      Size;
    int      Capacity;

    MinHeap( );
    MinHeap( int MaxSize, int v[] );
    void  PercDown( int p );
    bool  Insert( TNode X );
    bool  IsFull( );
    bool  IsEmpty( );
    TNode DeleteMin( );
};

class HuffmanTree
{
public:
    PrtTNode Root;
    int      Size;
    HuffmanTree( );
    HuffmanTree( int MaxSize, int v[] );
    void printHuffmanCodes( );
};

string PreInsert( string a, string b );

MinHeap::MinHeap( )
{
    // 空最小堆的初始化
    Data             = new TNode[ MAXSIZE ];
    Data[ 0 ].Weight = MINDATA;
    Size             = 0;
    Capacity         = MAXSIZE;
}

MinHeap::MinHeap( int MaxSize, int v[] )
{
    // 最小堆的建立
    Data             = new TNode[ MAXSIZE ];
    Size             = 0;
    Capacity         = MAXSIZE;
    Data[ 0 ].Weight = MINDATA;
    for ( int i = 0; i < MaxSize; i++ )
    {
        Data[ i + 1 ] = v[ i ];
        Size++;
    }
    for ( int i = Size / 2; i > 0; i-- ) this->PercDown( i );
}

void MinHeap::PercDown( int p )
{
    // 从二叉树到最小堆的变换
    int   Parent, Child;
    TNode X;
    X = Data[ p ];
    for ( Parent = p; Parent * 2 <= Size; Parent = Child )
    {
        Child = Parent * 2;
        if ( ( Child != Size ) && ( Data[ Child ] > Data[ Child + 1 ] ) )
            Child++;
        if ( X <= Data[ Child ] )
            break;
        else
            Data[ Parent ] = Data[ Child ];
    }
    Data[ Parent ] = X;
}

bool MinHeap::Insert( TNode X )
{
    // 最小堆的插入
    int i;
    if ( this->IsFull( ) )
    {
        cout << "Heap is Full" << endl;
        return false;
    }
    i = ++Size;
    for ( ; Data[ i / 2 ] > X; i /= 2 ) { Data[ i ] = Data[ i / 2 ]; }
    Data[ i ] = X;
    return true;
}

bool MinHeap::IsFull( )
{
    // 最小都是否为满
    return Size == Capacity;
}

bool MinHeap::IsEmpty( )
{
    // 最小堆是否为空
    return Size == 0;
}

TNode MinHeap::DeleteMin( )
{
    // 最小堆的弹出操作
    int   Parent, Child;
    TNode MinItem, X;
    if ( this->IsEmpty( ) ) { cout << "Heap is Empty" << endl; }
    MinItem = Data[ 1 ];
    X       = Data[ Size-- ];
    for ( Parent = 1; Parent * 2 <= Size; Parent = Child )
    {
        Child = Parent * 2;
        if ( ( Child != Size ) && ( Data[ Child ] > Data[ Child + 1 ] ) )
            Child++;
        if ( X <= Data[ Child ] )
            break;
        else
            Data[ Parent ] = Data[ Child ];
    }
    Data[ Parent ] = X;
    return MinItem;
}

HuffmanTree::HuffmanTree( )
{
    // 空Huffmann的初始化
    Root = NULL;
    Size = 0;
}

HuffmanTree::HuffmanTree( int MaxSize, int v[] )
{
    // Huffman的建立
    Root = new TNode;
    Size = 0;
    int     i;
    MinHeap H( MaxSize, v );
    Size = H.Size;
    for ( i = 1; i < Size; i++ )
    {
        PrtTNode T    = new TNode;
        T->Left       = new TNode;
        T->Right      = new TNode;
        *( T->Left )  = H.DeleteMin( );
        *( T->Right ) = H.DeleteMin( );
        T->Weight     = T->Left->Weight + T->Right->Weight;
        H.Insert( *T );
    }
    *Root     = H.DeleteMin( );
    Root->Tag = "";
    Root->BuildTag( );
}

void HuffmanTree::printHuffmanCodes( )
{
    // 输出Huffman编码
    MinHeap H;
    Root->Huffman2Heap( H );
    TNode data[ Size ];
    H.Size = Size;
    for ( int i = 1; i <= Size; i++ ) { data[ Size - i ] = H.DeleteMin( ); }
    for ( int i = 0; i < Size; i++ )
    {
        cout << data[ i ].Weight << ":" << data[ i ].Tag << endl;
        // cout << data[i].Tag;
    }
}

void TNode::BuildTag( )
{
    // 根据Huffman创建结点编码
    Left->Tag  = PreInsert( Tag, "0" );
    Right->Tag = PreInsert( Tag, "1" );
    if ( !( Left->IsLeaf( ) ) ) Left->BuildTag( );
    if ( !( Right->IsLeaf( ) ) ) Right->BuildTag( );
}

string PreInsert( string a, string b )
{
    // string的拼接
    string dest = a + b;
    return dest;
}

void TNode::Huffman2Heap( MinHeap H )
{
    // 从Huffman到最小堆的转化
    if ( !( Left->IsLeaf( ) ) )
        Left->Huffman2Heap( H );
    else
        H.Insert( *( Left ) );
    if ( !( Right->IsLeaf( ) ) )
        Right->Huffman2Heap( H );
    else
        H.Insert( *( Right ) );
}

int main( )
{
    int         v[] = { 2, 7, 5, 4 };
    HuffmanTree hTree( 4, v );
    hTree.printHuffmanCodes( );
    return 0;
}
