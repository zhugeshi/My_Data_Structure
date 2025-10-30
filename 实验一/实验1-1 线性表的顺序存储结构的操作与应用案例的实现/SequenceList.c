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
 * ����new��������洢�ռ䣬����һ���յ�˳����L��L�ĳ�ʼ��СΪMAXSIZE
 * �ɹ�����OK
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
 * ��˳�����������.
 * ��L.length���㣬��L.length=0����
 */
STATUS ClearList(PSqList sq) {
    sq->length = 0;
    return OK;
}

/**
 * ����L������Ԫ�ظ���
 */
int ListLength(PSqList sq) {
    return sq->length;
}

/**
 * ��e����L�е�i������Ԫ�ص�ֵ��1��i��L.Length
 */
STATUS GetElem(PSqList sq, int idx, ELEMTYPE* elem) {
    // ���������Ƿ�Ϸ�
    if (idx < 1 || idx > sq->length) {
        fprintf(stderr, "Index is invalid.\n");
        return FALSE;
    }

    *elem = sq->elem[idx - 1];
    return OK;
}

/**
 * ����L�е�1����e��ȵ�����Ԫ��λ����e�����ڣ��򷵻�0
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

/* ���ڴ�ӡ���Ա��е�Ԫ������ */
void PrintSqList(PSqList sq) {
    for (size_t i = 1; i <= sq->length; i++) {
        printf("���� %-5d -> ��Ӧ��ֵΪ%d\n", (int)i, sq->elem[i - 1]);
    }
}

/**
 * ��L�еĵ�i��λ��֮ǰ�����µ�����Ԫ��e
 */
STATUS ListInsert(PSqList sq, int idx, ELEMTYPE elem) {
    /* У��index */
    if ((idx < 1) || (idx > sq->length + 1)) { // �������ĩβ����,idx�����ǳ��ȼ�1
        fprintf(stderr, "Invalid index.\n");
        return FALSE;
    }

    /* ����С�Ƿ���� */
    if (ListLength(sq) >= MAXSIZE) {
        fprintf(stderr, "SqList size is too huge.\n");
        return FALSE;
    }

    /* ������ݲ��� */
    for (ELEMTYPE* end = &sq->elem[sq->length]; end >= &sq->elem[idx]; end--) {
        *end = *(end - 1);
    }
    sq->elem[idx - 1] = elem;
    sq->length++;
    return OK;
}

/**
 * ɾ��L�еĵ�i������Ԫ�أ���elem������ֵ��1��i��L.length
 */
STATUS ListDelete(PSqList sq, int idx, ELEMTYPE* elem) {
    // У������
    if ((idx < 1) || (idx > sq->length)) {
        fprintf(stderr, "Invalid index\n");
        return FALSE;
    }

    *elem = sq->elem[idx - 1];

    // ������Ԫ��ǰ��
    for (int i = idx; i < sq->length; i++) {
        sq->elem[i - 1] = sq->elem[i];
    }
    sq->length--;

    return OK;
}

/**
 * ����printf�����������L��ÿ������Ԫ�ص�ֵ
 */
void ListTraverse(PSqList sq) {
    int sq_len = ListLength(sq);
    printf("List len is %d.\n", sq_len);
    for (int i = 0; i < sq_len; i++) {
        printf("List %-2d value is -> %5d\n", i + 1, sq->elem[i]);
    }
}

/**
 * ������ La=La U Lb
 * ��������Ԫ��,�����ظ�Ԫ��,�൱�ڽ��������Ա�ȫ���ϲ�
 */
STATUS Union(PSqList target_sq, SqList other_sq) {
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }

    // ���other_sq�Ƿ�Ϊ�ջ�δ��ʼ��
    if (other_sq.elem == NULL || other_sq.length == 0) {
        return OK;  // ���other_sqΪ�գ�ֱ�ӷ���
    }

    // ������������other_sq�е�Ԫ�أ������ظ�Ԫ�أ�
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
 * ������ La=La �� Lb
 * �޸�target_sq��ʹ��ֻ������other_sq��ͬ��Ԫ��
 */
