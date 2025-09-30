#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMTYPE    int
#define BOOL        int
#define TRUE    1
#define FALSE   0

typedef struct LNode {
    ELEMTYPE     data;
    struct LNode *next;
} LNode, *PLNode;

// ����һ��ָ��NULL��ָ��
// ���ڴ���һ������
PLNode CreateListFromTail(PLNode pl) {
    PLNode temp = NULL, new_pl = NULL;
    char buffer[256];
    
    // Ϊָ������ڴ�,��ʼ���ڵ�����
    pl = (PLNode)malloc(sizeof(LNode));
    pl->next = NULL;
    temp = pl;

    scanf_s("%s", buffer, sizeof(buffer));
    ELEMTYPE c = strtol(buffer, NULL, 10);
    while(c != -1) {
        new_pl = (PLNode)malloc(sizeof(LNode)); 
        new_pl->data = c;
        new_pl->next = NULL;

        temp->next = new_pl;
        temp = new_pl;
        scanf_s("%s", buffer, sizeof(buffer));
        c = strtol(buffer, NULL, 10);
    }

    return pl;
}

// ����һ������ָ��
// ����ɾ�����������һ��
PLNode DeleteListElem(PLNode pl, size_t index) {
    PLNode temp = pl;
    for (size_t i = 1; i < index; temp = temp->next) {
        if (temp->next == NULL) { puts("index �����˷�Χ\n"); }
        i++;
    }
    printf("�㵱ǰ��ɾ����λ����: %d -> ��Ӧ��ֵ��: %d\n", index, temp->next->data);
    temp->next = temp->next->next;

    return pl;
}

// �޸������ж�Ӧλ�õ�ֵ
PLNode ModifyListElem(PLNode pl, size_t index, ELEMTYPE md) {
    PLNode temp = pl;
    for (size_t i = 1; i < index; temp = temp->next) {
        if (temp->next == NULL) { puts("index �����˷�Χ\n"); }
        i++;
    }
    printf("�㵱ǰ��ɾ����λ����: %d -> ��Ӧ��ֵ��: %d -> �޸�֮���ֵΪ: %d\n", index, temp->next->data, md);
    temp->next->data = md;

    return pl;
}

// ��ӡ���������е�Ԫ��ֵ
BOOL PrintInfor(PLNode pl) {
    PLNode temp = pl->next;
    int count = 1;
    for (; temp->next != NULL; temp = temp->next) {
        printf("List %-5d -> value is: %d\n", count, temp->data);
        count++; 
    }

    return TRUE;
}

int main() {
    puts("��������ֵ�Դ������");
    puts("��-1��������");
    PLNode pl;
    pl = CreateListFromTail(pl); 
    PrintInfor(pl);

    DeleteListElem(pl, 3);
    PrintInfor(pl);

    ModifyListElem(pl, 2, 1145114);
    PrintInfor(pl);
    return 0;
}