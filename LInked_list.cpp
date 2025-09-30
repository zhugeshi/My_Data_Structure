#include <iostream>
#include <forward_list>
#include <limits>

using ELEMTYPE = int;

// 创建一个链表
std::forward_list<ELEMTYPE> CreateListFromTail() {
    std::forward_list<ELEMTYPE> f1;
    auto tail = f1.before_begin(); // 提供一个假想的节点,指向tail的位置
    std::cout << "请输入数值来创造队列\n" << "用 -1 来结束输入\n";

    ELEMTYPE x;
    while(true) {
        if (!(std::cin >> x)) { // 处理非整数输入
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入非法,已经忽略.请继续输入整数或者 -1 结束.\n";
            continue;
        }
        if (x == -1) break;
        tail = f1.insert_after(tail, x);
    }
    return f1;
}

// 打印链表里的所有元素
bool PrintInfo(const std::forward_list<ELEMTYPE> fl) {
    int count = 1;
    for (auto it = fl.begin(); it != fl.end(); ++it, ++count) {
        std::cout << "List " << count << "      -> value is: " << *it << "\n";
    }
    return true;
}

// 删除第 index (1-based) 个元素
bool DeleteListElem(std::forward_list<ELEMTYPE>& fl, std::size_t index) {
    if (index < 1) {
        std::cout << "index 超出了范围\n";
        return false;
    }

    auto prev = fl.before_begin();
    auto curr = fl.begin();

    for (std::size_t i = 1; i < index && curr != fl.end(); ++i) {
        prev = curr;
        curr++;
    }

    if (curr == fl.end()) {
        std::cout << "index 超出了范围\n";
        return false;
    }

    std::cout << "你当前的位置是" << index << " -> 想删除的对应的值是: " << *curr << '\n';
    fl.erase_after(prev);
    return true;
}

// 修改指定位置的元素值
bool ModifyListElem(std::forward_list<ELEMTYPE>& fl, std::size_t index, ELEMTYPE md) {
    if (index < 1) {
        std::cout << "index 超出了范围\n";
        return false;
    }

    auto it = fl.begin();
    std::size_t i = 1;
    while (it != fl.end() && i < index) {
        it++;
        i++;
    }

    if (it == fl.end()) {
        std::cout << "index 超出了范围\n";
        return false;
    }

    std::cout   << "你当前想删除的位置是: " << index
                << " -> 对应的值是: " << *it
                << " -> 修改之后的值是: " << md << '\n';
    *it = md;
    return true;
}

int main() {
    auto fl = CreateListFromTail();
    PrintInfo(fl);

    DeleteListElem(fl, 3);
    PrintInfo(fl);

    ModifyListElem(fl, 6, 1145114);
    PrintInfo(fl);

    return 0;
}
