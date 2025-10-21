#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define LIST_INIT_SET 100
#define LISTINCREMENT  10
#define ELEMTYPE       char

typedef struct sequence_list {
    ELEMTYPE    *elem;
    int         length;
    int         listsize;
} SeqList, *PSeqList;

/* *
 * 用于初始化提供的线性表
 * 参数: 
 *      1. PSeqList sq 目标顺序表的地址
 * 返回值:
 *      提供的顺序表的地址
 */
PSeqList InitList(PSeqList sq) {
    /* 初始化一段内存空间 */
    sq->elem = (ELEMTYPE*)malloc(LIST_INIT_SET * (sizeof(ELEMTYPE))); 
    if (sq->elem == NULL) { return NULL; }

    /* 设置结构体参数 */
    sq->length = 0;
    sq->listsize = LIST_INIT_SET;
    return sq;
}

/* 用于打印线性表中的元素内容 */
void PrintSqList(PSeqList sq) {
    for (size_t i = 1; i <= sq->length; i++) {
        printf("索引 %-5d -> 对应的值为%d\n", i, sq->elem[i - 1]);
    }
}

/**
 * 用于在顺序表中插入元素内容
 * 参数:
 *      1. PSeqList sq      要操作的顺序表对象
 *      2. int index        插入的位置
 *      3. ELEMTYPE elem    插入元素的值
 * 返回值:
 *      操作对象的地址
 */
void InsertSqList(PSeqList sq, int index, ELEMTYPE elem) {
    /* 校验index */
    if ((index < 1) || (index > sq->length + 1)) { 
        printf("Invalid index", stderr);
        return;
    } 

    /* 如果内存不足,则重新分配 */
    if (sq->length >= sq->listsize) {
        sq->elem = (ELEMTYPE*)realloc(sq->elem, (sq->listsize + LISTINCREMENT) * sizeof(ELEMTYPE));
        sq->listsize += LISTINCREMENT;
    }

    /* 完成数据插入 */
    for (ELEMTYPE *end = &sq->elem[sq->length]; end >= &sq->elem[index]; end--) {
        *end = *(end - 1);
    }
    sq->elem[index - 1] = elem;
    sq->length++;
}

/**
 * 用于删除顺序表中指定位置的元素
 * 参数:
 *      1. PSeqList sq  操作对象
 *      2. int index    索引
 * 返回值:
 *      操作对象 
 */
void DeleteSqList(PSeqList sq, int index) {
    // 校验索引
    if ((index < 1) || (index > sq->length)) {
        printf("Invalid index", stderr);
    }
    
    for (size_t i = index; i < sq->length; i++) {
        sq->elem[index - 1] = sq->elem[index];
    }
    sq->length--;
}

int main(void) {
    PSeqList p_Sq; 
    InitList(p_Sq); 
    for (size_t i = 1; i < 22; i++) {
        InsertSqList(p_Sq, i, i);
    }
    InsertSqList(p_Sq, 8, 124);
    DeleteSqList(p_Sq, 8);
    PrintSqList(p_Sq);

    return 0;
}