#include <stdio.h>
#include <stdlib.h>

#define BOOL       int
#define TRUE       1
#define FALSE      0
#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef int Status;  //声明函数类型名
typedef int ELEMTYPE;

typedef struct LNode {
    ELEMTYPE data;
    struct LNode* next;
} LNode, *PLNode;

// 初始化链表：传入一个 PLNode*（指向 PLNode 变量的指针），函数会在内部分配头结点并通过 *pl 返回
// 返回值：OK(1) 表示成功，ERROR(0) 表示失败
Status InitList(PLNode *pl) {
    if (pl == NULL) return ERROR;
    *pl = (PLNode)malloc(sizeof(LNode));
    if (*pl == NULL) {
        fprintf(stderr, "Init head node failed.\n");
        return ERROR;
    }

    (*pl)->next = NULL;
    (*pl)->data = 0;

    return OK;
}

// 清空单链表。将L中所有数据结点空间释放。只剩下一个头结点，和初始化操作后的结果一样 
Status ClearList(PLNode pl) {
    if (pl == NULL) return ERROR;
    PLNode tmp = pl->next;
    while (tmp != NULL) {
        PLNode to_free = tmp;
        tmp = tmp->next;
        free(to_free);
    }
    pl->next = NULL;
    return OK;
}

// 求单链表的长度，即返回L中数据结点个数 
int ListLength(LNode ln) {
    if (ln.next == NULL) {
        return 0;
    }

    PLNode tmp = ln.next;
    int count = 0;

    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }

    return count;
}

// 用e返回L中第i个数据结点的值，其中1≤i≤Listlength(L)
Status GetElem(LNode ln, size_t idx, ELEMTYPE* elem) {
    // 检查输入参数是否合法
    size_t length = ListLength(ln);
    if (idx < 1 || idx > length) {
        fprintf(stderr, "Index is invalid.\n");
        return ERROR;
    }

    PLNode tmp = ln.next;
    for (size_t i = 1; i < idx; tmp = tmp->next, i++);
    *elem = tmp->data;

    return OK;
}

// 回L中第1个与e相等的数据结点的地址（指针），若e不存在，则返回NULL
PLNode LocateList(LNode ln, ELEMTYPE elem) {
    // 检查输入参数
    if (ln.next == NULL) {
        return NULL;
    }

    PLNode tmp = ln.next ;
    while (tmp != NULL) {
        if (tmp->data == elem) {
            return tmp;
        }
        tmp = tmp->next;
    }

    fprintf(stderr, "Can't find the value.\n");
    return NULL;
}

// 在L中的第i个数据结点之前插入新的数据结点e，1≤i≤Listlength(L)+1
Status ListInsert(PLNode pl, size_t idx, ELEMTYPE elem) {
    // 参数检查
    size_t length = ListLength(*pl);
    if (idx < 1 || idx > length + 1) {
        fprintf(stderr, "Index is invalid.\n");
        return ERROR;
    }

    PLNode tmp = pl;
    for (size_t i = 1; i < idx; tmp = tmp->next, i++);

    PLNode new_node = (PLNode)malloc(sizeof(LNode));

    new_node->data = elem;
    new_node->next = tmp->next;
    tmp->next = new_node;

    return OK;
}

// 删除L中的第i个数据j结点，用e返回删除结点的值，1≤i≤Listlength(L)
Status ListDelete(PLNode pl, size_t  idx, ELEMTYPE* elem) {
    // 参数检查
    size_t length = ListLength(*pl);
    if (idx < 1 || idx > length) {
        fprintf(stderr, "Index is invalid.\n");
        return ERROR;
    }

    PLNode tmp = pl, tmp_free;
    for (size_t i = 1; i < idx; tmp = tmp->next, i++);
    tmp_free = tmp->next;

    *elem = tmp->next->data;
    tmp->next = tmp->next->next;

    free(tmp_free);
    return OK;
}

