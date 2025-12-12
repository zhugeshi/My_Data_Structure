#include <stdio.h>
#include <stdlib.h>

#define BOOL       char
#define TRUE       1
#define FALSE      0
#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

#define STACK_INIT_SIZE 10
#define STACKINCREMENT  5

/******************
这里的是栈的元素定义
*******************/

typedef int SElemType;  // 声明ElemType的类型
typedef int Status;     // 声明函数类型名
typedef struct {
    SElemType* base;
    SElemType* top;
    int stacksize;
} SqStack;

/*********************
这里的是队列的元素定义
*********************/

#define MAXQSIZE 10  // 队列的最大长度
typedef int QElemType;

typedef struct {
    QElemType* base;
    int front;  // 头指针
    int rear;   // 尾指针
} SqQueue;

/********************
 这里是队列的操作函数
 *******************/

// 利用malloc函数申请存储空间，构造一个空的顺序栈S，S的初始大小为STACK_INIT_SIZE
Status InitStack(SqStack* stk) {
    stk->base = (SElemType*)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!stk->base)
        return ERROR;

    stk->top = stk->base;
    stk->stacksize = STACK_INIT_SIZE;
    return OK;
}

// 将顺序栈数据清空。只需做S.top=S.base赋值操作即可
Status ClearStack(SqStack* stk) {
    if (stk == NULL)
        return ERROR;
    stk->top = stk->base;
    return OK;
}

// 返回L中数据元素个数。直接返回S.top-S.base的值即可
int StackLength(SqStack stk) {
    return stk.top - stk.base;
}

// 用e返回栈顶元素的值
Status GetTop(SqStack stk, SElemType* e) {
    if (stk.top == stk.base)
        return ERROR;  // 栈空
    *e = *(stk.top - 1);
    return OK;
}

