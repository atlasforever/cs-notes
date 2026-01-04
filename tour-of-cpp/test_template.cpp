#include <iostream>
#include <string>
#include <vector>

// 模板的函数和类型在实例化时才会生成，因此通常直接定义在 .h 中，而非像普通的类声明在 .h，定义在 .cpp

// 参数化类型 T，以及直接替代的值参数 N
template<typename T, int N = 2>
class my_template_type {
public:
    my_template_type(const T &v) {
        for (int i = 0; i < N; i++) {
            ts[i] = v;
        }
    }

    int size() const {
        return N;
    }
private:
    T ts[N];
};

// 函数模板，也可用于成员函数
template<typename Seq, typename V>
V sum(Seq &s, V v)
{
    for (auto &o : s) {
        v += o;
    }
    return v;
}

void test_template()
{
    std::cout << "--- template ---" << std::endl;

    my_template_type<std::string, 10> test1(std::string("test"));
    std::cout << "test1 size is " << test1.size() << std::endl;

    // 模板参数推导出 <int,2>（N使用默认定义值2）
    my_template_type test2(233);
    std::cout << "test2 size is " << test2.size() << std::endl;

    std::vector<double> values {1.1, 2.2, 3.3};
    std::cout << "sum " << sum(values, 0.0);
}