#include <iostream>
#include <stdexcept>

class my_exception : public std::exception {
private:
  std::string text;

public:
  my_exception(int code);
  const char *what() const noexcept;
};

my_exception::my_exception(int code)
{
    text = "error code " + std::to_string(code);
}

const char * my_exception::what() const noexcept
{
    return text.c_str();
}

static void make_exception()
{
    throw my_exception(404);
}

void test_exception()
{
    std::cout << "--- exception ---" << std::endl;
    // 异常错误会从上到下匹配，直到被捕获
    try {
        make_exception();
    } catch (const std::out_of_range &ex) {
        std::cout << "out-of range" << std::endl;
    } catch (const my_exception &ex) {
        std::cout << "my_exception: " << ex.what() << std::endl;
        // throw; // 处理不了重新抛出，给上层处理
    } catch (...) {
        // 捕获任何异常
        std::cout << "finally" << std::endl;
    }
}