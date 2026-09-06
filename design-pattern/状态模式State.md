> 状态模式允许一个对象在其与外部事件互动时改变内部状态，不同状态下系统的行为也有所不同。

不同于传统的以 *枚举常量作状态值 + switch语句* 的方式，面向对象的方法是用 **状态类及其重载方法**。

## 结构

![](images\state.png)

1. **Context** 环境类，就是状态机。保存表示当前状态的 State 对象，并将处理委托给它。Context 有一个 setter 方法用于改变当前状态。

2. **State** 状态接口。状态机有事件输入时，会调用其子类的类方法。

3. **ConcreteState** 具体状态类。不同具体状态类表示不同的状态，并实现当前状态下的方法。可以在与 State 接口间加抽象类避免不同具体状态类之间的重复代码。

   具体状态类也可以保留对状态机 Context 类的引用，方便从其中获取其他数据。

   

   Context 类和 ConcreteState 类都应该能改变 Context 类的当前状态。

## 代码

**State 接口**。

```java
public interface State {
    public abstract void doStart();
    public abstract void doStop();
}
```

**ConcreteState 类** 实现的具体状态类

```java
public class StartState implements State {
    private Player player;

    StartState(Player player) {
        this.player = player;
    }

    @Override
    public void doStart() {
        System.out.println("Already started");
    }

    @Override
    public void doStop() {
        System.out.println("Stop player");
        player.setState(new StopState(player));
    }
}
```

```java
public class StopState implements State {
    private Player player;

    StopState(Player player) {
        this.player = player;
    }

    @Override
    public void doStart() {
        System.out.println("Player starts now");
        player.setState(new StartState(player));
    }

    @Override
    public void doStop() {
        System.out.println("Already stopped");
    }
}
```

**Context 类**

```java
public class Player {
    private State state;

    public Player() {
        this.state = new StopState(this);
    }

    public void pressStart() {
        state.doStart();
    }

    public void pressStop() {
        state.doStop();
    }

    public void setState(State state) {
        this.state = state;
    }
}
```

**测试的客户端**

```java
public class Main {
    public static void main(String[] args) {
        Player player = new Player();

        player.pressStart();
        player.pressStart();
        player.pressStop();
    }
}
```

## 优点

* 符合单一责任原则。不同状态的代码被分到不同类中。
* 简化状态机逻辑，避免巨大的条件语句代码块。

## 缺点

* 当状态较少时，可能这个模式用起来可能太重。
* 不太符合开闭原则。新增状态时要修改旧状态的状态切换流程。