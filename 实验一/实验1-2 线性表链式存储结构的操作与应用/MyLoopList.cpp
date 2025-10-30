#include <iostream>
#include <utility>
#include <cstddef>

template <typename T>
class CircularList {
private:
    // 类内结构体
    struct Node {
        T data;
        Node* next;

        // 结构体构造函数
        explicit Node(const T& v): data(v), next(nullptr) {}
    };
    
    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t size = 0;

public:
    CircularList() = default;
    ~CircularList() { clear(); }

    // 禁用拷贝
    CircularList(const CircularList&) = delete;
    CircularList& operator=(const CircularList&) = delete;

    // 允许移动
    CircularList(CircularList&& other) noexcept: head(other.head), tail(other.tail), size(other.size) {
        other.head = other.tail = nullptr;
        other.size = 0;
    }

    CircularList& operator=(CircularList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = other.tail = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // 检查函数
    bool empty() const { return size == 0; }
    std::size_t check_size() const { return size; }

    // 左值添加节点
    void push_back(const T& value) {
        Node* n = new Node(value);
        link_back(n);
    }

    // 右值添加节点
    void push_back(T&& value) {
        Node* n = new Node(std::move(value));
        link_back(n);
    }

    // 打印整个循环列表
    void print_list(std::size_t limit = 20) const {
        // 检查
        if (!head) {
            std::cout << "(empty)\n";
            return;
        }             

        const Node* cur = head;
        std::size_t count = 0;
        do {
            std::cout << cur->data << " -> ";
            cur = cur->next;
            if (++count > limit) {
                std::cout << "...(loop)\n";
                return;
            }  
       } while (cur != head);
       std::cout << "(back to head)\n";
    }

    // 清空链表
    void clear() noexcept {
        if (!head) return;
        tail->next = nullptr;
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        size = 0;
    }
    
private:
    /** 
     * 尾插法工具函数
    */
    void link_back(Node* n) noexcept {
        // 如果没有头节点
        if (!head) {
            head = tail = n;
            n->next = n; // 循环指向自己
        } else {
            n->next = head;
            tail->next = n;
            tail = n;
        }
        size++;
    }
};

int main() {
    CircularList<int> list;
    for (std::size_t i = 0; i < 15; i++) {
        list.push_back(i);
    }
    list.print_list();

    return 0;
}

/**
 * 发现循环链表用智能指针实现还是太麻烦了,下次涉及到循环的数据结构还是用c来实现好了
 * 虽然用modern cpp来写还是学到了不少东西的...
 */