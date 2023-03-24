#include <iostream>
using namespace std;

typedef char            ElementType;
typedef class TreeNode* PrtTNode;
typedef class MyTree*   PrtTree;
typedef char*           PrtChar;

typedef enum
{
    Link,
    Thread
} Tag;

class TreeNode
{
public:
    ElementType data;
    Tag         LTag;
    Tag         RTag;
    PrtTree     lchild;
    PrtTree     rchild;

    TreeNode( ElementType _data );
    TreeNode( );
    TreeNode( const TreeNode& Node );
    ~TreeNode( );
    void printNode( );
};

class MyTree
{
public:
    PrtTNode Root;

    MyTree( );
    MyTree( char* tree );
    MyTree( PrtChar* tree );
    MyTree( const MyTree& Tree );
    ~MyTree( );
    MyTree&  operator=( const MyTree& Tree );
    void     PreOrderTraverse( );
    void     InOrderTraverse( );
    void     PostOrderTraverse( );
    int      CountLeaf( );
    int      CountHeight( );
    bool     IsLeaf( );
    bool     IsThreadedTree( );
    void     InOrderThreading( );
    void     InThreading( PrtTree& Pre );
    TreeNode locateNode( char c );
    TreeNode preNode( TreeNode n );
    TreeNode nextNode( TreeNode n );
};

TreeNode::TreeNode( ElementType _data )
{
    data   = _data;
    LTag   = Link;
    RTag   = Link;
    lchild = NULL;
    rchild = NULL;
}

TreeNode::TreeNode( )
{
    data   = '@';
    LTag   = Link;
    RTag   = Link;
    lchild = NULL;
    rchild = NULL;
}

TreeNode::TreeNode( const TreeNode& Node )
{
    data   = Node.data;
    LTag   = Node.LTag;
    RTag   = Node.RTag;
    lchild = Node.lchild;
    rchild = Node.rchild;
}

TreeNode::~TreeNode( ) {}

void TreeNode::printNode( )
{
    cout << data << ' ';
}

MyTree::MyTree( )
{
    // 构造函数，无参初始化，用来初始化空树
    Root = NULL;
}

MyTree::MyTree( char* tree )
{
    // 构造函数，根据前序遍历字符串进行初始化
    PrtChar* P = &tree;    // 创建字符串指针
    if ( *( *P ) != '@' )  // 遍历访问字符串
    {
        // 构建新结点
        Root = new TreeNode( *( *P ) );
        ( *P )++;  // 字符串移动
        // 为结点左右子树的根节点开辟空间
        Root->lchild = new MyTree( P );
        Root->rchild = new MyTree( P );
        if ( ( Root->lchild )->Root == NULL ) Root->lchild = NULL;
        if ( ( Root->rchild )->Root == NULL ) Root->rchild = NULL;
    }
    else  // 空结点的pass
    {
        Root = NULL;
        if ( *( *P ) ) ( *P )++;
    }
}

MyTree::MyTree( PrtChar* tree )
{
    // 构造函数，输入可以为双引号引入的字符串，其余部分同上
    PrtChar* P = tree;
    if ( *( *P ) != '@' )
    {
        Root = new TreeNode( *( *P ) );
        ( *P )++;
        Root->lchild = new MyTree( P );
        Root->rchild = new MyTree( P );
        if ( ( Root->lchild )->Root == NULL ) Root->lchild = NULL;
        if ( ( Root->rchild )->Root == NULL ) Root->rchild = NULL;
    }
    else
    {
        Root = NULL;
        if ( *( *P ) ) ( *P )++;
    }
}

MyTree::MyTree( const MyTree& Tree )
{
    // 深度拷贝构造函数
    Root       = new TreeNode;    // 为根节点开辟空间
    *Root      = *( Tree.Root );  // 根节点的拷贝
    PrtTNode P = Root;
    // 左右孩子的深度拷贝，利用了MyTree的重载赋值运算符
    if ( ( Tree.Root )->lchild )
    {
        P->lchild      = new MyTree;
        *( P->lchild ) = *( ( Tree.Root )->lchild );
    }
    else
        P->lchild = NULL;  // 孩子为空，赋值为NULL
    if ( ( Tree.Root )->rchild )
    {
        P->rchild      = new MyTree;
        *( P->rchild ) = *( ( Tree.Root )->rchild );
    }
    else
        P->rchild = NULL;
}

MyTree& MyTree::operator=( const MyTree& Tree )
{
    // 重载赋值运算符
    Root  = new TreeNode;
    *Root = *( Tree.Root );
    // 递归调用赋值命令，完成深度拷贝
    if ( ( Tree.Root )->lchild )
    {
        Root->lchild      = new MyTree;
        *( Root->lchild ) = *( ( Tree.Root )->lchild );
    }
    if ( ( Tree.Root )->rchild )
    {
        Root->rchild      = new MyTree;
        *( Root->rchild ) = *( ( Tree.Root )->rchild );
    }
    return *this;
}

