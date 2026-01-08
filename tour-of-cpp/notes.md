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
