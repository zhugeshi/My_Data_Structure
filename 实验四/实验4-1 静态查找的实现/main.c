#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef int KeyType;  // 关键字的数据类型
// 数据元素（记录）的类型定义

typedef struct {
    KeyType key;       // 学号(关键字)
    const char* name;  // 姓名
    const char* sex;   // 性别
    int age;           // 年龄
} RecordType;
// 静态查找表的类型定义

typedef struct {
    RecordType* elem;  // 数据元素存储空间的基址(一维数组名)
    int length;        // 表的长度（元素个数）
} SSTable;

int count = 0;  // 全局变量，用于递归折半查找时统计比较次数

/**
 * @brief 创建静态查找表，从下标1开始存放数据，0位置可用作哨兵
 * 
 * @param ST 静态查找表
 * @param n 元素个数
 * @return int 是否创建成功
 */
int CreateSSTable(SSTable* ST, int n) {
    int i;
    // 分配空间，需要n+1个空间（0号位置作为哨兵）
    ST->elem = (RecordType*)malloc((n + 1) * sizeof(RecordType));
    if (!ST->elem) {
        return ERROR;
    }
    ST->length = n;
    
    // 初始化数据（从下标1开始）
    ST->elem[1].key = 56;  ST->elem[1].name = "Zhang";  ST->elem[1].sex = "F";  ST->elem[1].age = 19;
    ST->elem[2].key = 19;  ST->elem[2].name = "Wang";   ST->elem[2].sex = "F";  ST->elem[2].age = 20;
    ST->elem[3].key = 80;  ST->elem[3].name = "Zhou";   ST->elem[3].sex = "F";  ST->elem[3].age = 19;
    ST->elem[4].key = 5;   ST->elem[4].name = "Huang";  ST->elem[4].sex = "M";  ST->elem[4].age = 20;
    ST->elem[5].key = 21;  ST->elem[5].name = "Zheng";  ST->elem[5].sex = "M";  ST->elem[5].age = 20;
    ST->elem[6].key = 64;  ST->elem[6].name = "Li";     ST->elem[6].sex = "M";  ST->elem[6].age = 19;
    ST->elem[7].key = 88;  ST->elem[7].name = "Liu";    ST->elem[7].sex = "F";  ST->elem[7].age = 18;
    ST->elem[8].key = 13;  ST->elem[8].name = "Qian";   ST->elem[8].sex = "F";  ST->elem[8].age = 19;
    ST->elem[9].key = 37;  ST->elem[9].name = "Sun";    ST->elem[9].sex = "M";  ST->elem[9].age = 20;
    ST->elem[10].key = 75; ST->elem[10].name = "Zhao";  ST->elem[10].sex = "M"; ST->elem[10].age = 20;
    ST->elem[11].key = 92; ST->elem[11].name = "Chen";  ST->elem[11].sex = "M"; ST->elem[11].age = 20;
    
    return OK;
}

/**
 * @brief 遍历静态查找表，输出所有元素
 * 
 * @param ST 静态查找表
 */
void TraversSSTable(SSTable ST) {
    int i;
    printf("%-10s %-10s %-10s %-10s\n", "学号key", "姓名name", "性别sex", "年龄age");
    printf("------------------------------------------------\n");
    for (i = 1; i <= ST.length; i++) {
        printf("%-10d %-10s %-10s %-10d\n", 
               ST.elem[i].key, ST.elem[i].name, ST.elem[i].sex, ST.elem[i].age);
    }
}

/**
 * @brief 输出一个元素（记录）
 * 
 * @param ST 静态查找表
 * @param i 元素下标
 */
void OutElem(SSTable ST, int i) {
    printf("学号: %d, 姓名: %s, 性别: %s, 年龄: %d\n",
           ST.elem[i].key, ST.elem[i].name, ST.elem[i].sex, ST.elem[i].age);
}

/**
 * @brief 顺序查找（带哨兵）
 * 
 * @param ST 静态查找表
 * @param key 要查找的关键字
 * @return int 找到返回下标，未找到返回0
 */
int SearchSSTable_Seq(SSTable ST, KeyType key) {
    int i;
    ST.elem[0].key = key;  // 设置哨兵
    i = ST.length;
    while (ST.elem[i].key != key) {
        i--;
    }
    return i;  // 找到返回下标，未找到返回0
}

/**
 * @brief 对静态查找表按关键字递增排序（冒泡排序）
 * 
 * @param ST 静态查找表
 */
