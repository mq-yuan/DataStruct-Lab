#include <iostream>
#include <stack>

struct ArcNode
{
    int             adjvex;
    int             weight;
    struct ArcNode* nextarc;
    ArcNode( )
    {
        adjvex  = 0;
        nextarc = NULL;
    }
};

struct VNode
{
    int      data;
    ArcNode* firstarc;
    VNode( )
    {
        data     = 'a';
        firstarc = NULL;
    }
};

struct EdgeNode
{
    int tail;
    int dest;
    int weight;
};

class ALGraph
{
public:
    struct VNode* AdjList;
    int*          indegree;
    int*          ve;
    int*          vl;
    int           vexnum, arcnum;

    ALGraph( );
    ALGraph( int _vexnum, int _arcnum, int* _data, EdgeNode* _edge );
    ~ALGraph( );
    void PrintALGraph( );
    void PrintInDegree( );
    void Printstack( std::stack<int> S );
    int  GetValue( int _v );
    bool TopologicalOrder( std::stack<int>& T );
    void CriticalPath( );
};

ALGraph::ALGraph( )
{
    // 空析构函数
    AdjList  = NULL;
    vexnum   = 0;
    arcnum   = 0;
    indegree = NULL;
    ve       = NULL;
    vl       = NULL;
}

ALGraph::ALGraph( int _vexnum, int _arcnum, int* _data, EdgeNode* _edge )
{
    // 析构函数
    // 读入图中边和顶点的个数
    vexnum = _vexnum;
    arcnum = _arcnum;
    // 分配邻接表空间
    AdjList = new VNode[ vexnum ];
    // 分配入度数组空间&ve&vl
    indegree = new int[ vexnum ];
    ve       = new int[ vexnum ];
    vl       = new int[ vexnum ];
    // 完成邻接表顶点的构造和入度数组初始化
    for ( int i = 0; i < vexnum; i++ )
    {
        AdjList[ i ].data = _data[ i ];
        indegree[ i ]     = 0;
        ve[ i ]           = -1;
        vl[ i ]           = -1;
    }
    // 完成邻接表每个顶点的边链表构造, 头插法
    for ( int i = 0; i < arcnum; i++ )
    {
        ArcNode* edge = new ArcNode;
        edge->adjvex  = _edge[ i ].dest;
        edge->weight  = _edge[ i ].weight;
        edge->nextarc = AdjList[ _edge[ i ].tail ].firstarc;
        AdjList[ _edge[ i ].tail ].firstarc = edge;
        indegree[ edge->adjvex ]++;
    }
}

ALGraph::~ALGraph( )
{
    // 析构函数
    // 判断是否为空图
    if ( AdjList )
    {
        // 析构每个顶点的边链表
        for ( int i = 0; i < vexnum; i++ )
        {
            delete[] AdjList[ i ].firstarc;
            AdjList[ i ].firstarc = NULL;
        }
        // 析构图的邻接表
        delete[] AdjList;
    }
    AdjList = NULL;
    vexnum  = 0;
    arcnum  = 0;
}

void ALGraph::PrintALGraph( )
{
    if ( AdjList )
    {
        // 输出每个顶点的边链表
        for ( int i = 0; i < vexnum; i++ )
        {
            // 输出边链表
            ArcNode* P = AdjList[ i ].firstarc;
            while ( P )
            {
                // 输出顶点值
                std::cout << GetValue( i );
                std::cout << "-" << P->weight << "->";
                std::cout << GetValue( P->adjvex ) << ' ';

                P = P->nextarc;
            }
            std::cout << std::endl;
        }
    }
}

void ALGraph::PrintInDegree( )
{
    for ( int i = 0; i < vexnum; i++ )
    {
        std::cout << GetValue( i ) << ": " << indegree[ i ] << std::endl;
    }
}

int ALGraph::GetValue( int _v )
{
    // 输出图的第_v顶点的值
    return AdjList[ _v ].data;
}

bool ALGraph::TopologicalOrder( std::stack<int>& T )
{
    std::stack<int> S;         // 零入度栈
    int             cout = 0;  // 计数flag
    // 初始化零入度栈&最早开始时间数组
    for ( int i = 0; i < vexnum; i++ )
    {
        if ( indegree[ i ] == 0 )
        {
            S.push( i );
        }
        ve[ i ] = 0;
    }
    // 零入度栈不空
    while ( !S.empty( ) )
    {
        int i = S.top( );
        S.pop( );
        T.push( i );  // 零入度结点入拓扑栈
        cout++;
        ArcNode* P = AdjList[ i ].firstarc;
        while ( P )
        {
            if ( --indegree[ P->adjvex ] == 0 )
            {
                S.push( P->adjvex );
            }
            if ( P->weight + ve[ i ] > ve[ P->adjvex ] )
            {
                ve[ P->adjvex ] = ve[ i ] + P->weight;
            }
            P = P->nextarc;
        }
    }
    if ( cout < vexnum )
        return false;
    else
        return true;
}

