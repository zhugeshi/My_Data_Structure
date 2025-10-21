#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ELEMTYPE    int
#define BOOL        char
#define TRUE    1
#define FALSE   0

typedef struct QNode {
    ELEMTYPE data;
    struct QNode* next;    
}QNode, *QNodePtr;

typedef struct {
    QNodePtr front;
    QNodePtr rear;
}QStart, *PQStart;

// 用于队列初始化
BOOL InitQueue (PQStart q_start) {
    q_start->front = (QNodePtr)malloc(sizeof(QNode));

    if (q_start->front == NULL) { perror("Init failed!\n"); return FALSE; } 
    q_start->front->next = NULL;

    q_start->rear = q_start->front;
    return TRUE;
}

// 队列入队
BOOL EnterQueue (PQStart q_start, ELEMTYPE num) {
    QNodePtr new_node = (QNodePtr)malloc(sizeof(QNode));

    if (new_node == NULL) { perror("Init failed!\n"); return FALSE; }

    new_node->data = num;
    new_node->next = NULL;

    q_start->rear->next = new_node;
    q_start->rear = new_node;
    return TRUE;
}

ELEMTYPE OutQueue (PQStart q_start) {
    // 判断是否为空队列
    if (q_start->front == q_start->rear) {
        perror("Queue is empty.\n");
    }

    ELEMTYPE ret_value;
    QNodePtr tmp;
    tmp = q_start->front->next;

    // 如果只剩最后一个元素
    if (q_start->rear == tmp) {
        ret_value = tmp->data;
        q_start->rear = q_start->front;
        q_start->front->next = NULL;
        free(tmp);
        return ret_value;
    }

    q_start->front->next = tmp->next;
    ret_value = tmp->data;
    free(tmp);
    return ret_value;
}

// 输出队列中的所有元素
void PrintValue (PQStart q_st) {
    QNodePtr tmp;
    if ((tmp = q_st->front->next) == NULL) {
        printf("Queue is empty.\n");
        return;
    }

    int num = 0;
    do {
        printf("elem %-3d -> %d\n", num, tmp->data);
        tmp = tmp->next;
        num++;
    } while (tmp != NULL);
}

int main(void) {
    PQStart queue_st;
    InitQueue(queue_st);

    for (int i = 0; i < 20; i++) {
        EnterQueue(queue_st, i);
    }
    PrintValue(queue_st);

    puts("");
    for (int i = 0; i < 20; i++) {
        ELEMTYPE num = OutQueue(queue_st);
        printf("Out value is: %d\n", num);
    }

    puts("");
    PrintValue(queue_st);

    return 0;
}