在一个类方法中定义一个**算法的骨架**，将其中一些步骤（以虚函数）延迟到子类中。模板方法模式可以让子类复用算法结构，并且重新定义其中某些步骤的实现。常用在一些程序的框架库中。

其实就是使用 *late-binding* （虚函数继承）的普通用法。在程序有一段**不变**的算法流程时有用。

![](images\template.png)

**AbstractClass**， 定义了算法框架和部分步骤实现，留了一些步骤给子类继承来Overwrite。

```java
abstract class Framework {
    void step1() {};
    boolean step3() {
        return true;
    };
    // 待子类填写
    abstract void step2();
    abstract void step4();
    void step5() {};
	// 定义了算法框架
    void run() {
        step1();
        step2();
        if (step3()) {
            step4();
        }
        step5();
    }
}
```

**ConcreteClass** 重写了空缺的步骤，补完了框架。

```java
class Application extends Framework {
    @Override
    void step2() {
        System.out.println("My own step2");
    }

    @Override
    void step4() {
        System.out.println("My own step4");
    }
}
```

**使用**

```java
public class Main {
    public static void main(String[] args) {
        Application app = new Application();
        app.run();
    }
}
```

