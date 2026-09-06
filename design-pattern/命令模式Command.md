> 命令模式将请求封装为一个包含了请求的所有信息的独立的对象。通过传递对象让请求发起者与接收者解耦。从而延迟、排队请求的执行；以及支持 undo 操作。

和策略模式较相似，但其意图更倾向于：

* 表示各种**不同效果的动作**，而非算法。
* 把处理逻辑交给接收者，自身**只负责传递**。
* 因为是动作，所以有 undo 等概念。

## 结构

![](images\command.png)

1. **Sender**  (或 *invoker*) 调用者类，是请求的发起者，一定存储了一个 command 对象的引用。Sender 发起请求时操作 command 对象，而非直接传给 receiver。

   Sender 不负责创建 command 对象，通常是通过构造函数由 client 传入。

2. **Command** 命令接口，通常只声明一个方法，用来表示这一个动作的执行。

3. **ConcreteCommand** 具体命令类，对应各种不同请求。一个 ConcreteCommand 对象应该将请求传给业务逻辑对象，而不是自己完成所有工作。虽然为了简化代码，这两者也可以合并。

   执行动作需要的数据可以声明为 ConcreteCommand 类的字段。可以只通过其构造函数赋值这些字段，让这个对象达到 *不可变* 的效果。

4. **Receiver** 接收者类，包含了业务逻辑。大多数 command 类只负责请求的传递工作，receiver 才是真正处理请求的对象。

## 代码

带 undo 功能的电视机遥控器。

**Command接口**。每一个命令都能被执行，以及 undo。

```java
public interface Command {
    public void execute();
    public void undo();
}
```

两个**ConcreteCommand类**，作为具体的遥控器命令。其本身不处理请求，只**传递**给作为 receiver 的`Tv`对象，后者在初始化时指定。

```java
public class TvOnCommand implements Command {
    private Tv tv;

    TvOnCommand(Tv tv) {
        this.tv = tv;
    }

    @Override
    public void execute() {
        tv.switchOn();
    }

    @Override
    public void undo() {
        tv.switchOff();
    }
}
```

```java
public class TvOffCommand implements Command{
    private Tv tv;

    TvOffCommand(Tv tv) {
        this.tv = tv;
    }

    @Override
    public void execute() {
        tv.switchOff();
    }

    @Override
    public void undo() {
        tv.switchOn();
    }
}
```

**Sender类**，用于通过 command 发出请求，而本身不创建 command 对象。通过栈维护一个用于 undo 操作的列表。undo本身因特殊性不作为一个 command。

```java
import java.util.EmptyStackException;
import java.util.Stack;

public class Controller {
    private Stack<Command> history = new Stack<>();

    public void executeCommand(Command command) {
        command.execute();
        history.push(command);
    }

    public void undo() {
        try {
            Command command = history.pop();
            command.undo();
        }
        catch (EmptyStackException e) {
        }
    }
}
```

**Receiver类**，最终接受并实际处理请求。

```java
public class Tv {
    public void switchOn() {
        System.out.println("Switch on tv");
    }

    public void switchOff() {
        System.out.println("Switch off tv");
    }
}
```

**客户端测试**

```java
public class Main {
    public static void main(String[] args) {
        Tv tv = new Tv();
        Command on = new TvOnCommand(tv);
        Command off = new TvOffCommand(tv);

        Controller controller = new Controller();
        controller.executeCommand(on);
        controller.executeCommand(off);
        controller.undo();
        controller.undo();
        controller.undo();
    }
}
```

