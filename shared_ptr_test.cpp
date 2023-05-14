#include <memory>
#include <vector>

int main() {
    std::vector<std::shared_ptr<int>> vec;
    int x = 100;
    std::shared_ptr<int> ptr(&x);
    vec.push_back(ptr);
    vec.push_back(ptr);
    vec.erase(vec.begin()+1);
    return 0;
}