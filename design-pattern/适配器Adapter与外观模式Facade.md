## 适配器模式 Adapter Pattern

将一个类的**一个接口转化为客户希望的另一个接口**。该模式让原本接口不兼容的类可以一起工作。

适配器模式中的角色：

* Target：目标类（一般是新接口）
* Adapter：适配器类
* Adaptee：被适配的类（现有老接口）
* Client：客户类

常用的是**对象适配器**，将`Adaptee`对象组合地存在`Adapter`对象中。

![](images\Adapter.jpg)



**Target** 抽象类和示例的具体类。

```java
// 客户希望使用的接口，要把老接口往这上面适配
interface Duck {
    public void quack();
    public void fly();
}

class GreenDuck implements Duck {
    @Override
    public void quack() {
        System.out.println("Quack");
    }

    @Override
    public void fly() {
        System.out.println("Fly");
    }
}
```

**Adaptee** 抽象类和具体类，需要被适配的老接口类。

```java
// 与 Duck 有不同的接口。Turkey 不会呱呱叫，同时也只能飞一小段距离。
interface Turkey {
    public void globble();
    public void fly();
}

class WildTurkey implements Turkey {
    @Override
    public void globble() {
        System.out.println("Globble");
    }

    @Override
    public void fly() {
        System.out.println("Fly a short distance");
    }
}
```

**Adapter** 适配器类。利用被适配的`Adaptee`具体类已实现的代码，但是接口是新的`Target`的。

```java
class TurkeyAdapter implements Duck {
    private Turkey turkey;
	// 通过组合获取 Duck 的具体类的代码
    public TurkeyAdapter(Turkey turkey) {
        this.turkey = turkey;
    }
    // 直接用火鸡叫充当鸭子叫
    @Override
    public void quack() {
        turkey.globble();
    }
	// 火鸡飞行距离短，多次飞行来达到鸭子飞的效果
    @Override
    public void fly() {
        for (int i = 0; i < 4; i++) {
            turkey.fly();
        }
    }
}
```

**客户端中的使用**

```java
public class Main {
    public static void main(String[] args) {
        Duck turkeyDuck = new TurkeyAdapter(new WildTurkey());
        turkeyDuck.quack();
        turkeyDuck.fly();
    }
}
```



## 外观模式 Facade Pattern

用于为子系统中的一组接口提供一个稳定的界面。外观模式提供了高层接口，使子系统更容易复用。减少子系统与外部模块的耦合。

![](images\facade.png)