STATUS InterSection(PSqList target_sq, SqList other_sq) {
    // ��������Ƿ�Ϸ�
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }
    
    // ���other_sq�Ƿ�Ϊ�ջ�δ��ʼ��
    if (other_sq.elem == NULL || other_sq.length == 0) {
        ClearList(target_sq);  // ���other_sqΪ�գ�����Ϊ��
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

    // Ѱ����ͬ�Ĳ��֣���ȷ�����ظ�Ԫ��
    for (int i = 0; i < other_sq.length; i++) {
        ELEMTYPE tmp_elem = other_sq.elem[i];

        // ����ʱ����Ѱ���Ƿ��Ѿ�����Ԫ��
        int already_in_temp = LocateList(&temp_sq, tmp_elem);
        if (already_in_temp != 0) {
            continue;  
        }
        
        // ��Ŀ������Ա���Ѱ���Ƿ����Ԫ��
        int find_idx = LocateList(target_sq, tmp_elem);
        if (find_idx != 0) {
            // �ҵ���ͬ��Ԫ�أ���ӵ���ʱ����
            if (ListInsert(&temp_sq, temp_sq.length + 1, tmp_elem) != OK) {
                fprintf(stderr, "Failed to insert element to temp list.\n");
                free(temp_sq.elem);
                return ERROR;
            }
        }
    }

    // ���ԭ��������������ƻ�ȥ
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
 * ������ La=La - Lb
 * ������target_sq�е�����other_list�е�Ԫ��
 */
STATUS Difference(PSqList target_sq, SqList other_list) {
    // ����������
    if (target_sq == NULL) {
        fprintf(stderr, "Target sq is none.\n");
        return ERROR;
    }
    if (other_list.elem == NULL || other_list.length == 0) {
        return OK;  // ���other_listΪ�գ�ֱ�ӷ���
    }

    // ����һ����ʱ�����洢��ʱ���
    SqList temp_sq;
    if (InitList(&temp_sq) != OK) {
        fprintf(stderr, "Failed to initialize temp list.\n");
        return ERROR;
    }

    // ����target_sq�е�ÿ��Ԫ��
    for (int i = 0; i < target_sq->length; i++) {
        ELEMTYPE tmp_elem = target_sq->elem[i];
        
        // ����Ԫ���Ƿ���other_list��
        int find_idx = LocateList(&other_list, tmp_elem);
        if (find_idx == 0) {
            // �������other_list�У���ӵ���ʱ��
            if (ListInsert(&temp_sq, temp_sq.length + 1, tmp_elem) != OK) {
                fprintf(stderr, "Failed to insert element to temp list.\n");
                free(temp_sq.elem);
                return ERROR;
            }
        }
    }

    // ���ԭ�����������ƻ�ȥ
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
 * ���������ĺϲ�
 * ��La��Lb�е�Ԫ���Ƿǵݼ�����ģ��ϲ���LcҲ�Ƿǵݼ������
 * ʹ�ù鲢�㷨�ϲ����������
 */
STATUS MergeList(SqList fir_sq, SqList sec_sq, SqList *out_sq) {
    // ����������
    if (out_sq == NULL) {
        fprintf(stderr, "Output list Lc is NULL.\n");
        return ERROR;
    }
    if (fir_sq.elem == NULL || sec_sq.elem == NULL) {
        fprintf(stderr, "Input lists are not initialized.\n");
        return ERROR;
    }

    // ��ʼ��out
    if (InitList(out_sq) != OK) {
        fprintf(stderr, "Failed to initialize output list Lc.\n");
        return ERROR;
    }

    // ˫ָ����뷨
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
 * ȥ��˳����е��ظ�����Ԫ��
 * ��Lc��һ�����ظ�Ԫ�ص�˳���ȥ���ظ�Ԫ��
 * ʹ��˫ָ�뷨����ԭ��ȥ��
 */
STATUS Purge(SqList *sq) {
    // ����������
    if (sq == NULL || sq->elem == NULL) {
        fprintf(stderr, "Input list is NULL or not initialized.\n");
        return ERROR;
    }

    // �����Ϊ�ջ�ֻ��һ��Ԫ�أ�����Ҫȥ��
    if (sq->length <= 1) {
        return OK;
    }

    // ����ָ��
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
    printf("=== ��һ�� ===\n");
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
    
    printf("��LA: ");
    ListTraverse(pLA);
    printf("��LB: ");
    ListTraverse(pLB);

    printf("\n=== �ڶ��� ===\n");
    
    // ����LA��ԭʼ�������ڻָ�
    SqList LA_backup;
    InitList(&LA_backup);
    for (int i = 0; i < LA.length; i++) {
        ListInsert(&LA_backup, i + 1, LA.elem[i]);
    }

    printf("(1) ������ LA = LA �� LB\n");
    Union(pLA, LB);
    printf("��������: ");
    ListTraverse(pLA);

    // �ָ�LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("\n(2) ������ LA = LA �� LB\n");
    InterSection(pLA, LB);
    printf("��������: ");
    ListTraverse(pLA);

    // �ָ�LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("\n(3) ������ LA = LA - LB\n");
    Difference(pLA, LB);
    printf("��������: ");
    ListTraverse(pLA);

    printf("\n=== ������ ===\n");
    
    // �ָ�LA
    ClearList(pLA);
    for (int i = 0; i < LA_backup.length; i++) {
        ListInsert(pLA, i + 1, LA_backup.elem[i]);
    }

    printf("�����ϲ� MergeList(LA, LB, LC)\n");
    SqList LC;
    if (MergeList(LA, LB, &LC) == OK) {
        printf("�ϲ����LC: ");
        ListTraverse(&LC);
    }

    printf("\n=== ������ ===\n");

    printf("\nȥ������ Purge(LC)\n");
    if (Purge(&LC) == OK) {
        printf("ȥ�غ�LC: ");
        ListTraverse(&LC);
    }

    // �ͷ������ڴ�
    free(LA.elem);
    free(LB.elem);
    free(LA_backup.elem);
    free(LC.elem);
    
    return 0;
}
