#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef int ELEMTYPE;
typedef int STATUS;

typedef struct SqList {
    ELEMTYPE* elem;
    int length;
} SqList, *PSqList;

/**
 * 利用new函数申请存储空间，构造一个空的顺序表表L，L的初始大小为MAXSIZE
 * 成功返回OK
 */
STATUS InitList(PSqList sq) {
    ELEMTYPE* new_alloc = (ELEMTYPE*)malloc(MAXSIZE * sizeof(ELEMTYPE));
    if (new_alloc == NULL) {
        fprintf(stderr, "Init SqList failed: memory allocation error.\n");
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
int ListLength(PSqList sq) {
    return sq->length;
}

/**
 * 用e返回L中第i个数据元素的值，1≤i≤L.Length
 */
STATUS GetElem(PSqList sq, int idx, ELEMTYPE* elem) {
    // 检验索引是否合法
    if (idx < 1 || idx > sq->length) {
        fprintf(stderr, "Index is invalid.\n");
        return FALSE;
    }

    *elem = sq->elem[idx - 1];
    return OK;
}

/**
 * 返回L中第1个与e相等的数据元素位序，若e不存在，则返回0
 */
int LocateList(PSqList sq, ELEMTYPE elem) {
    ELEMTYPE* ptemp_elem = sq->elem;
    int sq_length = ListLength(sq);
    int index = 1;
    
    while (index <= sq_length) {
        if (*ptemp_elem == elem) {
            return index;
        }
        index++;
        ptemp_elem++;
    }

    fprintf(stderr, "Can't find the value.\n");
    return ERROR;
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
    if ((idx < 1) || (idx > sq->length + 1)) { // 如果是在末尾插入,idx可以是长度加1
        fprintf(stderr, "Invalid index.\n");
        return FALSE;
    }

    /* 检测大小是否合理 */
    if (ListLength(sq) >= MAXSIZE) {
        fprintf(stderr, "SqList size is too huge.\n");
        return FALSE;
    }

    /* 完成数据插入 */
    for (ELEMTYPE* end = &sq->elem[sq->length]; end >= &sq->elem[idx]; end--) {
        *end = *(end - 1);
    }
    sq->elem[idx - 1] = elem;
    sq->length++;
    return OK;
}

/**
 * 删除L中的第i个数据元素，用elem返回其值，1≤i≤L.length
 */
STATUS ListDelete(PSqList sq, int idx, ELEMTYPE* elem) {
    // 校验索引
    if ((idx < 1) || (idx > sq->length)) {
        fprintf(stderr, "Invalid index\n");
        return FALSE;
    }

    *elem = sq->elem[idx - 1];

    // 将后续元素前移
    for (int i = idx; i < sq->length; i++) {
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
    printf("List len is %d.\n", sq_len);
    for (int i = 0; i < sq_len; i++) {
        printf("List %-2d value is -> %5d\n", i + 1, sq->elem[i]);
    }
}

/**
 * 并运算 La=La U Lb
 * 保留所有元素,包括重复元素,相当于讲两个线性表全部合并
 */
STATUS Union(PSqList target_sq, SqList other_sq) {
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }

    // 检查other_sq是否为空或未初始化
    if (other_sq.elem == NULL || other_sq.length == 0) {
        return OK;  // 如果other_sq为空，直接返回
    }

    // 在最后插入所有other_sq中的元素（包括重复元素）
    for (int i = 0; i < other_sq.length; i++) {
        ELEMTYPE tmp_elem = other_sq.elem[i];
        if (ListLength(target_sq) >= MAXSIZE) {
            fprintf(stderr, "Union failed: target list overflow.\n");
            return OVERFLOW;
        }
        if (ListInsert(target_sq, target_sq->length + 1, tmp_elem) != OK) {
            fprintf(stderr, "Union failed: insert error.\n");
            return ERROR;
        }
    }

    return OK;
}

/**
 * 交运算 La=La ∩ Lb
 * 修改target_sq，使其只包含与other_sq相同的元素
 */
STATUS InterSection(PSqList target_sq, SqList other_sq) {
    // 检查输入是否合法
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }
    
    // 检查other_sq是否为空或未初始化
    if (other_sq.elem == NULL || other_sq.length == 0) {
        ClearList(target_sq);  // 如果other_sq为空，交集为空
        return OK;
    }

    SqList temp_sq;
    if (InitList(&temp_sq) != OK) {
        fprintf(stderr, "Failed to initialize temp list.\n");
        return ERROR;
    }

    /*
    a = [1, 2, 2, 3, 4, 5]
    b = [2, 3, 4, 5, 5]
    tmp = [2, 3, 4, 5]
    */

    // 寻找相同的部分，正确处理重复元素
    for (int i = 0; i < other_sq.length; i++) {
        ELEMTYPE tmp_elem = other_sq.elem[i];

        // 在临时表中寻找是否已经存在元素
        int already_in_temp = LocateList(&temp_sq, tmp_elem);
        if (already_in_temp != 0) {
            continue;  
        }
        
        // 在目标的线性表中寻找是否存在元素
        int find_idx = LocateList(target_sq, tmp_elem);
        if (find_idx != 0) {
            // 找到相同的元素，添加到临时表中
            if (ListInsert(&temp_sq, temp_sq.length + 1, tmp_elem) != OK) {
                fprintf(stderr, "Failed to insert element to temp list.\n");
                free(temp_sq.elem);
                return ERROR;
            }
        }
    }

    // 清空原表并将交集结果复制回去
    ClearList(target_sq);
    for (int i = 0; i < temp_sq.length; i++) {
        if (ListInsert(target_sq, target_sq->length + 1, temp_sq.elem[i]) != OK) {
            fprintf(stderr, "Failed to copy intersection result.\n");
            free(temp_sq.elem);
            return ERROR;
        }
    }

    free(temp_sq.elem);
    return OK;
}

/**
 * 差运算 La=La - Lb
 * 保留在target_sq中但不在other_list中的元素
 */
STATUS Difference(PSqList target_sq, SqList other_list) {
    // 检查参数输入
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }
    if (other_list.elem == NULL || other_list.length == 0) {
        return OK;  // 如果other_list为空，直接返回
    }

    // 创建一个临时表来存储临时结果
    SqList temp_sq;
    if (InitList(&temp_sq) != OK) {
        fprintf(stderr, "Failed to initialize temp list.\n");
        return ERROR;
    }

    // 遍历target_sq中的每个元素
    for (int i = 0; i < target_sq->length; i++) {
        ELEMTYPE tmp_elem = target_sq->elem[i];
        
        // 检查该元素是否在other_list中
        int find_idx = LocateList(&other_list, tmp_elem);
        if (find_idx == 0) {
            // 如果不在other_list中，添加到临时表
            if (ListInsert(&temp_sq, temp_sq.length + 1, tmp_elem) != OK) {
                fprintf(stderr, "Failed to insert element to temp list.\n");
                free(temp_sq.elem);
                return ERROR;
            }
        }
    }

    // 清空原表并将差集结果复制回去
    ClearList(target_sq);
    for (int i = 0; i < temp_sq.length; i++) {
        if (ListInsert(target_sq, target_sq->length + 1, temp_sq.elem[i]) != OK) {
            fprintf(stderr, "Failed to copy difference result.\n");
            free(temp_sq.elem);
            return ERROR;
        }
    }

    free(temp_sq.elem);
    return OK;
}

