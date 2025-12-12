#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef int Status;
typedef char TElemType;

typedef struct BiThrNode {
    TElemType data;
    struct BiThrNode *lchild, *rchild;
    int LTag, RTag;
} BiThrNode, *BiThrTree;

// 辅助栈结构
BiThrTree stack[MAXSIZE];
int top = -1;

void Push(BiThrTree p) {
    if (top < MAXSIZE - 1)
        stack[++top] = p;
}

void Pop(BiThrTree* p) {
    if (top >= 0)
        *p = stack[top--];
}

int StackEmpty() {
    return top == -1;
}

/**
 * @brief 创建二叉树
 *
 * @param T 二叉树节点
 * @return Status 状态码
 */
Status InitBiThrTree(BiThrTree* T) {
    char ch;
    scanf(" %c", &ch);
    if (ch == '#' || ch == ' ') {
        *T = NULL;
    } else {
        *T = (BiThrTree)malloc(sizeof(BiThrNode));
        if (!*T)
            exit(OVERFLOW);
        (*T)->data = ch;
        InitBiThrTree(&(*T)->lchild);

        if ((*T)->lchild)
            (*T)->LTag = 0;
        else
            (*T)->LTag = 1;

        InitBiThrTree(&(*T)->rchild);
        if ((*T)->rchild)
            (*T)->RTag = 0;
        else
            (*T)->RTag = 1;
    }
    return OK;
}

/**
 * @brief 访问节点
 *
 * @param p 节点指针
 */
void Visit(BiThrNode* p) {
    char leftVal = (p->lchild) ? p->lchild->data : '^';
    char rightVal = (p->rchild) ? p->rchild->data : '^';
    printf("| %d | %c | %c | %c | %d |\n", p->LTag, leftVal, p->data, rightVal, p->RTag);
}

/**
 * @brief 中序遍历二叉树
 *
 * @param T
 */
void InOrderTraverse(BiThrTree T) {
    if (T) {
        if (T->LTag == 0)
            InOrderTraverse(T->lchild);
        Visit(T);
        if (T->RTag == 0)
            InOrderTraverse(T->rchild);
    }
}

/**
 * @brief 中序线索化二叉树
 *
 * @param Thrt 头结点指针
 * @param T
 */
void InOrderThreading(BiThrTree* Thrt, BiThrTree T) {
    // 初始化自带头节点
    *Thrt = (BiThrTree)malloc(sizeof(BiThrNode));
    (*Thrt)->LTag = 0;
    (*Thrt)->RTag = 1;
    (*Thrt)->rchild = *Thrt;
    (*Thrt)->data = '@';

    if (!T) {
        (*Thrt)->lchild = *Thrt;
    } else {
        (*Thrt)->lchild = T;
        BiThrTree pre = *Thrt;
        BiThrTree p = T;
        top = -1;  // 初始化堆栈

        // 当没有遍历到叶子节点或者栈不为空
        while (p || !StackEmpty()) {
            if (p) {
                Push(p);
                // 遍历左子树
                if (p->LTag == 0)
                    p = p->lchild;
                else
                    p = NULL;
            } else {
                Pop(&p);

                if (p->LTag == 1) {
                    p->lchild = pre;
                }
                // 线索化右子树
                if (pre->RTag == 1 && pre->rchild == NULL) {  // 线索化
                    pre->rchild = p;
                }

                pre = p;

                // 继续遍历右节点
                if (p->RTag == 0)
                    p = p->rchild;
                else
                    p = NULL;
            }
        }
        pre->rchild = *Thrt;
        pre->RTag = 1;
        (*Thrt)->rchild = pre;
    }
}

/**
 * @brief 非递归中序遍历线索二叉树
 *
 * @param T
 */
void InOrderTraverse_Thr(BiThrTree T) {
    BiThrTree p = T->lchild;
    while (p != T) {
        while (p->LTag == 0) p = p->lchild;
        Visit(p);
        while (p->RTag == 1 && p->rchild != T) {
            p = p->rchild;
            Visit(p);
        }
        p = p->rchild;
    }
}

int main() {
    BiThrTree T, Thrt;
    printf("Please input preorder traversal string (e.g. ABD##E##CF##):\n");
    InitBiThrTree(&T);

    printf("中序遍历\n");
    InOrderTraverse(T);

    // 线索化二叉树
    InOrderThreading(&Thrt, T);

    // 递归
    printf("InOrderTraverse (Recursive on Header):\n");
    InOrderTraverse(Thrt);

    // 非递归
    printf("InOrderTraverse_Thr (Non-Recursive):\n");
    InOrderTraverse_Thr(Thrt);

    return 0;
}