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

/*
typedef struct {
    GraphKind kind;
    int arcnum, vexnum;
    VertexType vexs[MAX_VERTEX_NUM];
    AdjMatrix arcs;
};
*/

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
 *      简单来说, 只要知道了一个图的邻接矩阵, 就可以知道它的数据结构了
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

// 测试所有示例
void TestAllExamples() {
    MGraph G;

    // 示例1: 创建一个无向图(UDG)
    printf("\n示例1: 创建无向图\n");
    // 顶点数组
    char vexs1[] = {'0', '1', '2', '3'};
    // 表示边的信息
    int arcs1[] = {
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
    CreateMGraph(UDG, &G, 4, 5, vexs1, arcs1);
    OutMGraph(G);
    VerDegree(G);

    // 示例2: 创建一个有向图(DG)
    printf("\n示例2: 创建有向图\n");
    char vexs2[] = {'0', '1', '2', '3'};
    int arcs2[] = {
        0,
        1,
        1,
        1,
        0,
        1,
        2,
        0,
        1,
        1,
        2,
        1,
        2,
        1,
        1,
        3,
        1,
        1,
    };
    CreateMGraph(DG, &G, 4, 5, vexs2, arcs2);
    OutMGraph(G);
    VerDegree(G);

    // 示例3: 创建一个无向网(UDN)
    printf("\n示例3: 创建有权无向网\n");
    char vexs3[] = {'0', '1', '2', '3'};
    int arcs3[] = {
        0,
        1,
        56,

        0,
        2,
        34,

        0,
        3,
        78,

        2,
        3,
        25,
    };
    CreateMGraph(UDN, &G, 4, 4, vexs3, arcs3);
    OutMGraph(G);
    VerDegree(G);

    // 示例4: 创建一个有向网(DN)
    printf("\n示例4: 创建有权有向网\n");
    char vexs4[] = {'0', '1', '2', '3'};
    int arcs4[] = {
        0,
        2,
        50,

        2,
        0,
        64,

        1,
        2,
        45,
    };
    CreateMGraph(DN, &G, 4, 3, vexs4, arcs4);
    OutMGraph(G);
    VerDegree(G);
}

int main() {
    TestAllExamples();
    return 0;
}
