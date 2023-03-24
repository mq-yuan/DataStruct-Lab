#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define OVERFLOW 0
#define ERROR -2147483648

typedef enum{NUM, OPS} ElemTag;
typedef struct SNode* PtrToSNode;
typedef int Position;
typedef PtrToSNode Stack;


union data
{
    float num;
    char ops;
};


struct SNode
{
    ElemTag tag;
    union data x;
    PtrToSNode next;
};


Stack CreateStack(ElemTag Tag);
Stack InitStack(Stack S, ElemTag Tag);
void DestroyStack(Stack *S);
Stack PushStack(Stack S, ElemTag Tag);
data PopStack(Stack *S);
data ViewTopStack(Stack S);
data GetTopStack(Stack S);
void ViewStack(Stack S);
float EvaluateExpression();
Stack PushStackNoIO(Stack S, data X, ElemTag Tag);
float Str2Float(char* SNUM, int First, int End);
int QuickPow(int a, int b, int p);
char CmpOPS(data Top, data C);
data Operate(data x1, data x2, data ops);
void In2Post();


Stack CreateStack(ElemTag Tag)
{
    Stack S = NULL;
    S = InitStack(S, Tag);
    return S;
}


Stack InitStack(Stack S, ElemTag Tag)
{
    int num;
    do
    {
        printf("输入元素个数");
        scanf("%d", &num);
    }while(num < 0 && printf("请输入大于等于0的数字!\n"));
    for(; num--; S = PushStack(S, Tag));
    return S;
}


void DestroyStack(Stack *S)
{
    if(!(*S)) return;
    for(; *S; PopStack(S));
}


Stack PushStack(Stack S, ElemTag Tag)
{
    data X;
    if(Tag == NUM)
    {
        printf("请输入一个数字");
        scanf("%f", &(X.num));
    }
    else
    {
        printf("请输入一个操作符");
        scanf("\n%c", &(X.ops));
    }
    PtrToSNode P = (PtrToSNode) malloc (sizeof(SNode));
    if(!P) exit(OVERFLOW);
    P->tag = Tag; P->x = X; P->next = NULL;
    P->next = S;
    S = P;
    return S;
}


data PopStack(Stack *S)
{
    data x;
    if(!(*S))
    {
        x.num = ERROR;
        printf("执行了空栈弹出操作!\n");
        return x;
    }
    PtrToSNode P = *S;
    *S = P->next;
    x = P->x;
    free(P);
    return x;
}


data ViewTopStack(Stack S)
{
    if(!S)
    {
        data x;
        x.num = ERROR;
        printf("该栈为空 ");
        return x;
    }
    if(S->tag == NUM) printf("%.5f ", S->x.num);
    else printf("%c ", S->x.ops);
    return S->x;
}


data GetTopStack(Stack S)
{
    if(!S)
    {
        data x;
        x.num = ERROR;
        printf("该栈为空 ");
        return x;
    }
    return S->x;
}


void ViewStack(Stack S)
{
    if(!S)
    {
        printf("该栈为空\n");
        return ;
    }
    PtrToSNode P = S;
    for(; P; P = P->next) ViewTopStack(P);
    printf("\n");
    return ;
}


Stack PushStackNoIO(Stack S, data X, ElemTag Tag)
{
    PtrToSNode P = (PtrToSNode) malloc (sizeof(SNode));
    if(!P) exit(OVERFLOW);
    P->tag = Tag; P->x = X; P->next = NULL;
    P->next = S;
    S = P;
    return S;
}


float Str2Float(char* SNUM, int First, int End)
{
    float ans = 0, tmp=0.1;
    int j = 0;
    bool flag = true;
    for(int i = First; i < End; i++)
    {
        if(SNUM[i] == '.')
        {
            flag = false;
            continue;
        }
        if(flag) ans = ans * 10 + (SNUM[i] - '0');
        else
        {
            for(int k = 0; k < j; k++) tmp = tmp * 0.1;
            ans = ans + (SNUM[i] - '0') * tmp;
            j++;
        }
    }
    return ans;
}


int QuickPow(int a, int b, int p)
{
	int ans = 1;
	while(a && b)
	{
		if (b & 1) ans = ((ans*a) % p) % p;
		a = (a * a) % p;
		b >>= 1;
	}
	return ans;
}


float EvaluateExpression()
{
    Stack NUM_S, OPS_S;
    bool flag = false;
    data X, ans;
    NUM_S = NULL;
    OPS_S = NULL;
    printf("请输入正确的中缀数学表达式, 以#开头和结尾, 允许使用 ( ) + - * / 这六种符号(支持小数), 不支持负数\n");
    char Expression[100] = {};
    char *p = Expression;
    int first=1, end=1;
    scanf("\n%[^\n]", Expression);
    X.ops = *p++;
    if(X.ops != '#')
    {
        printf("未按要求在开头和结尾处输入\"#\"");
        exit(OVERFLOW);
    }
    OPS_S = PushStackNoIO(OPS_S, X, OPS);
    while(*p != '\0')
    {
        if((*p <= '9' && *p >= '0') || *p == '.')
        {
            end++;
            flag = true;
        }
        else
        {
            if(flag == true)
            {
                X.num = Str2Float(Expression, first, end);
                NUM_S = PushStackNoIO(NUM_S, X, NUM);
                flag = false;
            }
            first = ++end;
            if(*p == ' ')
            {
                p++;
                continue;
            }
            X.ops = *p;
            switch (CmpOPS(GetTopStack(OPS_S), X))
            {
                case '<':
                    OPS_S = PushStackNoIO(OPS_S, X, OPS);
                    break;
                case '>':
                    data tmp, x1, x2;
                    tmp = PopStack(&OPS_S);
                    x1 = PopStack(&NUM_S);
                    x2 = PopStack(&NUM_S);
                    NUM_S = PushStackNoIO(NUM_S, Operate(x2, x1, tmp), NUM);
                    p--;
                    first = --end;
                    break;
                case '=':
                    ans = PopStack(&NUM_S);
                    DestroyStack(&NUM_S);
                    DestroyStack(&OPS_S);
                    printf("%.5f", ans.num);
                    return ans.num;
                case '.':
                    PopStack(&OPS_S);
                    break;
            }
        }
        p++;
    }
    return 0.0;
}