// 利用printf函数依次输出L的每个数据结点的值
Status ListTraverse(LNode ln) {
    if (ln.next == NULL) {
        fprintf(stderr, "List is empty.\n");
        return ERROR;
    }

    PLNode tmp = ln.next;
    while (tmp != NULL) {
        printf("value is %d\n", tmp->data);
        tmp = tmp->next;
    }

    return OK;
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

// 在尾部追加一个元素（不使用 ListInsert），返回 OK/ERROR
Status AppendTail(PLNode head, ELEMTYPE val) {
    if (head == NULL) return ERROR;
    PLNode p = head;
    while (p->next != NULL) p = p->next;
    PLNode node = (PLNode)malloc(sizeof(LNode));
    if (!node) return ERROR;
    node->data = val;
    node->next = NULL;
    p->next = node;
    return OK;
}

// 从数组创建链表（返回头结点指针）
PLNode CreateListFromArray(const ELEMTYPE arr[], size_t n) {
    PLNode head = NULL;
    if (InitList(&head) != OK) return NULL;
    for (size_t i = 0; i < n; ++i) {
        AppendTail(head, arr[i]);
    }
    return head;
}

// 释放链表所有节点（包含头结点）
void FreeList(PLNode head) {
    if (head == NULL) return;
    PLNode cur = head;
    while (cur != NULL) {
        PLNode tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

// 打印链表为 2->8->...->^ 格式
void PrintListFormat(PLNode head) {
    if (head == NULL) { printf("^\n"); return; }
    PLNode p = head->next;
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) printf("->");
        p = p->next;
    }
    printf("->^\n");
}

// 判断值是否存在于链表中（从头结点的下一个开始）
int Exists(PLNode head, ELEMTYPE val) {
    if (head == NULL) return 0;
    PLNode p = head->next;
    while (p != NULL) {
        if (p->data == val) return OK;
        p = p->next;
    }
    return 0;
}

// 复制链表（深拷贝），返回新头结点
PLNode CopyList(PLNode src) {
    if (src == NULL) return NULL;
    PLNode dest = NULL;
    if (InitList(&dest) != OK) return NULL;
    PLNode p = src->next;
    while (p != NULL) {
        AppendTail(dest, p->data);
        p = p->next;
    }
    return dest;
}

// 并运算 La = La U Lb
void Union(PLNode La, PLNode Lb) {
    if (La == NULL || Lb == NULL) return;
    PLNode idx_p = Lb->next;
    while (idx_p != NULL) {
        if (!Exists(La, idx_p->data)) {
            AppendTail(La, idx_p->data);
        }
        idx_p = idx_p->next;
    }
}

// 交运算 La = La ∩ Lb
// 快慢指针删除法
void Intersection(PLNode La, PLNode Lb) {
    if (La == NULL || Lb == NULL) return;
    PLNode prev = La;
    PLNode cur = La->next;
    while (cur != NULL) {
        if (!Exists(Lb, cur->data)) {
            // 删除 cur
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

// 差运算 La = La - Lb
void Difference(PLNode La, PLNode Lb) {
    if (La == NULL || Lb == NULL) return;
    PLNode prev = La;
    PLNode cur = La->next;
    while (cur != NULL) {
        if (Exists(Lb, cur->data)) {
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

// MergeList：合并两个非递减有序表 La, Lb -> Lc（新分配链表）
int MergeList(PLNode La, PLNode Lb, PLNode *Lc) {
    if (La == NULL || Lb == NULL || Lc == NULL) return ERROR;
    if (InitList(Lc) != OK) return ERROR;
    PLNode pa = La->next;
    PLNode pb = Lb->next;
    while (pa != NULL && pb != NULL) {
        if (pa->data <= pb->data) {
            AppendTail(*Lc, pa->data);
            pa = pa->next;
        } else {
            AppendTail(*Lc, pb->data);
            pb = pb->next;
        }
    }
    while (pa != NULL) { AppendTail(*Lc, pa->data); pa = pa->next; }
    while (pb != NULL) { AppendTail(*Lc, pb->data); pb = pb->next; }
    return OK;
}

// Purge：去掉链表中重复多余元素（保留第一次出现）
void Purge(PLNode Lc) {
    if (Lc == NULL) return;
    PLNode p = Lc->next;
    while (p != NULL) {
        PLNode prev = p;
        PLNode q = p->next;
        while (q != NULL) {
            if (q->data == p->data) {
                // 删除 q
                prev->next = q->next;
                free(q);
                q = prev->next;
            } else {
                prev = q;
                q = q->next;
            }
        }
        p = p->next;
    }
}

int main() {
    ELEMTYPE la_vals[] = {2, 8, 27, 39, 66, 77, 89};
    size_t na = sizeof(la_vals) / sizeof(la_vals[0]);
    ELEMTYPE lb_vals[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};
    size_t nb = sizeof(lb_vals) / sizeof(lb_vals[0]);

    PLNode LA = CreateListFromArray(la_vals, na);
    PLNode LB = CreateListFromArray(lb_vals, nb);
    if (LA == NULL || LB == NULL) {
        fprintf(stderr, "Failed to create lists.\n");
        FreeList(LA); FreeList(LB);
        return 1;
    }

    printf("Initial LA: "); PrintListFormat(LA);
    printf("Initial LB: "); PrintListFormat(LB);

    /* 并运算 La = La U Lb */
    PLNode tmp = CopyList(LA);
    Union(tmp, LB);
    printf("After Union (LA U LB): "); PrintListFormat(tmp);
    FreeList(tmp);

    /* 交运算 La = La ∩ Lb */
    tmp = CopyList(LA);
    Intersection(tmp, LB);
    printf("After Intersection (LA ∩ LB): "); PrintListFormat(tmp);
    FreeList(tmp);

    /* 差运算 La = La - Lb */
    tmp = CopyList(LA);
    Difference(tmp, LB);
    printf("After Difference (LA - LB): "); PrintListFormat(tmp);
    FreeList(tmp);

    /* 有序表合并 Lc = La + Lb */
    PLNode LC = NULL;
    if (MergeList(LA, LB, &LC) != OK) {
        fprintf(stderr, "MergeList failed.\n");
        FreeList(LA); FreeList(LB);
        return 1;
    }
    printf("After Merge (LC): "); PrintListFormat(LC);

    /* 去掉重复多余的元素 */
    Purge(LC);
    printf("After Purge (LC): "); PrintListFormat(LC);

    /* 释放内存 */
    FreeList(LA);
    FreeList(LB);
    FreeList(LC);

    return 0;
}