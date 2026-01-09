# Tour of C++

## 命名空间

```cpp
// myspace.h，声明
namespace myspace {
    class animal {
        public:
            void walk();
    };
};

// myspace.cpp，定义
namespace myspace {
    void animal::walk() {
        std::cout << "walking" << std::endl;
    }
};

// 调用
myspace::animal dog()；
```

`using`使名称在当前作用域可用

```cpp
void test()
{
    // animal在test()中可用
    using myspace::animal;
    animal dog();
}

// std所有名称
using namespace std;
```

## 异常

所有异常类都派生自`std::exception`

```cpp
// 自定义异常
class my_exception : public std::exception {
private:
    std::string text;
public:
    my_exception(int code);
};

my_exception::my_exception(int code)
{
    text = "error code " + std::to_string(code);
}
```

`throw`抛出异常

```cpp
// 异常错误会从上到下匹配，直到被捕获
try {
    throw my_exception(404);
} catch (const std::out_of_range &ex) {
    std::cout << "out-of range" << std::endl;
} catch (const my_exception &ex) {
    std::cout << "my_exception: " << std::endl;
    throw; // 可以重新抛出
} catch (...) {
    // 捕获任何异常
    std::cout << "any other" << std::endl;
}
```

异常抛出后，返回调用处的路径中的局部变量退出作用域时会自己析构（*栈展开*）

* 构造函数抛出异常会导致构造失败
  * 不会调用析构函数（对象没创建成功）
  * 已构造的局部变量、成员、基类对象会按顺序析构（栈展开）
  * `new`申请的资源会泄露（无法通过析构函数释放）
* **不要在析构函数中抛出异常**
  * 栈展开过程中的析构函数再抛出异常，会导致程序错乱调用`std::terminate()`
  
```cpp
class Demo {
    int* data;
public:
    Demo() {
        data = new int[10];
        throw std::runtime_error("构造失败"); // 这里会导致 data 泄漏！
    }
    
    ~Demo() noexcept(false) { // 强行允许抛出（不推荐）
        // 如果这里抛出异常，且此时正因为另一个异常在进行栈展开，程序直接挂掉
        throw std::runtime_error("析构失败"); 
    }
};
```

## 模板

### 参数化类型

模板接受类型与数值作为参数

模板的代码实例化时才会生成，不能声明与定义分离在 .h 和 .cpp 中

```cpp
// 类模板
template <typename T, int N>
class Array {
public:
    // 数值参数 N 决定数组大小
    T data[N];

    // 类内直接定义，无需分离
    int size() const { return N; }
    T& operator[](int i) { return data[i]; }
};

// 函数模板
template <typename T>
T add(T a, T b) {
    // 结合 lambda
    auto calc = [](T x, T y) {
        return x + y;
    }

    return calc(a, b);
}

void test()
{
    Array<double, 2> array;
    array[0] = 0.4;

    // 自动类型推导
    auto res1 = add(10, 20);
    // 显示指定类型
    auto res2 = add<double>(1.5, 2.3);
}
```

### 特化

为特定类型提供特制的模板实现，根据类型自动选择

**全特化（Full Specialization）** 的函数模板

```cpp
// 1. 通用模板
template <typename T>
bool isEqual(T a, T b) {
    return a == b;
}

// 2. 全特化：针对 const char* 类型（字符串比较不能直接用 ==）
template <>
bool isEqual<const char*>(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}
```

全特化类模板

```cpp
template <typename T>
class Storage {
    T data;
};

// 针对 bool 的特殊实现（比如为了节省空间用位操作）
template <>
class Storage<bool> {
    unsigned char data; // 内部用 byte 存储
};
```

### 别名

通过固定部分或全部参数来定义一个新模板

```cpp
// 为复杂的 map 类型定义别名
template <typename T>
using StringMap = std::map<std::string, T>;

int main() {
    // 相当于 std::map<std::string, int>
    StringMap<int> inventory; 
    inventory["apple"] = 10;
}
```

### 转发

**转发（forward）** 在传值时保留参数的左右值属性和 const 修饰符

没有转发时，传入右值也会被作为左值处理，进行多余的拷贝操作

```cpp
void real_work(int& n)  { /* 处理左值 */ }
void real_work(int&& n) { /* 处理右值 */ }

template<typename T>
void wrapper(T arg) {
    // 无论传进来是什么，arg 到了这里都是左值！
    real_work(arg);
}
```

完美转发保证实参的原样传入

* `T&&` 在模板语境下是**万能引用**，同时接收左右值
* `std::forward<T>` 根据原始参数还原形参的左右值属性

```cpp
void target(int& x)  { std::cout << "左值引用\n"; }
void target(int&& x) { std::cout << "右值引用\n"; }

template <typename T>
void wrapper(T&& arg) { // 1. 使用万能引用
    // 2. 使用 std::forward 恢复属性
    target(std::forward<T>(arg)); 
}

int main() {
    int a = 10;
    wrapper(a);   // 输出：左值引用
    wrapper(20);  // 输出：右值引用
}
```

## 实用功能

### 智能指针

`std::unique_ptr` 独占所有权，不可拷贝，大多数场景的默认选择

```cpp
// 推荐：使用 make_unique (C++14)
auto p1 = std::make_unique<int>(10);

// 所有权转移
auto p2 = std::move(p1); 
if (!p1) std::cout << "p1 is now empty";
```

`std::shared_ptr` 共享所有权，引用计数，其循环引用时会无法释放

```cpp
struct B;
struct A { std::shared_ptr<B> b; ~A(){ puts("A dead"); } };
struct B { std::shared_ptr<A> a; ~B(){ puts("B dead"); } };

void leak() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->b = b;
    b->a = a; 
} // 结束后没有任何输出，内存泄露
```

`std::weak_ptr` 不持有所有权，解决循环引用

```cpp
struct B;
struct A { std::shared_ptr<B> b; ~A(){ puts("A dead"); } };
struct B { std::weak_ptr<A> a;   ~B(){ puts("B dead"); } }; // 改为 weak

void fix() {
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();
    a->b = b;
    b->a = a; 
} // 输出：A dead \n B dead，内存成功释放
```

**智能指针应仅用于表达所有权管理**，不操作生存期的函数应当接受原始指针或引用

```cpp
void update_score(Player& player) {
    player.set_score(100); // 直接修改内部状态
}

// 调用时：
auto u_ptr = std::make_unique<Player>("Alice");
update_score(*u_ptr);  // 解引用，直接传入内部对象的引用
```

### function

`function` 可持有任何对象并通过调用操作符`()`调用的类型

```cpp
int f1(double);
std::function<int(double)> fct1 {f1};

// fct2 的类型是 function<void(Shape*)>
std::function fct2 = [](Shape* p) { p->draw(); };
fct2(); // 调用
```