/**
 * 两个有序表的合并
 * 设La和Lb中的元素是非递减有序的，合并后Lc也是非递减有序的
 * 使用归并算法合并两个有序表
 */
STATUS MergeList(SqList fir_sq, SqList sec_sq, SqList *out_sq) {
    // 检查输入参数
    if (out_sq == NULL) {
        fprintf(stderr, "Output list Lc is NULL.\n");
        return ERROR;
    }
    if (fir_sq.elem == NULL || sec_sq.elem == NULL) {
        fprintf(stderr, "Input lists are not initialized.\n");
        return ERROR;
    }

    // 初始化out
    if (InitList(out_sq) != OK) {
        fprintf(stderr, "Failed to initialize output list Lc.\n");
        return ERROR;
    }

    // 双指针插入法
    int fir_idx = 0, sec_idx = 0;      
    int out_idx = 0;

    while (fir_idx < fir_sq.length && sec_idx < sec_sq.length) {
        if (fir_sq.elem[fir_idx] <= sec_sq.elem[sec_idx]) {
            if (ListInsert(out_sq, out_idx + 1, fir_sq.elem[fir_idx]) != OK) {
                fprintf(stderr, "Failed to insert element from La.\n");
                free(out_sq->elem);
                return ERROR;
            }
            fir_idx++;
            out_idx++;
        } else {
            if (ListInsert(out_sq, out_idx + 1, sec_sq.elem[sec_idx]) != OK) {
                fprintf(stderr, "Failed to insert element from Lb.\n");
                free(out_sq->elem);
                return ERROR;
            }
            sec_idx++;
            out_idx++;
        }
    }

    while (fir_idx < fir_sq.length) {
        if (ListInsert(out_sq, out_idx + 1, fir_sq.elem[fir_idx]) != OK) {
            fprintf(stderr, "Failed to insert remaining element from La.\n");
            free(out_sq->elem);
            return ERROR;
        }
        fir_idx++;
        out_idx++;
    }

    while (sec_idx < sec_sq.length) {
        if (ListInsert(out_sq, out_idx + 1, sec_sq.elem[sec_idx]) != OK) {
            fprintf(stderr, "Failed to insert remaining element from Lb.\n");
            free(out_sq->elem);
            return ERROR;
        }
        sec_idx++;
        out_idx++;
    }

    return OK;
}

