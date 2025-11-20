#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR      0
#define TRUE       1
#define FALSE      0
#define INFEASIBLE -1
#define OVERFLOW   -2
#define MAXSIZE    100

typedef struct {
    char* ch;       // 若是非空串，则按串长分配存储区，否则ch为NULL 
    int length;     // 串长度
} HString;          // 堆分配串长度的类型定义

typedef int Status;

static void InitEmpty(HString* S) {
    if (!S) return;
    S->ch = NULL;
    S->length = 0;
}

static void FreeHString(HString* S) {
    if (!S) return;
    if (S->ch) free(S->ch);
    S->ch = NULL;
    S->length = 0;
}

// （1）将字符串常量 chars 赋给串变量 S
Status StrAssign(HString* S, const char* chars) {
    if (!S) return ERROR; // 如果是NULL指针，返回错误
    FreeHString(S);
    if (!chars || chars[0] == '\0') {   // 空串
        return OK;
    }

    /* 计算长度 */
    int len = 0;
    for (const char* p = chars; *p; ++p) ++len;
    S->ch = (char*)malloc((size_t)len);

    /* 循环复值 */
    if (!S->ch) return ERROR;
    for (int i = 0; i < len; ++i) S->ch[i] = chars[i];
    S->length = len;
    return OK;
}

// （2）比较串 S 与 T 的值：S>T 返回 1；S=T 返回 0；否则返回 -1
int StrCompare(HString S, HString T) {
    /* 取较短串的长度 */
    int shortest_len = S.length < T.length ? S.length : T.length;

    for (int i = 0; i < shortest_len; ++i) {
        if (S.ch[i] != T.ch[i]) return (S.ch[i] > T.ch[i]) ? 1 : -1;
    }
    if (S.length == T.length) return 0;
    return (S.length > T.length) ? 1 : -1;
}

// （3）返回串 S 的长度
int StrLength(HString S) { return S.length; }

// （4）将 T1 与 T2 连接赋给 S
Status Concat(HString* S, HString T1, HString T2) {
    /* 参数检查 */
    if (!S) return ERROR;
    int total = T1.length + T2.length;
    if (total == 0) { FreeHString(S); return OK; }

    char* buf = (char*)malloc((size_t)total);
    if (!buf) return ERROR;

    /* 拼接字符串 */
    for (int i = 0; i < T1.length; ++i) buf[i] = T1.ch ? T1.ch[i] : 0;
    for (int j = 0; j < T2.length; ++j) buf[T1.length + j] = T2.ch ? T2.ch[j] : 0;

    FreeHString(S);
    S->ch = buf;
    S->length = total;
    return OK;
}

// （5）用 sub 返回 S 的第 pos 个字符起、长度为 len 的子串（1-based）
Status SubString(HString* sub, HString S, int pos, int len) {
    /* 参数检查 */
    if (!sub) return ERROR;
    if (pos < 1 || pos > S.length || len < 0 || pos + len - 1 > S.length) return ERROR;
    FreeHString(sub);

    /* 如果长度为0，返回空串 */
    if (len == 0) return OK;

    sub->ch = (char*)malloc((size_t)len);
    if (!sub->ch) return ERROR;

    int start = pos - 1;
    for (int i = 0; i < len; ++i) sub->ch[i] = S.ch[start + i];
    sub->length = len;

    return OK;
}

// （6）遍历（输出）字符串 S 中的各个字符
void StrTraverse(HString S) {
    for (int i = 0; i < S.length; ++i) putchar(S.ch[i]);
    putchar('\n');
}

// 其他操作

//（1）StrCopy：将串 T 赋给 S
Status StrCopy(HString* S, HString T) {
    if (!S) return ERROR;
    if (T.length == 0) { FreeHString(S); return OK; }
    char* buf = (char*)malloc((size_t)T.length);
    if (!buf) return OVERFLOW;
    for (int i = 0; i < T.length; ++i) buf[i] = T.ch[i];
    FreeHString(S);
    S->ch = buf;
    S->length = T.length;
    return OK;
}

//（2）Index：返回 T 在 S 中从 pos 开始首次出现的位置（1-based），否则返回 0
int Index(HString S, HString T, int pos) {
    if (pos < 1 || pos > S.length) return 0;
    if (T.length == 0) return 0; // 题目未定义空串，这里返回 0
    HString tmp_sub; InitEmpty(&tmp_sub);

    /* 逐个寻找 */
    for (int i = pos; i <= S.length - T.length + 1; ++i) {
        if (SubString(&tmp_sub, S, i, T.length) != OK) { FreeHString(&tmp_sub); return 0; }
        int cmp_flag = StrCompare(tmp_sub, T);
        if (cmp_flag == 0) { FreeHString(&tmp_sub); return i; }
    }
    FreeHString(&tmp_sub);
    return 0;
}