MyTree::~MyTree( )
{
    // 析构函数
    if ( Root )  // 判断节点是否为NULL
    {
        // 对左孩子进行析构
        if ( Root->LTag == Link && Root->lchild )
        {
            delete Root->lchild;
            Root->lchild = NULL;
        }
        // 对右孩子进行析构
        if ( Root->RTag == Link && Root->rchild )
        {
            delete Root->rchild;
            Root->rchild = NULL;
        }
        // 清除结点本身
        delete Root;
    }
    Root = NULL;
}

void MyTree::PreOrderTraverse( )
{
    // 前序遍历，递归实现
    if ( Root )  // 判断该子树是否为空树
    {
        Root->printNode( );
        if ( Root->LTag == Link && Root->lchild )
            ( Root->lchild )->PreOrderTraverse( );
        if ( Root->RTag == Link && Root->rchild )
            ( Root->rchild )->PreOrderTraverse( );
    }
}

void MyTree::InOrderTraverse( )
{
    // 中序遍历
    // 针对线索二叉树，循环实现
    if ( this->IsThreadedTree( ) )
    {
        PrtTree P = Root->lchild;
        while ( P != this )
        {
            while ( ( P->Root )->LTag == Link ) P = P->Root->lchild;
            P->Root->printNode( );
            while ( P->Root->RTag == Thread && P->Root->rchild != this )
            {
                P = P->Root->rchild;
                P->Root->printNode( );
            }
            P = P->Root->rchild;
        }
    }
    // 针对普通二叉树，递归实现
    else if ( Root )
    {
        if ( Root->LTag == Link && Root->lchild )
            ( Root->lchild )->InOrderTraverse( );
        Root->printNode( );
        if ( Root->RTag == Link && Root->rchild )
            ( Root->rchild )->InOrderTraverse( );
    }
}

void MyTree::PostOrderTraverse( )
{
    // 后序遍历，递归实现
    if ( Root )
    {
        if ( Root->LTag == Link && Root->lchild )
            ( Root->lchild )->PostOrderTraverse( );
        if ( Root->RTag == Link && Root->rchild )
            ( Root->rchild )->PostOrderTraverse( );
        Root->printNode( );
    }
}

int MyTree::CountLeaf( )
{
    // 计算叶子结点的个数，递归实现
    // 记录左右子树的叶子结点个数
    int LeftLeafCount  = 0;
    int RightLeafCount = 0;
    // 计算左子树叶子结点个数
    if ( Root->lchild )
    {
        if ( ( Root->lchild )->IsLeaf( ) )
            LeftLeafCount = 1;
        else { LeftLeafCount = ( Root->lchild )->CountLeaf( ); }
    }
    // 计算右子树叶子节点的个数
    if ( Root->rchild )
    {
        if ( ( Root->rchild )->IsLeaf( ) )
            RightLeafCount = 1;
        else { RightLeafCount = ( Root->rchild )->CountLeaf( ); }
    }
    return LeftLeafCount + RightLeafCount;
}

bool MyTree::IsLeaf( )
{
    // 判断是否为叶子结点
    return ( Root->lchild == NULL ) && ( Root->rchild == NULL );
}

int MyTree::CountHeight( )
{
    // 计算树的高度，递归实现
    int LeftLeafHeight  = 0;  // 记录左子树高度
    int RightLeafHeight = 0;  // 记录右子树高度
    // 计算左子树高度，递归实现
    if ( Root->lchild )
    {
        if ( ( Root->lchild )->IsLeaf( ) )
            LeftLeafHeight = 1;
        else { LeftLeafHeight = ( Root->lchild )->CountHeight( ); }
    }
    // 计算右子树高度，递归实现
    if ( Root->rchild )
    {
        if ( ( Root->rchild )->IsLeaf( ) )
            RightLeafHeight = 1;
        else { RightLeafHeight = ( Root->rchild )->CountHeight( ); }
    }
    // 返回左右子树高度最大值
    return ( LeftLeafHeight > RightLeafHeight ) ? LeftLeafHeight + 1 :
                                                  RightLeafHeight + 1;
}

void MyTree::InOrderThreading( )
{
    // 中序遍历线索化
    PrtTree Thrt;
    PrtTree Pre = NULL;  // 记录前驱
    // 为新的根节点，分配栈空间
    PrtTree Tree = new MyTree;
    // 用于拷贝原根节点
    PrtTNode TNode = new TreeNode;
    *TNode         = *( this->Root );
    Tree->Root     = TNode;
    // 创建新的根结点，并为其右孩子赋值
    Thrt                   = this;
    Thrt->Root             = new TreeNode( );
    ( Thrt->Root )->LTag   = Link;
    ( Thrt->Root )->RTag   = Thread;
    ( Thrt->Root )->rchild = Thrt;
    // 为其左孩子赋值
    if ( Root == NULL )
        ( Thrt->Root )->lchild = Thrt;
    else
    {
        ( Thrt->Root )->lchild = Tree;
        Pre                    = Thrt;
        Tree->InThreading( Pre );  // 递归抵用线索化方法
        ( Pre->Root )->rchild  = Thrt;
        ( Pre->Root )->RTag    = Thread;
        ( Thrt->Root )->rchild = Pre;
    }
}

