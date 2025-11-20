#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef int Status;              // 声明函数类型名
typedef char TElemType;          // 声明结点元素值的类型

// 普通二叉树结构定义
typedef struct BiTNode {
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/*****************************************************************
 * @brief 基本操作函数实现
 *****************************************************************/

// （1）根据先序遍历字符序列创建二叉树（#表示空）
// ABC##DE##F##G##
Status CreateBiTree(BiTree *T) {
    char ch;
    scanf("%c", &ch);
    // '#'或者' '表示空结点
    if (ch == '#' || ch == ' ') {
        *T = NULL;
    } else {
        *T = (BiTree)malloc(sizeof(BiTNode));
        if (!*T) exit(ERROR);
        (*T)->data = ch;
        CreateBiTree(&(*T)->lchild);  // 递归创建左子树
        CreateBiTree(&(*T)->rchild);  // 递归创建右子树
    }
    return OK;
}

// （2）递归先序遍历
void PreOrderTraverse(BiTree T) {
    if (T) {
        printf("%c ", T->data);        // 访问根
        PreOrderTraverse(T->lchild);    // 遍历左子树
        PreOrderTraverse(T->rchild);    // 遍历右子树
    }
}

// （3）递归中序遍历
void InOrderTraverse(BiTree T) {
    if (T) {
        InOrderTraverse(T->lchild);     // 遍历左子树
        printf("%c ", T->data);        // 访问根
        InOrderTraverse(T->rchild);     // 遍历右子树
    }
}

// （4）递归后序遍历
void PostOrderTraverse(BiTree T) {
    if (T) {
        PostOrderTraverse(T->lchild);   // 遍历左子树
        PostOrderTraverse(T->rchild);   // 遍历右子树
        printf("%c ", T->data);        // 访问根
    }
}

// 辅助函数：打印栈内容
void PrintStack(BiTree stack[], int top) {
    printf("[栈: ");
    for (int i = 0; i <= top; i++) {
        printf("%c", stack[i]->data);
        if (i < top) printf(", ");
    }
    printf("] ");
}

// （5）非递归先序遍历（输出进栈/出栈过程）
void NRPreOrderTraverse(BiTree T) {
    if (!T) return;
    BiTree stack[MAXSIZE];
    int top = -1;
    BiTree p_cur = T;  // 初始化为根节点
    
    /*****************************************************************
     * @brief 示例二叉树结构 
     * ! 改成注释形式 ABC##DE##F##G##
     *                     A
     *                    / \
     *                   B   G
     *                  / \
     *                 C   D
     *                    / \
     *                   E   F
     *****************************************************************/
    printf("\n非递归先序遍历过程：\n");
    while (p_cur || top >= 0) {
        while (p_cur) {
            // 访问结点
            printf("访问: %c, ", p_cur->data);
            // 入栈
            stack[++top] = p_cur;
            printf("入栈: %c, ", p_cur->data);
            PrintStack(stack, top);
            printf("\n");
            p_cur = p_cur->lchild;
        }
        if (top >= 0) {
            p_cur = stack[top--];
            printf("出栈: %c, ", p_cur->data);
            if (top >= 0) PrintStack(stack, top);
            else printf("[栈: 空] ");
            printf("\n");
            p_cur = p_cur->rchild;
        }
    }
}

// （6）非递归中序遍历（输出进栈/出栈过程）
void NRInOrderTraverse(BiTree T) {
    if (!T) return;
    BiTree stack[MAXSIZE];
    int top = -1;
    BiTree p = T;
    
    printf("\n非递归中序遍历过程：\n");
    while (p || top >= 0) {
        while (p) {
            // 入栈
            stack[++top] = p;
            printf("入栈: %c, ", p->data);
            PrintStack(stack, top);
            printf("\n");
            p = p->lchild;
        }
        if (top >= 0) {
            p = stack[top--];
            printf("出栈: %c, ", p->data);
            printf("访问: %c, ", p->data);
            if (top >= 0) PrintStack(stack, top);
            else printf("[栈: 空] ");
            printf("\n");
            p = p->rchild;
        }
    }
}

// （7）非递归后序遍历（输出进栈/出栈/读栈顶过程）
void NRPostOrderTraverse(BiTree T) {
    if (!T) return;
    BiTree stack[MAXSIZE];
    int top = -1;
    BiTree p = T, lastVisit = NULL;
    
    printf("\n非递归后序遍历过程：\n");
    while (p || top >= 0) {
        while (p) {
            // 入栈
            stack[++top] = p;
            printf("入栈: %c, ", p->data);
            PrintStack(stack, top);
            printf("\n");
            p = p->lchild;
        }
        
        // 读栈顶元素
        p = stack[top];
        printf("读栈顶: %c, ", p->data);
        PrintStack(stack, top);
        printf("\n");
        
        // 如果右子树为空或已访问，则访问该节点
        if (p->rchild == NULL || p->rchild == lastVisit) {
            top--;
            printf("出栈: %c, 访问: %c, ", p->data, p->data);
            if (top >= 0) PrintStack(stack, top);
            else printf("[栈: 空] ");
            printf("\n");
            lastVisit = p;
            p = NULL;
        } else {
            p = p->rchild;
        }
    }
}

/*****************************************************************
 * 应用函数实现
 *****************************************************************/

// 应用实例1：返回度为d的结点数（d=0,1,2）
int TNodes(BiTree T, int d) {
    if (!T) return 0;
    
    int degree = 0;
    if (T->lchild) degree++;
    if (T->rchild) degree++;
    
    int count = (degree == d) ? 1 : 0;
    return count + TNodes(T->lchild, d) + TNodes(T->rchild, d);
}

// 应用实例2：求二叉树高度
int High(BiTree T) {
    if (!T) return 0;
    int leftH = High(T->lchild);
    int rightH = High(T->rchild);
    return (leftH > rightH ? leftH : rightH) + 1;
}

// 应用实例3：创建二叉排序树（BST）
Status InsertBST(BiTree *T, char ch) {
    if (*T == NULL) {
        *T = (BiTree)malloc(sizeof(BiTNode));
        if (!*T) return OVERFLOW;
        (*T)->data = ch;
        (*T)->lchild = (*T)->rchild = NULL;
        return OK;
    }
    if (ch < (*T)->data) {
        return InsertBST(&(*T)->lchild, ch);
    } else if (ch > (*T)->data) {
        return InsertBST(&(*T)->rchild, ch);
    }
    return OK;  // 相等则不插入
}

Status CreateBST(BiTree *T, const char *chars) {
    *T = NULL;
    if (!chars) return ERROR;
    
    for (int i = 0; chars[i] != '\0'; i++) {
        if (chars[i] != ' ' && chars[i] != '\n') {
            if (InsertBST(T, chars[i]) != OK) {
                return ERROR;
            }
        }
    }
    return OK;
}

/*****************************************************************
 * 测试主函数
 *****************************************************************/
int main(void) {
    BiTree T = NULL;
    
    printf("=== 二叉树基本操作测试 ===\n\n");
    printf("请输入先序遍历序列（#表示空结点）：\n");
    printf("示例：ABC##DE##F##G##\n");
    
    // 创建二叉树
    CreateBiTree(&T);
    
    if (!T) {
        printf("二叉树为空！\n");
        return 0;
    }
    
    // 递归遍历
    printf("\n递归先序遍历: ");
    PreOrderTraverse(T);
    
    printf("\n递归中序遍历: ");
    InOrderTraverse(T);
    
    printf("\n递归后序遍历: ");
    PostOrderTraverse(T);
    printf("\n");
    
    // 非递归遍历（带详细过程）
    NRPreOrderTraverse(T);
    NRInOrderTraverse(T);
    NRPostOrderTraverse(T);
    
    // 应用函数测试
    printf("\n=== 应用函数测试 ===\n");
    printf("度为0的结点数（叶子结点）: %d\n", TNodes(T, 0));
    printf("度为1的结点数: %d\n", TNodes(T, 1));
    printf("度为2的结点数: %d\n", TNodes(T, 2));
    printf("二叉树高度: %d\n", High(T));
    
    // 测试二叉排序树
    printf("\n=== 二叉排序树测试 ===\n");
    BiTree BST = NULL;
    const char *chars = "FDACEGB";
    printf("输入字符序列: %s\n", chars);
    CreateBST(&BST, chars);
    printf("BST中序遍历（应为升序）: ");
    InOrderTraverse(BST);
    printf("\n");
    
    return 0;
}