//（3）Replace：将 S 中所有子串 T1 替换为 T2
// S:  1111222233334444
// T1: 3333
// T2: 55555
// tmp: 1111222255555
// result: 1111222255555
// result += tmp
// Replace 后 S 变为 11112222555554444
Status Replace(HString* S, HString T1, HString T2) {
    /* 检查参数 */
    if (!S) return ERROR;
    if (T1.length == 0) return OK; // 避免死循环

    HString result; InitEmpty(&result);
    HString piece; InitEmpty(&piece);

    int i = 1;
    while (i <= S->length - T1.length + 1) {
        // 逐个查找
        if (SubString(&piece, *S, i, T1.length) != OK) { FreeHString(&piece); FreeHString(&result); return ERROR; }

        // 如果存在子串T1
        if (StrCompare(piece, T1) == 0) {
            // result = result + T2
            HString tmp; InitEmpty(&tmp);
            if (Concat(&tmp, result, T2) != OK) { FreeHString(&piece); FreeHString(&result); return ERROR; }

            FreeHString(&result);
            result = tmp;
            i += T1.length;
        } else {
            // 追加一个字符 S[i]
            HString one; InitEmpty(&one);
            SubString(&one, *S, i, 1);
            HString tmp; InitEmpty(&tmp);

            if (Concat(&tmp, result, one) != OK) { FreeHString(&one); FreeHString(&piece); FreeHString(&result); return OVERFLOW; }
            FreeHString(&result);

            result = tmp;
            FreeHString(&one);
            i += 1;
        }
    }
    // 追加剩余的字符
    if (i <= S->length) {
        HString tail; InitEmpty(&tail);
        if (SubString(&tail, *S, i, S->length - i + 1) != OK) { FreeHString(&piece); FreeHString(&result); return ERROR; }

        HString tmp; InitEmpty(&tmp);
        if (Concat(&tmp, result, tail) != OK) { FreeHString(&tail); FreeHString(&piece); FreeHString(&result); return ERROR; }

        FreeHString(&result);
        result = tmp;
        FreeHString(&tail);
    }
    FreeHString(&piece);
    // 用 result 覆盖 S
    FreeHString(S);
    *S = result;
    return OK;
}

//（4）StrInsert：在 S 的第 pos 个字符之前插入 T（1 ≤ pos ≤ StrLength(S)+1）
Status StrInsert(HString* string, int pos, HString target_str) {
    /* 参数检查 */
    if (!string) return ERROR;
    if (pos < 1 || pos > string->length + 1) return ERROR;

    HString first_part, second_part, tmp; InitEmpty(&first_part); InitEmpty(&second_part); InitEmpty(&tmp);

    if (pos > 1 && SubString(&first_part, *string, 1, pos - 1) != OK) { FreeHString(&first_part); return ERROR; }

    if (pos <= string->length && SubString(&second_part, *string, pos, string->length - pos + 1) != OK) { FreeHString(&first_part); FreeHString(&second_part); return ERROR; }

    /* 将要第一部分和目标T拼接 */
    if (Concat(&tmp, first_part, target_str) != OK) { FreeHString(&first_part); FreeHString(&second_part); return OVERFLOW; }

    HString res; InitEmpty(&res);

    /* 将第二部分拼接在后面 */
    if (Concat(&res, tmp, second_part) != OK) { FreeHString(&first_part); FreeHString(&second_part); FreeHString(&tmp); return OVERFLOW; }

    FreeHString(&first_part); FreeHString(&second_part); FreeHString(&tmp);
    FreeHString(string);
    *string = res;
    return OK;
}

//（5）StrDelete：删除 S 中从 pos 起连续的 len 个字符
Status StrDelete(HString* string, int pos, int len) {
    /* 参数检查 */
    if (!string) return ERROR;
    if (len < 0 || pos < 1 || pos > string->length - len + 1) return ERROR;

    HString pre, suf, res; InitEmpty(&pre); InitEmpty(&suf); InitEmpty(&res);

    /* 获取前半部分 */
    if (pos > 1) {
        if (SubString(&pre, *string, 1, pos - 1) != OK) { FreeHString(&pre); return ERROR; }
    }

    /* 获取后半部分 */
    if (pos + len <= string->length) {
        if (SubString(&suf, *string, pos + len, string->length - (pos + len) + 1) != OK) { FreeHString(&pre); FreeHString(&suf); return ERROR; }
    }

    /* 拼接前后部分 */
    if (Concat(&res, pre, suf) != OK) { FreeHString(&pre); FreeHString(&suf); return OVERFLOW; }

    FreeHString(&pre); FreeHString(&suf);
    FreeHString(string);

    *string = res;
    return OK;
}

// --------------- 测试主函数 ---------------
int main(void) {
    HString T1, T2, T3, S; InitEmpty(&T1); InitEmpty(&T2); InitEmpty(&T3); InitEmpty(&S);

    // （1）数据准备：赋值并遍历输出
    StrAssign(&T1, "hello");
    StrAssign(&T2, "_WORLD_");
    StrAssign(&T3, "Hi");
    printf("T1: "); StrTraverse(T1);
    printf("T2: "); StrTraverse(T2);
    printf("T3: "); StrTraverse(T3);

    // （2）StrCopy：T1 -> S
    StrCopy(&S, T1);
    printf("S after StrCopy(T1): "); StrTraverse(S);

    // （3）StrInsert：将 T2 插入到 S 的尾部（位置 StrLength(S)+1）
    StrInsert(&S, StrLength(S) + 1, T2);
    printf("S after StrInsert tail T2: "); StrTraverse(S);

    // （4）Replace：将 S 中的 T1 替换为 T3（全部替换）
    Replace(&S, T1, T3);
    printf("S after Replace(T1->T3): "); StrTraverse(S);

    // （5）Index：查找 T2 在 S 中的位置
    int pos = Index(S, T2, 1);
    printf("Index of T2 in S: %d\n", pos);

    // （6）StrDelete：删除 S 中子串 T2（若存在则删除）
    if (pos > 0) StrDelete(&S, pos, StrLength(T2));
    printf("S after StrDelete(T2): "); StrTraverse(S);

    // 释放资源
    FreeHString(&T1); FreeHString(&T2); FreeHString(&T3); FreeHString(&S);
    return 0;
}