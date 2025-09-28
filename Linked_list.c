#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMTYPE int
#define BOOL    int
#define TRUE    1
#define FALSE   0

typedef struct LNode {
    ELEMTYPE     data;
    struct LNode *next;
} LNode, *PLNode;

// 传入一个指向NULL的指针
PLNode CreateListFromTail(PLNode pl) {
    PLNode temp = NULL, new_pl = NULL;
    char buffer[256];
    
    // 为指针分配内存,初始化节点内容
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

// 打印链表里所有的元素值
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
    PLNode pl;
    pl = CreateListFromTail(pl); 
    PrintInfor(pl);

    return 0;
}