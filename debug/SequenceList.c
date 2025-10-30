#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXSIZE 100

typedef int ELEMTYPE;
typedef int STATUS;

typedef struct SqList {
  ELEMTYPE *elem;
  int length;
} *PSqList;

/**
 * 利用new函数申请存储空间，构造一个空的顺序表表L，L的初始大小为MAXSIZE
 * 成功返回OK
 */
STATUS InitList(PSqList sq) {
  ELEMTYPE *new_alloc = (ELEMTYPE *)malloc(MAXSIZE);
  if (new_alloc == NULL) {
    perror("Init SqList failed.\n");
    return FALSE;
  }

  sq->elem = new_alloc;
  sq->length = 0;
  return OK;
}

/**
 * 将顺序表的数据清空.
 * 将L.length清零，即L.length=0即可
 */
STATUS ClearList(PSqList sq) {
  sq->length = 0;
  return OK;
}

/**
 * 返回L中数据元素个数
 */
int ListLength(PSqList sq) { return sq->length; }

/**
 * 用e返回L中第i个数据元素的值，1≤i≤L.Length
 */
STATUS GetElem(PSqList sq, int idx, ELEMTYPE *elem) {
  // 检验索引是否合法
  if (idx < 1 || idx > sq->length) {
    perror("Index is invalid.\n");
    return FALSE;
  }

  ELEMTYPE *ptemp_elem = sq->elem;
  for (size_t i = 0; i < idx; i++, ptemp_elem++)
    ;
  *elem = *ptemp_elem;

  return OK;
}

/**
 * 返回L中第1个与e相等的数据元素位序，若e不存在，则返回0
 */
int LocateList(PSqList sq, ELEMTYPE elem) {
  ELEMTYPE *ptemp_elem = sq->elem;
  int sq_lenth = ListLength(sq);
  int index = 1;
  do {
    if (*ptemp_elem == elem) {
      return index;
    }
    index++;
    ptemp_elem++;
  } while (index <= sq_lenth);

  puts("Can't find the element. Return value is 0.\n");
  return 0;
}

/* 用于打印线性表中的元素内容 */
void PrintSqList(PSqList sq) {
  for (size_t i = 1; i <= sq->length; i++) {
    printf("索引 %-5d -> 对应的值为%d\n", (int)i, sq->elem[i - 1]);
  }
}

/**
 * 在L中的第i个位置之前插入新的数据元素e
 */
STATUS ListInsert(PSqList sq, int idx, ELEMTYPE elem) {
  /* 校验index */
  if ((idx < 1) || (idx > sq->length + 1)) {
    printf("Invalid index.\n", stderr);
    return FALSE;
  }

  // 检测大小是否合理
  if (ListLength(sq) >= 99) {
    perror("SqList size is too huge.\n");
    return FALSE;
  }

  /* 完成数据插入 */
  for (ELEMTYPE *end = &sq->elem[sq->length]; end >= &sq->elem[idx]; end--) {
    *end = *(end - 1);
  }
  sq->elem[idx - 1] = elem;
  sq->length++;
  return OK;
}

/**
 * 删除L中的第i个数据元素，用elem返回其值，1≤i≤L.length
 */
STATUS ListDelete(PSqList sq, int idx, ELEMTYPE *elem) {
  // 校验索引
  if ((idx < 1) || (idx > sq->length)) {
    printf("Invalid index", stderr);
    return FALSE;
  }

  *elem = sq->elem[idx - 1];

  for (size_t i = idx; i <= sq->length; i++) {
    sq->elem[i - 1] = sq->elem[i];
  }
  sq->length--;

  return OK;
}

/**
 * 利用printf函数依次输出L的每个数据元素的值
 */
void ListTraverse(PSqList sq) {
  int sq_len = ListLength(sq);
  printf("List length is %d.\n", sq_len);
  for (int i = 0; i < sq_len; i++) {
    printf("List %-2d value is -> %5d\n", i + 1, sq->elem[i]);
  }
}

int main(void) {
  PSqList psq_list;
  InitList(psq_list);
  for (int i = 1; i <= 20; i++) {
    ListInsert(psq_list, i, i);
  }
  ListTraverse(psq_list);
  puts("");

  ListInsert(psq_list, 10, 10);
  int del_value = 0;

  int target_val = 10;
  printf("The first index of value %d is %d", target_val,
         LocateList(psq_list, target_val));

  free(psq_list);
  return 0;
}