void SortSSTable(SSTable* ST) {
    int i, j;
    RecordType temp;
    for (i = 1; i < ST->length; i++) {
        for (j = 1; j <= ST->length - i; j++) {
            if (ST->elem[j].key > ST->elem[j + 1].key) {
                temp = ST->elem[j];
                ST->elem[j] = ST->elem[j + 1];
                ST->elem[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief 折半查找（非递归）
 * 
 * @param ST 静态查找表（必须有序）
 * @param key 要查找的关键字
 * @param c 比较次数（输出参数）
 * @return int 找到返回下标，未找到返回0
 */
int SearchSSTable_Bin(SSTable ST, KeyType key, int* c) {
    int low = 1, high = ST.length, mid;
    *c = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        (*c)++;
        if (ST.elem[mid].key == key) {
            return mid;  // 找到
        } else if (ST.elem[mid].key < key) {
            low = mid + 1;  // 在右半部分
        } else {
            high = mid - 1;  // 在左半部分
        }
    }
    return 0;  // 未找到
}

/**
 * @brief 折半查找（递归）
 * 
 * @param ST 静态查找表（必须有序）
 * @param key 要查找的关键字
 * @param low 查找区间下界
 * @param high 查找区间上界
 * @return int 找到返回下标，未找到返回0
 */
int SearchSSTable_Bin1(SSTable ST, KeyType key, int low, int high) {
    int mid;
    if (low > high) {
        return 0;  // 未找到
    }
    mid = (low + high) / 2;
    count++;  // 比较次数加1
    if (ST.elem[mid].key == key) {
        return mid;  // 找到
    } else if (ST.elem[mid].key < key) {
        return SearchSSTable_Bin1(ST, key, mid + 1, high);  // 在右半部分
    } else {
        return SearchSSTable_Bin1(ST, key, low, mid - 1);  // 在左半部分
    }
}

int main() {
    int i, key;
    int n = 11;  // 11个元素
    int c, total;
    SSTable ST1;
    
    // 创建静态查找表ST1
    CreateSSTable(&ST1, n);
    printf("\n学号无序的静态查找表\n");
    TraversSSTable(ST1);
    
    printf("\n---顺序查找---\n");
    printf("请输入要查找的学号（-1退出）：");
    scanf("%d", &key);
    while (key != -1) {
        i = SearchSSTable_Seq(ST1, key);
        if (i == 0)
            printf("当前要查找的记录不存在！\n");
        else
            OutElem(ST1, i);  // 输出找到的记录
        printf("查找时比较的次数=%d\n", ST1.length - i + 1);
        printf("\n请输入要查找的学号（-1退出）：");  // 下一次查找
        scanf("%d", &key);
    }
    
    printf("\n顺序查找各关键字的比较次数：\n");
    total = 0;
    for (i = 1; i <= ST1.length; i++) {
        c = SearchSSTable_Seq(ST1, ST1.elem[i].key);
        total = total + ST1.length - c + 1;
        printf("关键字=%2d  比较次数=%d\n", ST1.elem[i].key, ST1.length - c + 1);
    }
    printf("\n查找成功的平均查找长度ASL=%.2f\n", (float)total / ST1.length);
    printf("查找不成功的平均查找长度ASL=%.2f\n", (float)(ST1.length + 1));
    
    // 查找表排序
    SortSSTable(&ST1);
    printf("\n学号有序的静态查找表\n");
    TraversSSTable(ST1);
    
    printf("\n---折半查找(递归)---\n");
    printf("请输入要查找的学号（-1退出）：");
    scanf("%d", &key);
    while (key != -1) {
        count = 0;
        i = SearchSSTable_Bin1(ST1, key, 1, ST1.length);
        if (i == 0)
            printf("当前要查找的记录不存在！\n");
        else
            OutElem(ST1, i);  // 输出找到的记录
        printf("查找时比较的次数=%d\n", count);
        printf("\n请输入要查找的学号（-1退出）：");  // 下一次查找
        scanf("%d", &key);
    }
    
    printf("\n折半查找各关键字的比较次数：\n");
    total = 0;
    for (i = 1; i <= ST1.length; i++) {
        count = 0;
        SearchSSTable_Bin1(ST1, ST1.elem[i].key, 1, ST1.length);
        total = total + count;
        printf("关键字=%2d  比较次数=%d\n", ST1.elem[i].key, count);
    }
    printf("\n查找成功的平均查找长度ASL=%.2f\n", (float)total / ST1.length);
    
    printf("\n折半查找不成功时各关键字的比较次数：\n");
    total = 0;
    for (i = 0; i <= ST1.length; i++) {
        count = 0;
        if (i == 0)
            SearchSSTable_Bin1(ST1, 4, 1, ST1.length);
        else
            SearchSSTable_Bin1(ST1, ST1.elem[i].key + 1, 1, ST1.length);
        total = total + count;
        printf("关键字=%2d  比较次数=%d\n", (i == 0) ? 4 : ST1.elem[i].key + 1, count);
    }
    printf("\n查找不成功的平均查找长度ASL=%.2f\n", (float)total / (ST1.length + 1));
    
    // 释放内存
    free(ST1.elem);
    
    return 1;
}
