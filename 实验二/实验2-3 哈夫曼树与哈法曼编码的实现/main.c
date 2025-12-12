#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef struct {
    unsigned int weight;
    unsigned int parent, lchild, rchild;
} HTNode, *HTree;

typedef char** HTCode;

/**
 * @brief 初始化哈夫曼树
 *
 * @param HT 传入的节点指针
 * @param w 权重数组
 * @param n 叶子节点个数
 */
void InitHTree(HTree* HT, int* w, int n) {
    *HT = (HTree)malloc(sizeof(HTNode) * (2 * n - 1));
    if (!*HT)
        exit(OVERFLOW);

    // 初始化叶子节点的权重和其他属性
    for (int i = 0; i < n; i++) {
        (*HT)[i].weight = w[i];
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }

    // 初始化根节点的权重和其他属性
    for (int i = n; i < 2 * n - 1; i++) {
        (*HT)[i].weight = 0;
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }
}

/**
 * @brief 选择最小的两个节点
 *
 * @param HT 哈夫曼树
 * @param k 当前节点数
 * @param s1 节点1
 * @param s2 节点2
 */
void Select(HTree HT, int k, int* s1, int* s2) {
    unsigned int min1 = UINT_MAX, min2 = UINT_MAX;
    *s1 = -1;
    *s2 = -1;

    // 查找最小的
    for (int i = 0; i < k; i++) {
        if (HT[i].parent == -1) {  // 父节点为 -1 (UINT_MAX)
            if (HT[i].weight < min1) {
                min1 = HT[i].weight;
                *s1 = i;
            }
        }
    }

    // 查找第二小的
    for (int i = 0; i < k; i++) {
        if (HT[i].parent == -1 && i != *s1) {
            if (HT[i].weight < min2) {
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }

    // 确保 s1 < s2 以保持一致的顺序（可选，但便于调试）
    if (*s1 > *s2) {
        int temp = *s1;
        *s1 = *s2;
        *s2 = temp;
    }
}

/**
 * @brief 创建哈夫曼树
 *
 * @param HT 哈夫曼树
 * @param n 叶子节点个数
 */
void CreateHTree(HTree* HT, int n) {
    int s1, s2;
    for (int i = n; i < 2 * n - 1; i++) {
        Select(*HT, i, &s1, &s2);
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
}

/**
 * @brief 哈夫曼编码
 *
 * @param HT 哈夫曼树
 * @param HC 哈夫曼编码数组
 * @param n 叶子节点个数
 */
void HTCoding(HTree HT, HTCode* HC, int n) {
    *HC = (HTCode)malloc(sizeof(char*) * n);
    char* cd = (char*)malloc(sizeof(char) * n);
    cd[n - 1] = '\0';

    for (int i = 0; i < n; i++) {
        int start = n - 1;
        int c = i;
        unsigned int f = HT[i].parent;
        while (f != -1) {  // -1 即 UINT_MAX
            if (HT[f].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';
            c = f;
            f = HT[f].parent;
        }
        (*HC)[i] = (char*)malloc(sizeof(char) * (n - start));
        strcpy((*HC)[i], &cd[start]);
    }
    free(cd);
}

/**
 * @brief 打印哈夫曼编码
 *
 * @param HT 节点
 * @param n 叶子节点个数
 */
void PrintHT(HTree HT, int n) {
    printf("Index\tWeight\tParent\tLChild\tRChild\n");
    for (int i = 0; i < 2 * n - 1; i++) {
        unsigned int p = HT[i].parent;
        unsigned int l = HT[i].lchild;
        unsigned int r = HT[i].rchild;
        printf("%d\t%u\t%d\t%d\t%d\n", i, HT[i].weight,
               (p == -1) ? -1 : (int)p,
               (l == -1) ? -1 : (int)l,
               (r == -1) ? -1 : (int)r);
    }
}

int main() {
    HTree HT;
    HTCode HC;

    // 权重数组
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11};
    // 叶子节点数量
    int n = 8;

    printf("初始化哈夫曼树\n");
    InitHTree(&HT, w, n);
    PrintHT(HT, n);

    printf("创建哈夫曼树\n");
    CreateHTree(&HT, n);
    PrintHT(HT, n);

    printf("哈夫曼编码\n");
    HTCoding(HT, &HC, n);
    for (int i = 0; i < n; i++) {
        printf("Leaf %d (weight %d): %s\n", i, w[i], HC[i]);
    }

    return 0;
}