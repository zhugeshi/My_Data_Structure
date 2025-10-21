#include <iostream>
#include <forward_list>
#include <limits>

using ELEMTYPE = int;

// ����һ������
std::forward_list<ELEMTYPE> CreateListFromTail() {
    std::forward_list<ELEMTYPE> f1;
    auto tail = f1.before_begin(); // �ṩһ������Ľڵ�,ָ��tail��λ��
    std::cout << "��������ֵ���������\n" << "�� -1 ����������\n";

    ELEMTYPE x;
    while(true) {
        if (!(std::cin >> x)) { // �������������
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "����Ƿ�,�Ѿ�����.����������������� -1 ����.\n";
            continue;
        }
        if (x == -1) break;
        tail = f1.insert_after(tail, x);
    }
    return f1;
}

// ��ӡ�����������Ԫ��
bool PrintInfo(const std::forward_list<ELEMTYPE> fl) {
    int count = 1;
    for (auto it = fl.begin(); it != fl.end(); ++it, ++count) {
        std::cout << "List " << count << "      -> value is: " << *it << "\n";
    }
    return true;
}

// ɾ���� index (1-based) ��Ԫ��
bool DeleteListElem(std::forward_list<ELEMTYPE>& fl, std::size_t index) {
    if (index < 1) {
        std::cout << "index �����˷�Χ\n";
        return false;
    }

    auto prev = fl.before_begin();
    auto curr = fl.begin();

    for (std::size_t i = 1; i < index && curr != fl.end(); ++i) {
        prev = curr;
        curr++;
    }

    if (curr == fl.end()) {
        std::cout << "index �����˷�Χ\n";
        return false;
    }

    std::cout << "�㵱ǰ��λ����" << index << " -> ��ɾ���Ķ�Ӧ��ֵ��: " << *curr << '\n';
    fl.erase_after(prev);
    return true;
}

// �޸�ָ��λ�õ�Ԫ��ֵ
bool ModifyListElem(std::forward_list<ELEMTYPE>& fl, std::size_t index, ELEMTYPE md) {
    if (index < 1) {
        std::cout << "index �����˷�Χ\n";
        return false;
    }

    auto it = fl.begin();
    std::size_t i = 1;
    while (it != fl.end() && i < index) {
        it++;
        i++;
    }

    if (it == fl.end()) {
        std::cout << "index �����˷�Χ\n";
        return false;
    }

    std::cout   << "�㵱ǰ��ɾ����λ����: " << index
                << " -> ��Ӧ��ֵ��: " << *it
                << " -> �޸�֮���ֵ��: " << md << '\n';
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
