#include <stdio.h>
#include <stdlib.h>

#define OK             1
#define ERROR          0
#define TRUE           1
#define FALSE          0
#define INFEASIBLE     -1
#define OVERFLOW       -2
#define MAXSIZE        100
#define INFINITY       32767  // 表示最大值∞
#define MAX_VERTEX_NUM 20     // 最大顶点数

typedef int Status;      // 声明函数类型名
typedef char TElemType;  // 声明结点元素值的类型
typedef int Status;
typedef char VertexType;  // 顶点数据的类型(字符）

typedef enum {
    DG,       // 有向图
    DN,       // 有向简单网
    UDG,      // 无向图
    UDN       // 无向简单图
} GraphKind;  // 图的类型

// 邻接表节点数据类型
typedef struct ArcNode {
    int adjvex;               // 该边(弧)所依附(指向)的顶点在顶点数组的下标
    struct ArcNode* nextarc;  // 指向下一条边(弧)的指针
    int weight;               // 边(弧)的权值，无权图其值为0
} ArcNode;

// 顶点数组数据类型
typedef struct VNode {
    VertexType data;    // 顶点数据
    ArcNode* firstarc;  // 指向第一条依附该顶点的边(弧)的指针
} VNode, AdjList[MAX_VERTEX_NUM];

// 图的数据类型
typedef struct {
    AdjList vertices;    // 图的顶点数组
    int vexnum, arcnum;  // 图的顶点数和边(弧)数
    GraphKind kind;      // 图的类型
} ALGraph;

// 函数声明
Status CreateALGraph(GraphKind GKind, ALGraph* G, int vexnum, int arcnum, char* vexs, int* arcs);
void OutMGraph(ALGraph G);
void VerDegree(ALGraph G);

/**
 * @brief 查找顶点在顶点数组中的位置
 *
 * @param G 图对象
 * @param vex 顶点数组
 * @return int
 */
int LocateVex(ALGraph G, char vex) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data == vex) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 创建图的邻接表存储结构
 *
 * @param GKind  图的类型(DG,DN,UDG,UDN)
 * @param G      图指针变量
 * @param vexnum 顶点数
 * @param arcnum 边(弧)数
 * @param vexs   顶点数组
 * @param arcs   边(弧)数组，每条边用三元组(i,j,w)表示
 * @return Status
 */
Status CreateALGraph(GraphKind GKind, ALGraph* G, int vexnum, int arcnum, char* vexs, int* arcs) {
    int i, k;
    int from, to, weight;
    ArcNode* p_arc_node;

    G->kind = GKind;
    G->arcnum = arcnum;
    G->vexnum = vexnum;

    for (i = 0; i < vexnum; i++) {
        G->vertices[i].data = vexs[i];
        G->vertices[i].firstarc = NULL;
    }

    for (k = 0; k < arcnum; k++) {
        from = arcs[k * 3];
        to = arcs[k * 3 + 1];
        weight = arcs[k * 3 + 2];

        p_arc_node = (ArcNode*)malloc(sizeof(ArcNode));
        if (!p_arc_node)
            return ERROR;

        p_arc_node->adjvex = to;
        p_arc_node->weight = weight;

        // 头插法插入新节点
        p_arc_node->nextarc = G->vertices[from].firstarc;
        G->vertices[from].firstarc = p_arc_node;

        if (GKind == UDG || GKind == UDN) {
            p_arc_node = (ArcNode*)malloc(sizeof(ArcNode));
            if (!p_arc_node)
                return ERROR;

            p_arc_node->adjvex = from;
            p_arc_node->weight = weight;

            // 头插法插入新节点
            p_arc_node->nextarc = G->vertices[to].firstarc;
            G->vertices[to].firstarc = p_arc_node->nextarc;
        }
    }

    return OK;
}

/**
 * @brief 输出图的邻接表结构
 *
 * @param G
 */
