#include <iostream>

namespace namespace_test {
    class animal {
        public:
            std::string name;
            void walk();
    };

    int main(animal *a);
};


// definition
namespace namespace_test {
    void animal::walk() {
        std::cout << "walking" << std::endl;
    }

    int main(animal *a) {
        std::cout << a->name << ": ";
        a->walk();
        return 0;
    }
}

void test_namespace()
{
    std::cout << "--- namespace ---" << std::endl;
    namespace_test::animal *a = new namespace_test::animal;
    a->name = "duck";

    // 在作用域（此处即test_namespace内部）内引入命名空间的符号
    using namespace_test::main;
    main(a);

    delete a;
}