/**
 * 去掉顺序表中的重复多余元素
 * 设Lc是一个有重复元素的顺序表，去掉重复元素
 * 使用双指针法进行原地去重
 */
STATUS Purge(SqList *sq) {
    // 检查输入参数
    if (sq == NULL || sq->elem == NULL) {
        fprintf(stderr, "Input list is NULL or not initialized.\n");
        return ERROR;
    }

    // 如果表为空或只有一个元素，不需要去重
    if (sq->length <= 1) {
        return OK;
    }

    // 快慢指针
    // a = [1,1,1,2,2,3,4]
    // func(a) = [1,2,3,4]
    int slow = 0;
    int fast = 1; 
    while (fast < sq->length) {
        if (sq->elem[fast] != sq->elem[slow]) {
            slow++;
            sq->elem[slow] = sq->elem[fast];
        }
        fast++;
    }
    sq->length = slow + 1;

    return OK;
}

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

int main(void) {
    printf("=== 第一题 ===\n");
    ELEMTYPE arr_a[] = {2, 8, 27, 39, 66, 77, 89};
    ELEMTYPE arr_b[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};

    SqList LA, LB;
    PSqList pLA = &LA;
    PSqList pLB = &LB;

    InitList(pLA);
    InitList(pLB);

    for (int i = 0; i < ARRLEN(arr_a); i++) {
        ListInsert(pLA, i + 1, arr_a[i]);
    }
    for (int i = 0; i < ARRLEN(arr_b); i++) {
        ListInsert(pLB, i + 1, arr_b[i]);
    }
    
    printf("表LA: ");
    ListTraverse(pLA);
    printf("表LB: ");
    ListTraverse(pLB);

    printf("\n=== 第二题 ===\n");
    
    // 保存LA的原始数据用于恢复
    SqList LA_backup;
    InitList(&LA_backup);
    for (int i = 0; i < LA.length; i++) {
        ListInsert(&LA_backup, i + 1, LA.elem[i]);
    }

    printf("并运算 LA = LA ∪ LB\n");
    Union(pLA, LB);
    printf("并运算结果: ");
    ListTraverse(pLA);

    // 恢复LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("\n交运算 LA = LA ∩ LB\n");
    InterSection(pLA, LB);
    printf("交运算结果: ");
    ListTraverse(pLA);

    // 恢复LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("\n差运算 LA = LA - LB\n");
    Difference(pLA, LB);
    printf("差运算结果: ");
    ListTraverse(pLA);

    printf("\n=== 第三题 ===\n");
    
    // 恢复LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("有序表合并 MergeList(LA, LB, LC)\n");
    SqList LC;
    if (MergeList(LA, LB, &LC) == OK) {
        printf("合并结果LC: ");
        ListTraverse(&LC);
    }

    printf("\n=== 第四题 ===\n");

    printf("\n去重运算 Purge(LC)\n");
    if (Purge(&LC) == OK) {
        printf("去重后LC: ");
        ListTraverse(&LC);
    }

    // 释放所有内存
    free(LA.elem);
    free(LB.elem);
    free(LA_backup.elem);
    free(LC.elem);
    
    return 0;
}
