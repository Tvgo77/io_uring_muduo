#include <memory>
#include <vector>

int main() {
    std::vector<std::shared_ptr<int>> vec;
    int x = 100;
    int * normalPtr = new int(99);
    std::shared_ptr<int> ptr(&x);
    std::shared_ptr<int> ptr2(ptr.get());
    std::shared_ptr<int> ptr3(normalPtr);
    ptr3.reset();
    vec.push_back(ptr);
    vec.push_back(ptr);
    vec.erase(vec.begin()+1);
    return 0;
}