// 将数据e进栈
Status Push(SqStack* stk, SElemType e) {
    if (stk->top - stk->base >= stk->stacksize) {
        stk->base = (SElemType*)realloc(stk->base, (stk->stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!stk->base)
            return ERROR;

        stk->top = stk->base + stk->stacksize;  // 重新定位top指针
        stk->stacksize += STACKINCREMENT;
    }  // 栈满，追加存储空间

    *(stk->top) = e;
    stk->top++;
    return OK;
}

// 将栈顶元素出栈，并用e返回其值
Status Pop(SqStack* stk, SElemType* e) {
    if (stk->top == stk->base)
        return ERROR;  // 栈空
    stk->top--;
    *e = *(stk->top);
    return OK;
}

/************************
 这里是循环队列的操作函数
 ***********************/

// 利用malloc函数申请存储空间，构造一个空的循环队列Q，Q的大小为MAXQSIZE
Status InitQueue(SqQueue* q) {
    q->base = (QElemType*)malloc(MAXQSIZE * sizeof(QElemType));
    if (!q->base)
        return ERROR;

    q->front = 0;
    q->rear = 0;
    return OK;
}

// 将循环队列数据清空
Status ClearQueue(SqQueue* q) {
    if (q == NULL)
        return ERROR;
    q->front = 0;
    q->rear = 0;
    return OK;
}

// 返回Q中数据元素个数
Status QueueLength(SqQueue q) {
    /* 注意考虑回绕的时候 */
    return (q.rear - q.front + MAXQSIZE) % MAXQSIZE;  // 注意取模运算
}

// 数据e入队
Status EnQueue(SqQueue* q, QElemType e) {
    /* 循环队列的rear指针不指向元素 */
    if ((q->rear + 1) % MAXQSIZE == q->front)
        return ERROR;  // 队列满

    q->base[q->rear] = e;
    q->rear = (q->rear + 1) % MAXQSIZE;
    return OK;
}

// 输入出队,用e返回队头元素的值
Status DeQueue(SqQueue* q, QElemType* e) {
    if (q->front == q->rear)
        return ERROR;  // 队列空

    *e = q->base[q->front];
    q->front = (q->front + 1) % MAXQSIZE;
    return OK;
}

// 判断是否为空队列
Status QueueEmpty(SqQueue q) {
    return q.front == q.rear ? TRUE : FALSE;
}

// 判断对列为满
Status QueueFull(SqQueue q) {
    return (q.rear + 1) % MAXQSIZE == q.front ? TRUE : FALSE;
}

/*******
应用案例
********/

// 利用顺序栈的基本操作函数，编制一个将十进制整数m转换成n进制(n=2或8)整数的数制转换函数Conversion(m,n) ，并在函数中输出转换结果
void Conversion(int m, int n) {
    if (n != 2 && n != 8) {
        printf("n must be 2 or 8.\n");
        return;
    }

    SqStack stk;
    if (InitStack(&stk) != OK) {
        printf("Init stack failed.\n");
        return;
    }

    int num = m;
    while (num != 0) {
        Push(&stk, num % n);
        num /= n;
    }

    printf("The number %d in base %d is: ", m, n);
    SElemType e;
    while (StackLength(stk) > 0) {
        Pop(&stk, &e);
        printf("%d", e);
    }
    printf("\n");

    ClearStack(&stk);
    free(stk.base);
}

// 编一个函数QueueReverse(Q)，利用栈将将队列中的元素逆序重排，即队头元素变成队尾元素
Status QueueReverse(SqQueue* q) {
    if (q == NULL)
        return ERROR;

    SqStack stk;
    if (InitStack(&stk) != OK)
        return ERROR;

    while (!QueueEmpty(*q)) {
        QElemType e;
        DeQueue(q, &e);
        Push(&stk, e);
    }

    while (StackLength(stk) > 0) {
        SElemType e;
        Pop(&stk, &e);
        EnQueue(q, e);
    }

    ClearStack(&stk);
    free(stk.base);
    return OK;
}

#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
void PrintIndex(SqQueue q) {
    printf("Front is: %d. Rear is: %d\n", q.front, q.rear);
}

void PrintIndexValue(SqQueue q) {
    printf("Base[Front] is: %d. Base[Rear] is: %d\n", q.base[q.front], q.base[(q.rear - 1 + MAXQSIZE) % MAXQSIZE]);
}

void PrintElem(SqQueue q) {
    int qlen = QueueLength(q);
    for (int i = 0; i < qlen; ++i) {
        QElemType e;
        DeQueue(&q, &e);
        printf("%d ", e);
    }
    puts("");
}

int main(void) {
    printf("[1] 数值转换\n");
    int m = 2233;
    Conversion(m, 2);
    Conversion(m, 8);

    printf("[2] 循环队列操作\n");
    QElemType vals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    SqQueue q;
    if (InitQueue(&q) != OK) {
        fprintf(stderr, "Init queue failed.\n");
        return 1;
    }

    printf("<1> 连续入队9个整数后,显示队列Q的front和rear的值\n");
    for (int i = 0; i < ARR_LEN(vals); ++i) {
        EnQueue(&q, vals[i]);
    }
    PrintIndex(q);
    PrintElem(q);

    printf("<2> 若再入队1个整数,显示此时队列的状态信息\n");
    if (EnQueue(&q, 10) == OK) {
        PrintIndex(q);
    } else {
        printf("队列已满，无法再入队\n");
    }

    printf("<3> 连续出队5个整数,再入队2个整数,显示此时的队列Q的长度与front和rear的值\n");
    for (int i = 0; i < 5; ++i) {
        QElemType e;
        DeQueue(&q, &e);
    }
    for (int i = 0; i < 2; ++i) {
        EnQueue(&q, 100 + i);
    }
    printf("队列长度为: %d\n", QueueLength(q));
    PrintIndex(q);
    PrintElem(q);

    printf("调用函数QueueReverse(Q),将队列元素逆序重排,然后将队列元素出队并输出\n");
    QueueReverse(&q);
    PrintElem(q);

    return 0;
}