void MyTree::InThreading( PrtTree& Pre )
{
    // 递归实现中序线索化核心代码
    if ( Root )
    {
        if ( Root->lchild )
            ( Root->lchild )->InThreading( Pre );  // 左子树线索化
        // 左子树为空，则指向前驱
        if ( Root->lchild == NULL )
        {
            Root->LTag   = Thread;
            Root->lchild = Pre;
        }
        // 前驱的右子树为空，则前驱的右子树指向该树
        if ( ( Pre->Root )->rchild == NULL )
        {
            ( Pre->Root )->RTag   = Thread;
            ( Pre->Root )->rchild = this;
        }
        Pre = this;  // 更新前驱
        if ( Root->rchild )
            ( Root->rchild )->InThreading( Pre );  // 右子树线索化
    }
}

bool MyTree::IsThreadedTree( )
{
    // 判断是否线索化，根据新建立根节点判断
    return ( Root->data == '@' );
}

TreeNode MyTree::preNode( TreeNode n )
{
    // 确定结点的前驱，只针对线索化二叉树
    if ( this->IsThreadedTree( ) )
    {
        if ( n.LTag == Thread )
            return *( n.lchild->Root );  // 若该结点左子树为线索，则返回左孩子
        else
        {
            PrtTree P   = Root->lchild;  // 记录游标节点的后继
            PrtTree Cur = this;          // 游标节点
            // 中序遍历找到前驱
            while ( P != this )
            {
                while ( ( P->Root )->LTag == Link ) { P = P->Root->lchild; }
                if ( P->Root == &n )
                    return *( Cur->Root );  // 后继为输入结点，则返回游标节点
                else
                    Cur = P;  // 更新游标节点
                // 对后继的处理，主要代码同上
                while ( P->Root->RTag == Thread && P->Root->rchild != this )
                {
                    P = P->Root->rchild;
                    if ( P->Root == &n )
                        return *( Cur->Root );
                    else
                        Cur = P;
                }
                P = P->Root->rchild;
            }
            cout << "Not EXIST Node" << endl;  // 未找到该结点
        }
    }
    else
    {
        cout << "Not ThreadedTree!" << endl;  // 非线索二叉树
    }
    return n;
}

TreeNode MyTree::nextNode( TreeNode n )
{
    // 线索化二叉树结点的后继
    if ( this->IsThreadedTree( ) )
    {
        if ( n.RTag == Thread )
            return *( n.rchild->Root );  // 若该结点右子树为线索，则返回右孩子
        else
        {
            PrtTree P   = Root->lchild;  // 记录游标节点后继
            PrtTree Cur = this;          // 游标节点
            while ( P != this )
            {
                while ( ( P->Root )->LTag == Link ) { P = P->Root->lchild; }
                if ( Cur->Root == &n )
                    return *( P->Root );  // 若游标节点为输入结点，则返回其后继
                else
                    Cur = P;  // 更新游标节点
                // 继续中序遍历，代码主要部分同上
                while ( P->Root->RTag == Thread && P->Root->rchild != this )
                {
                    P = P->Root->rchild;
                    if ( Cur->Root == &n )
                        return *( P->Root );
                    else
                        Cur = P;
                }
                P = P->Root->rchild;
            }
            if ( Cur->Root == &n )
                return *( P->Root );
            else
                cout << "Not EXIST Node" << endl;  // 未找到结点
        }
    }
    else
    {
        cout << "Not ThreadedTree!" << endl;  // 非线索二叉树
    }
    return n;
}

TreeNode MyTree::locateNode( char c )
{
    // 定位值为c的前序遍历的第一个结点
    TreeNode ans;
    if ( Root )
    {
        // 前序遍历定位
        if ( Root->data == c ) return *Root;
        if ( ans.data != c && Root->LTag == Link && Root->lchild )
            ans = ( Root->lchild )->locateNode( c );
        if ( ans.data != c && Root->RTag == Link && Root->rchild )
            ans = ( Root->rchild )->locateNode( c );
    }
    return ans;
}

int main( )
{
    MyTree myTree( ( char* )"ABC@@DE@G@@F@@@" );
    myTree.PreOrderTraverse( );
    cout << endl;
    myTree.InOrderTraverse( );
    cout << endl;
    myTree.PostOrderTraverse( );
    cout << endl;
    cout << myTree.CountLeaf( ) << endl;
    cout << myTree.CountHeight( ) << endl;
    MyTree myThreadedTree( myTree );
    cout << myThreadedTree.IsThreadedTree( ) << endl;
    myThreadedTree.InOrderThreading( );
    cout << myThreadedTree.IsThreadedTree( ) << endl;
    myThreadedTree.InOrderTraverse( );
    cout << endl;
    TreeNode n = myThreadedTree.locateNode( 'G' );
    myThreadedTree.preNode( n ).printNode( );
    cout << endl;
    myThreadedTree.nextNode( n ).printNode( );
    cout << endl;
}
