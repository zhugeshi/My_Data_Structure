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

// 传入一个指向NULL的指针
// 用于创造一个链表
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

// 传入一个链表指针
// 用于删除链表里面的一项
PLNode DeleteListElem(PLNode pl, size_t index) {
    PLNode temp = pl;
    for (size_t i = 1; i < index; temp = temp->next) {
        if (temp->next == NULL) { puts("index 超出了范围\n"); }
        i++;
    }
    printf("你当前想删除的位置是: %d -> 对应的值是: %d\n", index, temp->next->data);
    temp->next = temp->next->next;

    return pl;
}

// 修改链表中对应位置的值
PLNode ModifyListElem(PLNode pl, size_t index, ELEMTYPE md) {
    PLNode temp = pl;
    for (size_t i = 1; i < index; temp = temp->next) {
        if (temp->next == NULL) { puts("index 超出了范围\n"); }
        i++;
    }
    printf("你当前想删除的位置是: %d -> 对应的值是: %d -> 修改之后的值为: %d\n", index, temp->next->data, md);
    temp->next->data = md;

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
    puts("请输入数值以创造队列");
    puts("用-1结束输入");
    PLNode pl;
    pl = CreateListFromTail(pl); 
    PrintInfor(pl);

    DeleteListElem(pl, 3);
    PrintInfor(pl);

    ModifyListElem(pl, 2, 1145114);
    PrintInfor(pl);
    return 0;
}