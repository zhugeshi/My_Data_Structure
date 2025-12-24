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
typedef unsigned int VRType;
typedef char InfoType;
typedef char VertexType;
typedef enum {
    DG,
    DN,
    UDG,
    UDN
} GraphKind;  // 四种图的枚举类型

typedef struct ArcCell {
    VRType adj;      // 对无权图用1或0表示；对带权图，则为权值类型。
    InfoType* info;  // 该边或弧相关信息的指针(字符串)
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];  // 顶点数组，存放顶点信息, 最多20个
    AdjMatrix arcs;                   // 邻接矩阵
    int vexnum, arcnum;               // 图中顶点总数与弧数
    GraphKind kind;                   // 图的种类标志
} MGraph;

/**
 * @brief 查找顶点在顶点数组中的位置
 *
 * @param G 图变量
 * @param v 顶点值
 * @return int 表示是否成功
 */
int LocateVex(MGraph G, char v) {
    int i;
    for (i = 0; i < G.vexnum; i++) {
        if (G.vexs[i] == v) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 创建图的邻接矩阵存储的通用函数
 * 简单来说, 只要知道了一个图的邻接矩阵, 就可以知道它的数据结构了
 * @param GKind: 指定创建的图的类型 (DG-有向图, DN-有向网, UDG-无向图, UDN-无向网)
 * @param  G: 图对象
 * @param  vexnum: 图的顶点数
 * @param  arcnum: 图的边或弧数
 * @param  vexs: 顶点数组
 * @param  arcs: 边(弧)数组，每条边用三元组(i, j, w)表示
 * @return 是否创建成功
 */
Status CreateMGraph(GraphKind GKind, MGraph* G, int vexnum, int arcnum, char* vexs, int* arcs) {
    int i, j, k;
    int v1, v2, w;
    // 初始化基本数据
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;
    // 初始化顶点数组
    for (i = 0; i < vexnum; i++) {
        G->vexs[i] = vexs[i];
    }
    // 初始化邻接矩阵
    for (i = 0; i < vexnum; i++) {
        for (j = 0; j < vexnum; j++) {
            if (GKind == DG || GKind == UDG) {
                // 无权图，初始化为0
                G->arcs[i][j].adj = 0;
            } else {
                // 有权网，初始化为无穷大
                G->arcs[i][j].adj = INFINITY;
            }
            G->arcs[i][j].info = NULL;
        }
    }
    // 根据边(弧)数组构建邻接矩阵
    // arcs是一维数组，每3个元素表示一条边: arcs[k*3], arcs[k*3+1], arcs[k*3+2]
    for (k = 0; k < arcnum; k++) {
        v1 = arcs[k * 3];      // 第一个顶点的索引
        v2 = arcs[k * 3 + 1];  // 第二个顶点的索引
        w = arcs[k * 3 + 2];   // 权值
        // 检查数据是否合法
        if (v1 < 0 || v1 >= vexnum || v2 < 0 || v2 >= vexnum) {
            printf("错误: 顶点索引越界！\n");
            return ERROR;
        }
        if (GKind == DG) {
            // 有向图 (无权值) , 只有正向的数据才能初始化邻接矩阵
            G->arcs[v1][v2].adj = 1;
        } else if (GKind == DN) {
            // 有向网(有权值)
            G->arcs[v1][v2].adj = w;
        } else if (GKind == UDG) {
            // 无向图(无权值) ，对称矩阵
            G->arcs[v1][v2].adj = 1;
            G->arcs[v2][v1].adj = 1;
        } else if (GKind == UDN) {
            // 无向图(有权值), 对称矩阵
            G->arcs[v1][v2].adj = w;
            G->arcs[v2][v1].adj = w;
        }
    }
    return OK;
}
/**
 * @brief 输出图的顶点数组和邻接矩阵
 *
 * @param G 图变量
 */
void OutMGraph(MGraph G) {
    int i, j;
    const char* kindStr[] = {"有向图(DG)", "有向网(DN)", "无向图(UDG)", "无向网(UDN)"};
    printf("\n=========== 图的信息 ===========\n");
    printf("图的类型: %s\n", kindStr[G.kind]);
    printf("顶点数: %d, 边(弧)数: %d\n", G.vexnum, G.arcnum);
    // 输出顶点数组
    printf("\n顶点数组: ");
    for (i = 0; i < G.vexnum; i++) {
        printf("%c ", G.vexs[i]);
    }
    printf("\n");
    // 输出邻接矩阵
    printf("\n邻接矩阵:\n");
    printf("    ");
    for (i = 0; i < G.vexnum; i++) {
        printf("%4c", G.vexs[i]);
    }
    printf("\n");
    for (i = 0; i < G.vexnum; i++) {
        printf("%4c", G.vexs[i]);
        for (j = 0; j < G.vexnum; j++) {
            // 判断图的种类
            if (G.kind == DG || G.kind == UDG) {
                // 无权图，输出0或1
                printf("%4d", G.arcs[i][j].adj);
            } else {
                // 有权网，输出权值或∞
                if (G.arcs[i][j].adj == INFINITY) {
                    printf("   ∞");
                } else {
                    printf("%4d", G.arcs[i][j].adj);
                }
            }
        }
        printf("\n");
    }
    printf("================================\n");
}
/**
 * @brief 输出图中各顶点的度, 对于有向图，分别输出入度和出度
 *
 * @param G 图变量
 */
void VerDegree(MGraph G) {
    int i, j;
    int inDegree, outDegree, degree;
    printf("\n=========== 顶点的度 ===========\n");
    if (G.kind == DG || G.kind == DN) {
        // 有向图或有向网
        printf("顶点\t入度\t出度\t总度数\n");
        for (i = 0; i < G.vexnum; i++) {
            inDegree = 0;
            outDegree = 0;
            // 计算出度(第i行中非0且非∞的个数)
            for (j = 0; j < G.vexnum; j++) {
                if (G.arcs[i][j].adj != 0 && G.arcs[i][j].adj != INFINITY) {
                    outDegree++;
                }
            }
            // 计算入度(第i列中非0且非∞的个数)
            for (j = 0; j < G.vexnum; j++) {
                if (G.arcs[j][i].adj != 0 && G.arcs[j][i].adj != INFINITY) {
                    inDegree++;
                }
            }
            printf("%c\t%d\t%d\t%d\n", G.vexs[i], inDegree, outDegree, inDegree + outDegree);
        }
    } else {
        // 无向图或无向网
        printf("顶点\t度\n");
        for (i = 0; i < G.vexnum; i++) {
            degree = 0;
            // 计算度(第i行中非0且非∞的个数)
            for (j = 0; j < G.vexnum; j++) {
                if (G.arcs[i][j].adj != 0 && G.arcs[i][j].adj != INFINITY) {
                    degree++;
                }
            }
            printf("%c\t%d\n", G.vexs[i], degree);
        }
    }
    printf("================================\n");
}

// ==================== Prim算法相关 ====================
// 辅助数组的数据结构
typedef struct {
    VertexType adjvex;  // 最小边在U中的那个顶点
    VRType lowcost;     // 最小边上的权值
} ClosEdge;

/**
 * @brief 找到closedge数组中权值最小且不在U中的顶点
 *
 * @param closedge 辅助数组
 * @param vexnum 顶点数
 * @return int 返回顶点下标
 */
int Minimum(ClosEdge closedge[], int vexnum) {
    int i, k = -1;
    VRType min = INFINITY;
    for (i = 0; i < vexnum; i++) {
        // lowcost不为0表示该顶点不在U中
        if (closedge[i].lowcost != 0 && closedge[i].lowcost < min) {
            min = closedge[i].lowcost;
            k = i;
        }
    }
    return k;
}
/**
 * @brief 输出closedge数组
 *
 * @param closedge 辅助数组
 * @param vexs 顶点数组
 * @param vexnum 顶点数
 */
void PrintClosEdge(ClosEdge closedge[], VertexType vexs[], int vexnum) {
    int i;
    printf("closedge数组: ");
    for (i = 0; i < vexnum; i++) {
        if (closedge[i].lowcost == 0) {
            printf("(%c, 0) ", closedge[i].adjvex);
        } else if (closedge[i].lowcost == INFINITY) {
            printf("(%c, ∞) ", closedge[i].adjvex);
        } else {
            printf("(%c, %d) ", closedge[i].adjvex, closedge[i].lowcost);
        }
    }
    printf("\n");
}
/**
 * @brief Prim算法求最小生成树
 *
 * @param G 图
 * @param u 起始顶点
 */
void MiniSpanTree_Prim(MGraph G, VertexType u) {
    int i, col, min_vex;
    ClosEdge closedge[MAX_VERTEX_NUM];
    int start = LocateVex(G, u);  // 起始顶点的下标
    if (start == -1) {
        printf("错误: 顶点 %c 不存在！\n", u);
        return;
    }
    if (G.kind != UDN) {
        printf("错误: Prim算法只适用于无向网(UDN)！\n");
        return;
    }
    printf("\n========== Prim算法求最小生成树 ==========\n");
    printf("起始顶点: %c\n\n", u);
    // 初始化辅助数组closedge
    for (i = 0; i < G.vexnum; i++) {
        closedge[i].adjvex = u;
        closedge[i].lowcost = G.arcs[start][i].adj;
    }
    closedge[start].lowcost = 0;  // 起始顶点加入U集合
    printf("初始化closedge数组:\n");
    PrintClosEdge(closedge, G.vexs, G.vexnum);
    printf("\n");
    // 循环n-1次，每次选择一条最小边
    for (i = 1; i < G.vexnum; i++) {
        // 通过最小权值找到顶点
        min_vex = Minimum(closedge, G.vexnum);
        if (min_vex == -1) {
            printf("图不连通，无法生成最小生成树！\n");
            return;
        }
        // 输出这条最小生成边
        printf("第%d条边: (%c, %c) 权值: %d\n",
               i, closedge[min_vex].adjvex, G.vexs[min_vex], closedge[min_vex].lowcost);
        // 将顶点k加入U集合
        closedge[min_vex].lowcost = 0;
        // 更新closedge数组
        for (col = 0; col < G.vexnum; col++) {
            // 如果顶点j不在U中，且边(k,j)的权值小于当前lowcost
            if (G.arcs[min_vex][col].adj < closedge[col].lowcost) {
                closedge[col].adjvex = G.vexs[min_vex];
                closedge[col].lowcost = G.arcs[min_vex][col].adj;
            }
        }
        printf("更新后的");
        PrintClosEdge(closedge, G.vexs, G.vexnum);
        printf("\n");
    }
    printf("==========================================\n");
}

// ==================== Kruskal算法相关 ====================
// 边的数据结构
typedef struct {
    int head;     // 边的起点下标
    int tail;     // 边的终点下标
    VRType cost;  // 边的权值
} Edge;
/**
 * @brief 比较函数，用于qsort排序
 */
int CompareEdge(const void* a, const void* b) {
    const Edge* e1 = (const Edge*)a;
    const Edge* e2 = (const Edge*)b;
    return e1->cost - e2->cost;
}
/**
 * @brief 查找顶点所在的连通分量
 *
 * @param cnvx 连通分量数组
 * @param v 顶点下标
 * @return int 连通分量编号
 */
int FindSet(int cnvx[], int v) {
    while (cnvx[v] != v) {
        v = cnvx[v];
    }
    return v;
}
/**
 * @brief 输出连通分量数组
 *
 * @param cnvx 连通分量数组
 * @param vexs 顶点数组
 * @param vexnum 顶点数
 */
void PrintCnvx(int cnvx[], VertexType vexs[], int vexnum) {
    int i;
    printf("cnvx数组: ");
    for (i = 0; i < vexnum; i++) {
        printf("%c->%d ", vexs[i], cnvx[i]);
    }
    printf("\n");
}
/**
 * @brief Kruskal算法求最小生成树, 按照边的权值由小到大来进行遍历
 *
 * @param G 图
 */
void MiniSpanTree_Kruskal(MGraph G) {
    int i, j, added_arcs;
    Edge edges[MAX_VERTEX_NUM * MAX_VERTEX_NUM];  // 边数组
    int cnvx[MAX_VERTEX_NUM];                     // 连通分量数组
    int edgeCount = 0;                            // 边的数量
    int end1, end2;                               // 边的两个顶点所在的连通分量
    if (G.kind != UDN) {
        printf("错误: Kruskal算法只适用于无向网(UDN)！\n");
        return;
    }
    printf("\n========== Kruskal算法求最小生成树 ==========\n");
    /*
    typedef struct {
        VertexType vexs[MAX_VERTEX_NUM];  // 顶点数组，存放顶点信息, 最多20个
        AdjMatrix arcs;                   // 邻接矩阵
        int vexnum, arcnum;               // 图中顶点总数与弧数
        GraphKind kind;                   // 图的种类标志
    } MGraph;

    typedef struct ArcCell {
        VRType adj;      // 对无权图用1或0表示；对带权图，则为权值类型。
        InfoType* info;  // 该边或弧相关信息的指针(字符串)
    } ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    */
    // 初始化边数组
    for (i = 0; i < G.vexnum; i++) {
        for (j = i + 1; j < G.vexnum; j++) {  // j从i+1开始，避免重复
            if (G.arcs[i][j].adj != 0 && G.arcs[i][j].adj != INFINITY) {
                edges[edgeCount].head = i;
                edges[edgeCount].tail = j;
                edges[edgeCount].cost = G.arcs[i][j].adj;
                edgeCount++;
            }
        }
    }
    // 2. 对边按权值排序
    qsort(edges, edgeCount, sizeof(Edge), CompareEdge);
    printf("排序后的边数组:\n");
    for (i = 0; i < edgeCount; i++) {
        printf("(%c, %c) 权值: %d\n",
               G.vexs[edges[i].head], G.vexs[edges[i].tail], edges[i].cost);
    }
    printf("\n");
    // 3. 初始化连通分量数组，每个顶点自成一个连通分量
    for (i = 0; i < G.vexnum; i++) {
        cnvx[i] = i;
    }
    printf("初始化");
    PrintCnvx(cnvx, G.vexs, G.vexnum);
    printf("\n");
    added_arcs = 0;  // 已加入最小生成树的边数
    for (i = 0; i < edgeCount && added_arcs < G.vexnum - 1; i++) {
        end1 = FindSet(cnvx, edges[i].head);
        end2 = FindSet(cnvx, edges[i].tail);
        // 如果两个顶点不指向同一个终点
        if (end1 != end2) {
            added_arcs++;
            printf("第%d条边: (%c, %c) 权值: %d\n",
                   added_arcs, G.vexs[edges[i].head], G.vexs[edges[i].tail], edges[i].cost);
            // 更新终点
            cnvx[end2] = end1;
            printf("更新后的");
            PrintCnvx(cnvx, G.vexs, G.vexnum);
            printf("\n");
        } else {
            printf("跳过边 (%c, %c)，会形成回路\n",
                   G.vexs[edges[i].head], G.vexs[edges[i].tail]);
        }
    }
    if (added_arcs < G.vexnum - 1) {
        printf("图不连通，无法生成最小生成树！\n");
    }
    printf("==========================================\n");
}
// ==================== 测试函数 ====================
void TestMinSpanTree() {
    MGraph G;
    printf("\n================== 测试示例1 ==================\n");
    printf("创建测试用无向网:\n");
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs[] = {
        0, 1, 6,  // A-B: 6
        0, 2, 1,  // A-C: 1
        0, 3, 5,  // A-D: 5
        1, 2, 5,  // B-C: 5
        1, 4, 3,  // B-E: 3
        2, 3, 5,  // C-D: 5
        2, 4, 6,  // C-E: 6
        2, 5, 4,  // C-F: 4
        3, 5, 2,  // D-F: 2
        4, 5, 6   // E-F: 6
    };
    CreateMGraph(UDN, &G, 6, 10, vexs, arcs);
    OutMGraph(G);
    // 测试Prim算法 - 从不同顶点开始
    MiniSpanTree_Prim(G, 'A');
    MiniSpanTree_Prim(G, 'D');
    // 测试Kruskal算法
    MiniSpanTree_Kruskal(G);
    printf("\n================== 测试示例2 ==================\n");
    printf("创建较小的无向网:\n");
    char vexs2[] = {'A', 'B', 'C', 'D'};
    int arcs2[] = {
        0, 1, 10,  // A-B: 10
        0, 2, 15,  // A-C: 15
        0, 3, 20,  // A-D: 20
        1, 2, 35,  // B-C: 35
        1, 3, 25,  // B-D: 25
        2, 3, 30   // C-D: 30
    };
    MGraph G2;
    CreateMGraph(UDN, &G2, 4, 6, vexs2, arcs2);
    OutMGraph(G2);
    MiniSpanTree_Prim(G2, 'A');
    MiniSpanTree_Kruskal(G2);
}

int main() {
    TestMinSpanTree();
    return 0;
}