void ALGraph::Printstack( std::stack<int> S )
{
    std::stack<int> s = S;
    while ( !s.empty( ) )
    {
        std::cout << GetValue( s.top( ) ) << ' ';
        s.pop( );
    }
    std::cout << std::endl;
}

void ALGraph::CriticalPath( )
{
    std::stack<int> T;
    // 如果不是有向无环图则什么都不做
    if ( !TopologicalOrder( T ) )
    {
        std::cout << "Not DAG" << std::endl;
        return;
    }
    std::cout << std::endl << "拓扑倒序" << std::endl;
    Printstack( T );
    std::cout << std::endl << "关键路径" << std::endl;
    // 初始化vl都为ve[last]
    for ( int i = 0; i < vexnum; i++ )
    {
        vl[ i ] = ve[ vexnum - 1 ];
    }
    // 求最晚开始时间
    while ( !T.empty( ) )
    {
        int i = T.top( );
        T.pop( );
        ArcNode* P = AdjList[ i ].firstarc;
        while ( P )
        {
            int j = P->adjvex;
            if ( vl[ i ] > vl[ j ] - P->weight )
            {
                vl[ i ] = vl[ j ] - P->weight;
            }
            P = P->nextarc;
        }
    }
    // 求关键路径
    for ( int i = 0; i < vexnum; i++ )
    {
        ArcNode* P = AdjList[ i ].firstarc;
        while ( P )
        {
            int j  = P->adjvex;
            int ee = ve[ i ];
            int el = vl[ j ] - P->weight;
            if ( ee == el )
            {
                std::cout << GetValue(i) << "->" << GetValue(j) << std::endl;
            }
            P = P->nextarc;
        }
    }
}

void test( )
{
    // 测试案例输入
    int  vexnum = 9;
    int  arcnum = 11;
    int* data   = new int[ vexnum ];
    for ( int i = 0; i < vexnum; i++ )
        data[ i ] = i + 1;
    EdgeNode* edge   = new EdgeNode[ arcnum ];
    edge[ 0 ].tail   = 0;
    edge[ 0 ].weight = 6;
    edge[ 0 ].dest   = 1;  // 1-6->2
    edge[ 1 ].tail   = 0;
    edge[ 1 ].weight = 4;
    edge[ 1 ].dest   = 2;  // 1-4->3
    edge[ 2 ].tail   = 0;
    edge[ 2 ].weight = 5;
    edge[ 2 ].dest   = 3;  // 1-5->4

    edge[ 3 ].tail   = 1;
    edge[ 3 ].weight = 1;
    edge[ 3 ].dest   = 4;  // 2-1->5

    edge[ 4 ].tail   = 2;
    edge[ 4 ].weight = 1;
    edge[ 4 ].dest   = 4;  // 3-1->5

    edge[ 5 ].tail   = 3;
    edge[ 5 ].weight = 2;
    edge[ 5 ].dest   = 5;  // 4-2->6

    edge[ 6 ].tail   = 4;
    edge[ 6 ].weight = 9;
    edge[ 6 ].dest   = 6;  // 5-9->7
    edge[ 7 ].tail   = 4;
    edge[ 7 ].weight = 7;
    edge[ 7 ].dest   = 7;  // 5-7->8

    edge[ 8 ].tail   = 5;
    edge[ 8 ].weight = 4;
    edge[ 8 ].dest   = 7;  // 6-4->8

    edge[ 9 ].tail   = 6;
    edge[ 9 ].weight = 2;
    edge[ 9 ].dest   = 8;  // 7-2->9

    edge[ 10 ].tail   = 7;
    edge[ 10 ].weight = 3;
    edge[ 10 ].dest   = 8;  // 8-3->9

    // 测试部分
    ALGraph*        G = new ALGraph( vexnum, arcnum, data, edge );
    std::stack<int> T;  // 拓扑栈
    std::cout << std::endl << "邻接表" << std::endl;
    G->PrintALGraph( );
    std::cout << "Indegree" << std::endl;
    G->PrintInDegree( );
    // 关键路径
    G->CriticalPath( );
    return;
}

int main( )
{
    test( );
    return 0;
}
