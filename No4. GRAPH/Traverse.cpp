#include <iostream>
#include <queue>
#include <stack>


struct ArcNode
{
    int             adjvex;
    struct ArcNode* nextarc;
    ArcNode( )
    {
        adjvex  = 0;
        nextarc = NULL;
    }
};

struct VNode
{
    char     data;
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
};

class ALGraph
{
public:
    struct VNode* AdjList;
    int           vexnum, arcnum;

    ALGraph( );
    ALGraph( int _vexnum, int _arcnum, char* _data, EdgeNode* _edge );
    ~ALGraph( );
    void PrintALGraph( );
    char GetValue( int _v );
    int  GetFirstNeighbor( int _v );
    int  GetNextNeighbor( int _v, int _w );
    void DFS( int _v, int* visted, int* vex, int& begin );
    void BFS( int _v, int* visted, int* vex, int& begin );
    int* Graph_DTraverse( );
    int* Graph_BTraverse( );
    bool isConnected( );
    void PrintConnected( int* vex );
};


ALGraph::ALGraph( )
{
    // 空析构函数
    AdjList = NULL;
    vexnum  = 0;
    arcnum  = 0;
}

ALGraph::ALGraph( int _vexnum, int _arcnum, char* _data, EdgeNode* _edge )
{
    // 构造函数
    // 读入图中边和顶点的个数
    vexnum = _vexnum;
    arcnum = _arcnum;
    // 分配邻接表空间
    AdjList = new VNode[ vexnum ];
    // 完成邻接表顶点的构造
    for ( int i = 0; i < vexnum; i++ )
    {
        AdjList[ i ].data = _data[ i ];
    }
    // 完成邻接表每个顶点的边链表构造, 头插法
    for ( int i = 0; i < arcnum; i++ )
    {
        ArcNode* edge = new ArcNode;
        edge->adjvex  = _edge[ i ].dest;
        edge->nextarc = AdjList[ _edge[ i ].tail ].firstarc;
        AdjList[ _edge[ i ].tail ].firstarc = edge;
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
            // 输出顶点值
            std::cout << GetValue( i );
            // 输出边链表
            ArcNode* P = AdjList[ i ].firstarc;
            while ( P )
            {
                std::cout << "->";
                std::cout << GetValue( P->adjvex );
                P = P->nextarc;
            }
            std::cout << std::endl;
        }
    }
}

char ALGraph::GetValue( int _v )
{
    // 输出图的第_v顶点的值
    return AdjList[ _v ].data;
}

int ALGraph::GetFirstNeighbor( int _v )
{
    // 第_v个顶点没有边
    if ( AdjList[ _v ].firstarc == NULL )
        return -1;
    // 返回图第_v个顶点的边链表头结点的弧头
    else
        return ( AdjList[ _v ].firstarc )->adjvex;
}

int ALGraph::GetNextNeighbor( int _v, int _w )
{
    // 返回第_v顶点的边链表弧头为_w结点的下一个结点的弧头
    // 维护第_v顶点的边链表指针
    ArcNode* P = AdjList[ _v ].firstarc;
    // 指针循环至返回或指针为NULL
    while ( P )
    {
        // 判断指针所指结点的弧头是否为_w
        if ( P->adjvex == _w )
        {
            // 返回指针下一个结点的弧头
            if ( P->nextarc )
                return P->nextarc->adjvex;
            else
                return -1;
        }
        // 移动指针
        P = P->nextarc;
    }
    // 没有找到弧头为_w, 返回-1
    return -1;
}

int* ALGraph::Graph_DTraverse( )
{
    // 深度优先遍历, 非递归代码
    int* visted = new int[ vexnum ];
    int* vex    = new int[ 2 * vexnum ];  // 存储连通分量, 不同连通分量间用-2隔开
    int  begin  = 0;                      // 存储连通分量指针
    // visted & vex初始化
    for ( int i = 0; i < vexnum; i++ )
    {
        visted[ i ] = 0;
    }
    for ( int i = 0; i < 2 * vexnum; i++ )
        vex[ i ] = -1;
    // 解决不连通分量
    for ( int i = 0; i < vexnum; i++ )
    {
        if ( visted[ i ] == 0 )
        {
            // 深度优先搜索
            DFS( i, visted, vex, begin );
            std::cout << std::endl;
            vex[ begin ] = -2;  // 隔开不同连通分量
            begin++;
        }
    }
    delete[] visted;
    return vex;
}

int* ALGraph::Graph_BTraverse( )
{
    // 广度优先搜索遍历, 非递归代码
    int* visted = new int[ vexnum ];
    int* vex    = new int[ 2 * vexnum ];  // 存储连通分量, 不同连通分量间用-2隔开
    int  begin  = 0;                      // 存储连通分量指针
    // visted & vex初始化
    for ( int i = 0; i < vexnum; i++ )
    {
        visted[ i ] = 0;
    }
    for ( int i = 0; i < 2 * vexnum; i++ )
        vex[ i ] = -1;
    // 解决不连通分量
    for ( int i = 0; i < vexnum; i++ )
    {
        if ( visted[ i ] == 0 )
        {
            // 广度优先搜索
            BFS( i, visted, vex, begin );
            std::cout << std::endl;
            vex[ begin ] = -2;  // 隔开不同连通分量
            begin++;
        }
    }
    delete[] visted;
    return vex;
}

