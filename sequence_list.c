#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define LIST_INIT_SIZE 100
#define LISTINCREMENT  10
#define ELEMTYPE       char

typedef struct sequence_list
{
    ELEMTYPE    *elem;
    int         length;
    int         listsize;
} SeqList, *PSeqList;

/* *
 * ���ڳ�ʼ���ṩ�����Ա�
 * ����: 
 *      1. PSeqList sq Ŀ��˳���ĵ�ַ
 * ����ֵ:
 *      �ṩ��˳���ĵ�ַ
 */
PSeqList InitList(PSeqList sq) {
    /* ��ʼ��һ���ڴ�ռ� */
    sq->elem = (ELEMTYPE*)malloc(LIST_INIT_SIZE * (sizeof(ELEMTYPE))); 
    if (sq->elem == NULL) { return NULL; }

    /* ���ýṹ����� */
    sq->length = 0;
    sq->listsize = LIST_INIT_SIZE;
    return sq;
}

/* ���ڴ�ӡ���Ա��е�Ԫ������ */
void PrintSqList(PSeqList sq) {
    for (size_t i = 1; i <= sq->length; i++) {
        printf("���� %-5d -> ��Ӧ��ֵΪ%d\n", i, sq->elem[i - 1]);
    }
}

/**
 * ������˳����в���Ԫ������
 * ����:
 *      1. PSeqList sq      Ҫ������˳������
 *      2. int index        �����λ��
 *      3. ELEMTYPE elem    ����Ԫ�ص�ֵ
 * ����ֵ:
 *      ��������ĵ�ַ
 */
void InsertSqList(PSeqList sq, int index, ELEMTYPE elem) {
    /* У��index */
    if ((index < 1) || (index > sq->length + 1)) { 
        printf("Invalid index", stderr);
        return;
    } 

    /* ����ڴ治��,�����·��� */
    if (sq->length >= sq->listsize) {
        sq->elem = (ELEMTYPE*)realloc(sq->elem, (sq->listsize + LISTINCREMENT) * sizeof(ELEMTYPE));
        sq->listsize += LISTINCREMENT;
    }

    /* ������ݲ��� */
    for (ELEMTYPE *end = &sq->elem[sq->length]; end >= &sq->elem[index]; end--) {
        *end = *(end - 1);
    }
    sq->elem[index - 1] = elem;
    sq->length++;
}

/**
 * ����ɾ��˳�����ָ��λ�õ�Ԫ��
 * ����:
 *      1. PSeqList sq  ��������
 *      2. int index    ����
 * ����ֵ:
 *      �������� 
 */
void DeleteSqList(PSeqList sq, int index) {
    // У������
    if ((index < 1) || (index > sq->length)) {
        printf("Invalid index", stderr);
    }
    
    for (size_t i = index; i < sq->length; i++) {
        sq->elem[index - 1] = sq->elem[index];
    }
    sq->length--;
}

int main(void) {
    PSeqList PSq_0; 
    InitList(PSq_0); 
    for (size_t i = 1; i < 22; i++) {
        InsertSqList(PSq_0, i, i);
    }
    InsertSqList(PSq_0, 8, 124);
    PrintSqList(PSq_0);

    return 0;
}

