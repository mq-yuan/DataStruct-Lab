#include <iostream>
using namespace std;

typedef char             elemtype;
typedef struct treenode* tree;
typedef char*            prtchar;

enum tag
{
    link,
    thread
};  // 枚举tag的可选值

struct treenode
{
    elemtype  data;
    treenode* lchild;
    treenode* rchild;
    tag       ltag = link;
    tag       rtag = link;
    int       a    = 0;

    treenode( )
    {
        data   = '@';
        lchild = NULL;
        rchild = NULL;
        ltag   = link;
        rtag   = link;
        a      = 0;
    }

    treenode( elemtype n )  // 初始化树节点
    {
        data = n;
    }

    // ~treenode()//销毁树节点
    // {
    //     if(lchild != NULL) delete lchild;
    //     if(rchild != NULL) delete rchild;
    // }
    void printnode( )  // 打印节点数据
    {
        cout << data << " ";
    }
};

treenode* pre = NULL;

void inimytree( tree& t )  // 初始化树
{
    t         = new treenode( );
    t->lchild = NULL;
    t->rchild = NULL;
}

void desmytree( tree& t )  // 销毁树
{
    if ( t )
    {
        if ( t->ltag == link && t->lchild )
        {
            delete t->lchild;
            t->lchild = NULL;
        }
        if ( t->rtag == link && t->rchild )
        {
            delete t->rchild;
            t->rchild = NULL;
        }
        delete t;
    }
}

void creattree( tree& t )  // 以先序遍历的顺序建立二叉树
{
    char ch;
    cin >> ch;
    if ( ch == '@' ) { t = NULL; }
    else
    {
        t       = new treenode( );
        t->data = ch;
        creattree( t->lchild );
        creattree( t->rchild );
    }
}

void preorder( tree t )  // 前序遍历二叉树，仅支持普通二叉树
{
    if ( t != NULL )
    {
        t->printnode( );
        preorder( t->lchild );
        preorder( t->rchild );
    }
}

void postorder( tree t )  // 后序遍历二叉树，仅支持普通二叉树
{
    if ( t != NULL )
    {
        postorder( t->lchild );
        postorder( t->rchild );
        t->printnode( );
    }
}

bool isleaf( tree t )  // 判断是否为叶节点
{
    return ( t->lchild == NULL ) && ( t->rchild == NULL );
}

int countleaf( tree t )  // 计算叶子数
{
    int lleaf = 0;
    int rleaf = 0;
    if ( t->lchild )
    {
        if ( isleaf( t->lchild ) ) { lleaf = 1; }
        else { lleaf = countleaf( t->lchild ); }
    }
    if ( t->rchild )
    {
        if ( isleaf( t->rchild ) ) { rleaf = 1; }
        else { rleaf = countleaf( t->rchild ); }
    }
    return lleaf + rleaf;
}

int countheight( tree t )  // 判断树的高度
{
    int lheight = 0;
    int rheight = 0;
    if ( t->lchild )
    {
        if ( isleaf( t->lchild ) ) { lheight = 1; }
        else { lheight = countheight( t->lchild ); }
    }
    if ( t->rchild )
    {
        if ( isleaf( t->rchild ) ) { rheight = 1; }
        else { rheight = countheight( t->rchild ); }
    }
    return ( lheight > rheight ) ? lheight + 1 : rheight + 1;
}

void inorderthreading( tree t )  // 为二叉树生成中序线索二叉树
{
    if ( t == NULL ) { return; }
    inorderthreading( t->lchild );
    if ( t->lchild == NULL )
    {
        t->ltag   = thread;
        t->a      = 1;
        t->lchild = pre;
    }
    if ( pre != NULL && pre->rchild == NULL )
    {
        pre->rtag   = thread;
        t->a        = 1;
        pre->rchild = t;
    }
    pre = t;
    inorderthreading( t->rchild );
}

bool isthread( tree t )  // 判断是否为线索二叉树
{
    return ( t->a == 1 );
}

void inorder( tree t )  // 中序遍历二叉树，支持普通二叉树与线索二叉树
{
    if ( t != NULL && isthread( t ) )
    {
        tree p = t;
        while ( p != NULL )
        {
            while ( p->ltag == link ) { p = p->lchild; }
            p->printnode( );
            while ( p->rtag == thread )
            {
                p = p->rchild;
                p->printnode( );
            }
            p = p->rchild;
        }
    }
    else if ( t != NULL )
    {
        inorder( t->lchild );
        t->printnode( );
        inorder( t->rchild );
    }
}

treenode prenode( tree t )  // 寻找某节点的前驱节点
{
}

treenode postnode( tree t )  // 寻找某节点的后继节点
{
}

treenode locatednode( tree t, char c )  // 定位二叉树中的节点
{
    treenode ans;
    if ( t != NULL )
    {
        if ( t->data == c ) { ans = *t; }
        if ( ans.data != c && t->ltag == link && t->lchild )
        {
            ans = locatednode( t->lchild, c );
        }
        if ( ans.data != c && t->rtag == link && t->rchild )
        {
            ans = locatednode( t->rchild, c );
        }
    }
    return ans;
}

int main( )
{
    tree t;
    inimytree( t );
    creattree( t );
    preorder( t );
    cout << endl;
    inorder( t );
    cout << endl;
    postorder( t );
    cout << endl;
    cout << countleaf( t );
    cout << endl;
    cout << countheight( t );
    cout << endl;
    inorderthreading( t );
    cout << isthread( t );
    cout << endl;
    inorder( t );
    treenode n = locatednode( t, 'G' );
    return 0;
}