void OutMGraph(ALGraph G) {
    int i;
    ArcNode* p;
    char* kindStr[] = {"有向图", "有向网", "无向图", "无向网"};

    printf("\n图的类型: %s\n", kindStr[G.kind]);
    printf("顶点数: %d, 边(弧)数: %d\n", G.vexnum, G.arcnum);
    printf("\n顶点数组: ");
    for (i = 0; i < G.vexnum; i++) {
        printf("%c ", G.vertices[i].data);
    }
    printf("\n\n邻接表结构:\n");

    for (i = 0; i < G.vexnum; i++) {
        printf("[%d] %c -> ", i, G.vertices[i].data);
        p = G.vertices[i].firstarc;

        while (p != NULL) {
            // 对于有权图(网)，权值加括号表示
            if (G.kind == DN || G.kind == UDN) {
                printf("%c(%d) -> ", G.vertices[p->adjvex].data, p->weight);
            } else {
                printf("%c -> ", G.vertices[p->adjvex].data);
            }
            p = p->nextarc;
        }
        printf("NULL\n");
    }
}

/**
 * @brief 计算并输出各顶点的度
 *
 * @param G
 */
void VerDegree(ALGraph G) {
    int i;
    int* inDegree;  // 入度数组
    int outDegree;  // 出度
    ArcNode* p;

    printf("\n各顶点的度:\n");

    // 如果是有向的
    if (G.kind == DG || G.kind == DN) {
        inDegree = (int*)calloc(G.vexnum, sizeof(int));
        if (!inDegree) {
            printf("内存分配失败!\n");
            return;
        }

        /*
        A -- > B-- > C-- > NULL
        0 -- > 1-- > 2-- > NULL
        A -- > B-- > C-- > NULL
        A -- > B-- > C-- > NULL
        A -- > B-- > C-- > NULL
        */

        // 计算入度
        for (i = 0; i < G.vexnum; i++) {
            p = G.vertices[i].firstarc;
            while (p != NULL) {
                inDegree[p->adjvex]++;
                p = p->nextarc;
            }
        }

        // 输出入度和出度
        printf("顶点\t入度\t出度\t总度数\n");
        for (i = 0; i < G.vexnum; i++) {
            outDegree = 0;
            p = G.vertices[i].firstarc;
            while (p != NULL) {
                outDegree++;
                p = p->nextarc;
            }
            printf("%c\t%d\t%d\t%d\n", G.vertices[i].data,
                   inDegree[i], outDegree, inDegree[i] + outDegree);
        }

        free(inDegree);
    }
    // 如果是无向的
    else {
        printf("顶点\t度数\n");
        for (i = 0; i < G.vexnum; i++) {
            int degree = 0;
            p = G.vertices[i].firstarc;
            while (p != NULL) {
                degree++;
                p = p->nextarc;
            }
            printf("%c\t%d\n", G.vertices[i].data, degree);
        }
    }

    puts("===========================================");
}

void TestAllGraph() {
    ALGraph G;

    printf("\n创建有向图示例:\n");
    char vexs_0[] = {'0', '1', '2', '3'};
    int arcs_0[] = {
        0, 1, 1,
        1, 0, 1,
        1, 2, 1,
        2, 0, 1,
        2, 1, 1,
        3, 1, 1};
    CreateALGraph(DG, &G, 4, 6, vexs_0, arcs_0);
    OutMGraph(G);
    VerDegree(G);

    printf("\n创建有向网示例:\n");
    char vexs_1[] = {'0', '1', '2', '3'};
    int arcs_1[] = {
        0,
        2,
        50,
        1,
        2,
        45,
        2,
        0,
        64,
    };
    CreateALGraph(DN, &G, 3, 3, vexs_1, arcs_1);
    OutMGraph(G);
    VerDegree(G);

    printf("\n创建无向图示例:\n");
    char vexs_2[] = {'0', '1', '2', '3'};
    int arcs_2[] = {
        0,
        1,
        1,
        0,
        2,
        1,
        0,
        3,
        1,
        1,
        2,
        1,
        2,
        3,
        1,
    };
    CreateALGraph(UDG, &G, 4, 5, vexs_2, arcs_2);
    OutMGraph(G);
    VerDegree(G);

    printf("\n创建无向网示例:\n");
    char vexs_3[] = {'0', '1', '2', '3', 'E'};
    int arcs_3[] = {
        0,
        1,
        56,
        0,
        3,
        78,
        0,
        2,
        34,
        2,
        3,
        25,
    };
    CreateALGraph(UDN, &G, 4, 4, vexs_3, arcs_3);
    OutMGraph(G);
    VerDegree(G);
}

int main() {
    TestAllGraph();
    return 0;
}