void ALGraph::DFS( int _v, int* visted, int* vex, int& begin )
{
    // 深度优先搜索, 非递归算法, 利用栈辅助
    std::stack<int> s;
    s.push( _v );
    // 栈不空就循环
    while ( !s.empty( ) )
    {
        _v = s.top( );
        s.pop( );
        // 未访问结点, 输出并存储至vex中
        if ( visted[ _v ] == 0 )
        {
            std::cout << GetValue( _v ) << ' ';
            if ( vex != NULL )
            {
                vex[ begin ] = _v;
                begin++;
            }
            visted[ _v ] = 1;
        }
        // 访问结点边链表头结点, 遍历链表入栈
        int w = GetFirstNeighbor( _v );
        while ( w != -1 )
        {
            if ( visted[ w ] == 0 )
            {
                s.push( w );
            }
            w = GetNextNeighbor( _v, w );
        }
    }
}

void ALGraph::BFS( int _v, int* visted, int* vex, int& begin )
{
    // 广度优先搜索, 非递归算法, 利用队列辅助
    std::queue<int> q;
    q.push( _v );
    // 队列不空就循环
    while ( !q.empty( ) )
    {
        _v = q.front( );
        q.pop( );
        // 未访问结点, 输出并存储至vex中
        if ( visted[ _v ] == 0 )
        {
            std::cout << GetValue( _v ) << ' ';
            if ( vex != NULL )
            {
                vex[ begin ] = _v;
                begin++;
            }
            visted[ _v ] = 1;
        }
        // 访问结点边链表头结点, 遍历链表入栈
        int w = GetFirstNeighbor( _v );
        while ( w != -1 )
        {
            if ( visted[ w ] == 0 )
            {
                q.push( w );
            }
            w = GetNextNeighbor( _v, w );
        }
    }
}

bool ALGraph::isConnected( )
{
    int* visted = new int[ vexnum ];
    int  _      = 0;
    for ( int i = 0; i < vexnum; i++ )
    {
        visted[ i ] = 0;
    }
    for ( int i = 0; i < vexnum; i++ )
    {
        if ( visted[ i ] == 0 )
        {
            DFS( i, visted, NULL, _ );
        }
        for ( int i = 0; i < vexnum; i++ )
        {
            if ( visted[ i ] == 0 )
                return false;
            else
                visted[ i ] = 0;
        }
    }
    delete[] visted;
    return true;
}

void ALGraph::PrintConnected( int* vex )
{
    for ( int i = 0; i < 2 * vexnum; i++ )
    {
        if ( vex[ i ] > -1 )
            std::cout << GetValue( vex[ i ] ) << ' ';
        else if ( vex[ i ] == -2 )
            std::cout << std::endl;
    }
}

void test( )
{
    // 测试案例输入
    int   vexnum = 15;
    int   arcnum = 32;
    char* data   = new char[ vexnum ];
    for ( int i = 0; i < vexnum; i++ )
        data[ i ] = i + 65;
    EdgeNode* edge = new EdgeNode[ arcnum ];
    edge[ 0 ].tail = 0;
    edge[ 0 ].dest = 1;  // A->B
    edge[ 1 ].tail = 0;
    edge[ 1 ].dest = 2;  // A->C
    edge[ 2 ].tail = 0;
    edge[ 2 ].dest = 3;  // A->D
    edge[ 3 ].tail = 0;
    edge[ 3 ].dest = 4;  // A->E
    edge[ 4 ].tail = 1;
    edge[ 4 ].dest = 5;  // B->F
    edge[ 5 ].tail = 4;
    edge[ 5 ].dest = 5;  // E->F
    edge[ 6 ].tail = 4;
    edge[ 6 ].dest = 6;  // E->G
    edge[ 7 ].tail = 5;
    edge[ 7 ].dest = 6;  // F->G

    edge[ 8 ].tail  = 7;
    edge[ 8 ].dest  = 8;  // H->I
    edge[ 9 ].tail  = 8;
    edge[ 9 ].dest  = 9;  // I->J
    edge[ 10 ].tail = 7;
    edge[ 10 ].dest = 9;  // H->J

    edge[ 11 ].tail = 10;
    edge[ 11 ].dest = 11;  // K->L
    edge[ 12 ].tail = 10;
    edge[ 12 ].dest = 12;  // K->M
    edge[ 13 ].tail = 10;
    edge[ 13 ].dest = 13;  // K->N
    edge[ 14 ].tail = 11;
    edge[ 14 ].dest = 14;  // L->O
    edge[ 15 ].tail = 12;
    edge[ 15 ].dest = 14;  // M->O
    // 无向图弧头尾交换
    int half = arcnum / 2;
    for ( int i = 0; i < half; i++ )
    {
        edge[ half + i ].tail = edge[ i ].dest;
        edge[ half + i ].dest = edge[ i ].tail;
    }

    // 测试部分
    ALGraph* G   = new ALGraph( vexnum, arcnum, data, edge );
    int*     ans = NULL;
    std::cout << std::endl << "邻接表" << std::endl;
    G->PrintALGraph( );
    std::cout << std::endl;
    std::cout << "广度优先搜索" << std::endl;
    ans = G->Graph_BTraverse( );
    std::cout << "连通分量" << std::endl;
    G->PrintConnected( ans );
    std::cout << std::endl << "深度优先搜索" << std::endl;
    ans = G->Graph_DTraverse( );
    std::cout << "连通分量" << std::endl;
    G->PrintConnected( ans );
    return;
}

int main( )
{
    test( );
    return 0;
}
