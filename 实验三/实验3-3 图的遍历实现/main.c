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

#define ARR_LEN(X) sizeof(X) / sizeof(X[0])  // 计算数组长度的宏

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
void DFSTraverse(ALGraph G);
void BFSTraverse(ALGraph G);

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
    int start, end, k, weight;
    ArcNode* p_arc;

    // 初始化图的基本信息
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;

    // 初始化顶点数组
    for (start = 0; start < vexnum; start++) {
        G->vertices[start].data = vexs[start];
        G->vertices[start].firstarc = NULL;
    }

    // 创建邻接表
    for (k = 0; k < arcnum; k++) {
        // 从arcs数组中读取边(弧)信息
        // arcs数组按照[i, j, w, i, j, w, ...]的顺序存储
        start = arcs[k * 3];       // 起点下标
        end = arcs[k * 3 + 1];     // 终点下标
        weight = arcs[k * 3 + 2];  // 权值

        // 创建边(弧)结点
        p_arc = (ArcNode*)malloc(sizeof(ArcNode));
        if (!p_arc)
            return ERROR;

        /* typedef struct ArcNode {
            int adjvex;               // 该边(弧)所依附(指向)的顶点在顶点数组的下标
            struct ArcNode* nextarc;  // 指向下一条边(弧)的指针
            int weight;               // 边(弧)的权值，无权图其值为0
        } ArcNode; */
        p_arc->adjvex = end;
        p_arc->weight = weight;

        // 采用头插法插入边表
        /* typedef struct {
            AdjList vertices;    // 图的顶点数组
            int vexnum, arcnum;  // 图的顶点数和边(弧)数
            GraphKind kind;      // 图的类型
        } ALGraph; */

        /* typedef struct VNode {
            VertexType data;    // 顶点数据
            ArcNode* firstarc;  // 指向第一条依附该顶点的边(弧)的指针
        } VNode, AdjList[MAX_VERTEX_NUM]; */
        p_arc->nextarc = G->vertices[start].firstarc;
        G->vertices[start].firstarc = p_arc;

        // 如果是无向图(UDG或UDN)，需要添加对称的边
        if (GKind == UDG || GKind == UDN) {
            p_arc = (ArcNode*)malloc(sizeof(ArcNode));
            if (!p_arc)
                return ERROR;

            p_arc->adjvex = start;
            p_arc->weight = weight;

            // 采用头插法插入边表
            p_arc->nextarc = G->vertices[end].firstarc;
            G->vertices[end].firstarc = p_arc;
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

// 访问标记数组,用于记录顶点是否被访问过
int visited[MAX_VERTEX_NUM];

/**
 * @brief DFS递归辅助函数 - 从顶点v开始深度优先遍历
 *
 * @param G 图对象
 * @param v 当前访问的顶点下标
 */
void DFS(ALGraph G, int v) {
    ArcNode* p;

    // 访问顶点v
    visited[v] = TRUE;
    printf("%c ", G.vertices[v].data);

    // 递归访问v的所有未访问的邻接顶点
    p = G.vertices[v].firstarc;
    while (p != NULL) {
        if (!visited[p->adjvex]) {
            DFS(G, p->adjvex);  // 递归调用
        }
        p = p->nextarc;
    }
}

/**
 * @brief 深度优先遍历 - 递归实现
 *
 * @param G 图对象
 */
void DFSTraverse(ALGraph G) {
    int i;

    // 初始化访问标记数组
    for (i = 0; i < G.vexnum; i++) {
        visited[i] = FALSE;
    }

    printf("\n深度优先遍历序列(DFS): ");

    // 对每个未访问的顶点调用DFS
    // 这样可以处理非连通图的情况
    for (i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            DFS(G, i);
        }
    }

    printf("\n");
}

/**
 * @brief 广度优先遍历 - 非递归实现(使用队列)
 *
 * @param G 图对象
 */
void BFSTraverse(ALGraph G) {
    int i, v;
    ArcNode* p;

    // 简单队列实现
    int queue[MAX_VERTEX_NUM];
    int front = 0, rear = 0;  // 队头和队尾指针

    // 初始化访问标记数组
    for (i = 0; i < G.vexnum; i++) {
        visited[i] = FALSE;
    }

    printf("\n广度优先遍历序列(BFS): ");

    // 对每个未访问的顶点进行BFS
    // 这样可以处理非连通图的情况
    /*
    [0] 0 -> 3 -> 2 -> 1 -> NULL
    [1] 1 -> 2 -> NULL
    [2] 2 -> 3 -> NULL
    [3] 3 -> NULL
    */
    for (i = 0; i < G.vexnum; i++) {
        if (!visited[i]) {
            // 访问顶点i并入队
            visited[i] = TRUE;
            printf("%c ", G.vertices[i].data);
            queue[rear++] = i;

            // 队列非空时循环
            while (front < rear) {
                // 出队
                v = queue[front++];

                // 访问v的所有未访问的邻接顶点
                p = G.vertices[v].firstarc;
                while (p != NULL) {
                    if (!visited[p->adjvex]) {
                        // 访问邻接顶点并入队
                        visited[p->adjvex] = TRUE;
                        printf("%c ", G.vertices[p->adjvex].data);
                        queue[rear++] = p->adjvex;
                    }
                    p = p->nextarc;
                }
            }
        }
    }

    printf("\n");
}

void TestFunc() {
    ALGraph G;
    int choice;

    printf("\n创建无向图示例\n");
    char vexs[] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
    };
    int arcs[] = {
        0,
        1,
        1,
        0,
        2,
        1,
        1,
        3,
        1,
        1,
        4,
        1,
        2,
        5,
        1,
        4,
        5,
        1,
        3,
        4,
        1,
    };
    int vexnum = ARR_LEN(vexs);
    int arcnum = ARR_LEN(arcs) / 3;
    CreateALGraph(UDG, &G, vexnum, arcnum, vexs, arcs);
    OutMGraph(G);
    DFSTraverse(G);
    BFSTraverse(G);
}

int main() {
    TestFunc();

    printf("\n程序执行完毕!\n");
    return 0;
}