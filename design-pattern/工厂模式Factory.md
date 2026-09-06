**工厂模式**封装对象的创建逻辑，而不需要标出需要被创建的对象的明确类型。

客户端可以避免直接用`new`创建产品对象的责任，只需要知道产品对应的参数。减少了代码中的耦合度。

## 简单工厂模式 Simple Factory Pattern

定义一个工厂类用于创建其他类的对象。通过传入参数，获取对应类型的新对象。创建的对象通常都有共同的父类。

工厂类用于创建的方法 *createProduct(string)* 可以设为静态函数。又称为 **静态工厂方法**。方便，但没有传递工厂类对象的方式灵活。



简单工厂模式包含如下角色：

**Factory：工厂角色**：工厂角色负责实现创建所有实例的内部逻辑

**Product：抽象产品角色**：抽象产品角色是所创建的所有对象的父类，负责描述所有实例所共有的公共接口

**ConcreteProduct：具体产品角色**：具体产品角色是创建目标，所有创建的对象都充当这个角色的某个具体类的实例。

![](images\SimpleFactory-1.jpg)

但添加、删除产品类时，需要修改 *createProduct(string)*，不符合开闭原则。

## 工厂方法模式Factory Method Pattern

不同于简单工厂模式中用一个工厂类负责所有产品类的创建，**工厂方法模式**中核心工厂类`Creator`实现了处理产品的方法，但不负责创建产品的逻辑，后者工厂类的子类`ConcreteCreator`负责。

当有创建逻辑有变化时，只需要增加新的具体工厂类。

![](images\factory_pattern_0.PNG)



**作为`Product`和`ConcreteProduct`类。**工厂需要创建使用的产品类。

```java
class Pizza {
    void prepare() {}
    void bake() {}
    void cut() {}
}
class NYStyleCheesePizza extends Pizza {}
class NYStylePeperoniPizza extends Pizza {}
class ChicagoStyleCheesePizza extends Pizza {}
class ChicagoStyleClamPizza extends Pizza{}
```

**作为`Creator`类**。实现了处理产品的逻辑，但不负责创建的流程。

```java
abstract class PizzaStore {
    abstract Pizza createPizza(String name);
    Pizza orderPizza(String name) {
        Pizza pizza = createPizza(name);
        pizza.prepare();
        pizza.bake();
        pizza.cut();
        return pizza;
    }
}
```

**作为`ConcreteProduct`类**。不同的店应该有不同的创建逻辑。

```java
class NYPizzaStore extends PizzaStore {
    @Override
    Pizza createPizza(String name) {
        if (name.equals("cheese")) {
            return new NYStyleCheesePizza();
        } else if (name.equals("peperon")) {
            return new NYStylePeperoniPizza();
        } else {
            return null;
        }
    }
}
class ChicagoPizzaStore extends PizzaStore {
    @Override
    Pizza createPizza(String name) {
        if (name.equals("cheese")) {
            return new ChicagoStyleCheesePizza();
        } else if (name.equals("clam")) {
            return new ChicagoStyleClamPizza();
        } else {
            return null;
        }
    }
}
```

**测试**

```java
public class Main {
    public static void main(String[] args) {
        NYPizzaStore nyStore = new NYPizzaStore();
        nyStore.orderPizza("cheese");
    }
}
```



## 抽象工厂模式Abstract Factory Pattern

工厂方法模式只能生产一类产品（如上例的`PizzaStore`类唯一的生产方法`createPizza`来生产`Pizza`，虽然它可以根据参数返回不同的`Pizza`子类）。**每个工厂方法类是针对一类产品的**。

抽象工厂有多个生产方法来生产不同类型的产品，这些不同类的产品构成一个**产品族（product family）**。**每个抽象工厂是针对一个产品族的**。

**当需要创建一组属于同一产品族的不同类型产品，且想让它们关联起来时，可使用抽象工厂模式**。



![](images\structureaf.png)

**一种产品类**

```java
// 相当于 ProductA
interface Button {
    void paint();
}
// 相当于 ConcreteProductA1 和 ConcreteProductA2
class WinButton implements Button {
    @Override
    public void paint() {}
}
class MacButton implements Button {
    @Override
    public void paint() {}
}
```

**另一种需要被生产的产品类**

```java
// 相当于 ProductA
interface CheckBox {
    void paint();
}
// 相当于 ConcreteProductB1 和 ConcreteProductB2
class WinCheckBox implements CheckBox {
    @Override
    public void paint() {}
}
class MacCheckBox implements CheckBox {
    @Override
    public void paint() {}
}
```

**抽象工厂类的抽象类和具体类**

```java
// 相当于 AbstractFactory，它生产的两类产品（Button和CheckBox）构成一个产品族。
// 并且一个产品族中的不同产品往往是有关联性的。
interface GUIFactory {
    Button createButton();
    CheckBox createCheckBox();
}
// 相当于 ConcreteFactory 的具体工厂类。生产的产品都在一个产品族（Win）中，相互兼容，但
// 和MacGUIFactory的产品不兼容。
class WinGUIFactory implements GUIFactory{
    @Override
    public Button createButton() {
        return new WinButton();
    }

    @Override
    public CheckBox createCheckBox() {
        return new WinCheckBox();
    }
}
// Mac产品族的具体工厂类
class MacGUIFactory implements GUIFactory {
    @Override
    public Button createButton() {
        return new MacButton();
    }

    @Override
    public CheckBox createCheckBox() {
        return new MacCheckBox();
    }
}
```

**客户端**

```java
// 需要用到同一产品族的不同产品，但不用关心是属于Win还是Mac产品族，只关心自己处理的逻辑。不依赖于
// 具体的产品。同时不会出现不同产品族产品的错乱交织。（一起绘制了 WinButton 和 MacCheckBox)
class AppClient {
    private GUIFactory factory;
    private Button button;
    private CheckBox checkBox;

    public AppClient (GUIFactory factory) {
        this.factory = factory;
    }
    public void createUI() {
        button = factory.createButton();
        checkBox = factory.createCheckBox();
    }
    public void paint() {
        button.paint();
        checkBox.paint();
    }
}
```

**测试**

```java
// 创建需要用到的具体工厂类对象，具体产品对象的创建都通过这个工厂对象
public class Main {
    public static void main(String[] args) {
        GUIFactory factory = new WinGUIFactory();
        AppClient app = new AppClient(factory);
        app.createUI();
        app.paint();
    }
}
```

### 优点

* 隔离了具体类的实例化，客户端不需要知道对象具体类型，只要针对对象的公共接口编程就行。
* 当一个产品族的多种产品需一起工作时，抽象工厂可以保证同一时刻用到都是一个产品族的产品。
* 增加新的产品族时方便，只需添加新的具体抽象工厂类，符合开闭原则。

### 缺点

* 增加新产品时，必须往所有抽象工厂类中增加生成方法。

### 工厂模式的退化

* 当抽象工厂模式中每一个具体工厂类只创建一个产品对象时，抽象工厂模式退化成工厂方法模式。
* 当工厂方法模式中工厂的抽象类和具体类合并，提供一个统一的工厂来创建产品对象，并将创建对象的工厂方法设计为静态方法时，工厂方法模式退化成简单工厂模式。