char CmpOPS(data Top, data C)
{
    assert (Top.ops == '+' || '-' || '*' || '/' || '(' || ')' || '#');
    assert (C.ops == '+' || '-' || '*' || '/' || '(' || ')' || '#');
    if(Top.ops == '+' || Top.ops == '-')
    {
        if (C.ops == '*' || C.ops == '/' || C.ops == '(') return '<';
        else return '>';
    }
    else if(Top.ops == '*' || Top.ops == '/')
    {
        if(C.ops == '(') return '<';
        else return '>';
    }
    else if(Top.ops == '(')
    {
        if (C.ops == ')') return '.';
        else if(C.ops != '#') return '<';
        printf("出现多余\"(\"");
        exit(OVERFLOW);
    }
    else if(Top.ops == ')')
    {
        if(C.ops != '(') return '>';
        printf("出现\")(\"现象");
        exit(OVERFLOW);
    }
    else if(Top.ops == '#')
    {
        if(C.ops == '#') return '=';
        else if(C.ops != ')') return '<';
        printf("出现多余\")\"");
        exit(OVERFLOW);
    }
    else
    {
        printf("输入无效操作符%c", Top.ops);
        exit(OVERFLOW);
    }
}


data Operate(data x1, data x2, data ops)
{
    assert (ops.ops == '+' || '-' || '*' || '/' || '(' || ')' || '#');
    data ans;
    switch(ops.ops)
    {
        case '+':
            ans.num = x1.num + x2.num;
            break;
        case '-':
            ans.num = x1.num - x2.num;
            break;
        case '*':
            ans.num = x1.num * x2.num;
            break;
        case '/':
            if(x2.num == 0.0)
            {
                printf("执行了除0运算");
                exit(OVERFLOW);
            }
            ans.num = x1.num / x2.num;
            break;
    }
    return ans;
}


void In2Post()
{
    Stack NUM_S, OPS_S;
    bool flag = false;
    bool flag1 = true;
    data X;
    NUM_S = NULL;
    OPS_S = NULL;
    printf("请输入正确的中缀数学表达式, 以#开头和结尾, 允许使用 ( ) + - * / 这六种符号\n");
    char Expression[100] = {};
    char *p = Expression;
    int first=1, end=1;
    scanf("\n%[^\n]", Expression);
    X.ops = *p++;
    if(X.ops != '#')
    {
        printf("未按要求在开头和结尾处输入\"#\"");
        exit(OVERFLOW);
    }
    OPS_S = PushStackNoIO(OPS_S, X, OPS);
    while(*p != '\0')
    {
        if((*p <= '9' && *p >= '0') || *p == '.')
        {
            end++;
            flag = true;
        }
        else
        {
            if(flag == true)
            {
                X.num = Str2Float(Expression, first, end);
                NUM_S = PushStackNoIO(NUM_S, X, NUM);
                flag = false;
            }
            first = ++end;
            if(*p == ' ')
            {
                p++;
                continue;
            }
            X.ops = *p;
            switch (CmpOPS(GetTopStack(OPS_S), X))
            {
                case '<':
                    OPS_S = PushStackNoIO(OPS_S, X, OPS);
                    break;
                case '>':
                    data tmp, x1, x2;
                    tmp = PopStack(&OPS_S);
                    x1 = PopStack(&NUM_S);
                    if(flag1)
                    {
                        x2 = PopStack(&NUM_S);
                        printf("%.5f %.5f %c ", x2.num, x1.num, tmp.ops);
                        flag1 = false;
                    }
                    else printf("%.5f %c ", x1.num, tmp.ops);
                    // NUM_S = PushStackNoIO(NUM_S, Operate(x2, x1, tmp), NUM);
                    p--;
                    first = --end;
                    break;
                case '=':
                    // ans = PopStack(&NUM_S);
                    DestroyStack(&NUM_S);
                    DestroyStack(&OPS_S);
                    // printf("%.5f", ans.num);
                    return ;
                case '.':
                    PopStack(&OPS_S);
                    break;
            }
        }
        p++;
    }
}


void test_1()
{
    printf("对Stack定义、操作进行测试\n\n");
    Stack INTS = CreateStack(NUM);
    Stack CHARS = CreateStack(OPS);
    data x;
    x = PopStack(&INTS);
    printf("%.5f\n", x.num);
    ViewStack(INTS);
    ViewStack(CHARS);
    DestroyStack(INTS);
    DestroyStack(CHARS);
}


void test_2()
{
    printf("对表达式求值进行测试\n\n");
    EvaluateExpression();
}


void test_3()
{
    printf("中缀表达式转后缀表达式测试\n\n");
    In2Post();
}


int main()
{
    test_1();
    test_2();
    test_3();
    return 0;
}
