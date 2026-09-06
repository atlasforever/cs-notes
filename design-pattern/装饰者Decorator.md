动态地将需要附加的功能属性（称为**Decorator**，装饰者)，添加到需要被装饰的对象（称为**Component**）。

Decorator通过包含装饰Component，而Decorator外层可再装饰多层的Decorator，每层的Decorator都给Component加上新功能。

这种方式灵活符合开闭原则，增加新Decorator，新Component时都不会修改旧类。

![](images\decorator_design_pattern_class_diagram_1.jpg)

* 继承`Component`超类（也可以是接口）只是为了让子类能相互包含
* `ConcreteComponent`不需要知道`ConcreteDecorator`

（下图展示，在披萨Component外面装饰一层辣椒Decorator、一层芝士Decorator）

![](images\piz3.png)

**作为Component超类的Drink**

```Java
interface Drink {
    double cost();
    String getDescription();
}
```

**作为ConcreteComponent的具体饮料类，用于被装饰**

```Java
class DarkRoast implements Drink{

    @java.lang.Override
    public double cost() {
        return 3;
    }

    @java.lang.Override
    public String getDescription() {
        return "Dark Roast";
    }
}
```

**抽象装饰类Decorator**

```Java
abstract class Decorator implements Drink {
    protected Drink drink;

    public Decorator(Drink d) {
        drink = d;
    }
}
```

**作为ConcreteDecorator的具体调料类，用于装饰饮料**

```Java
class Milk extends Decorator {

    public Milk(Drink d) {
        super(d);
    }

    @java.lang.Override
    public String getDescription() {
        return drink.getDescription() + " with Milk";
    }

    @java.lang.Override
    public double cost() {
        return drink.cost() + 0.5;
    }
}

class Sugar extends Decorator {
    public Sugar(Drink d) {
        super(d);
    }
	
    @java.lang.Override
    public String getDescription() {
        return drink.getDescription() + " with Sugar";
    }

    @java.lang.Override
    public double cost() {
        return drink.cost() + 0.3;
    }
}
```

**测试**

```Java
public class Main {
    public static void main(String[] args) {
        Drink dr = new DarkRoast();
        // 可用不同的装饰者，连续装饰增加新特性
        Drink addTwoMilk = new Milk(new Milk(dr));
        Drink addSugarTwoMilk = new Sugar(addTwoMilk);
		// 调用会层层递归，在被装饰者的功能上加上每一个装饰者的功能效果
        double cost = addSugarTwoMilk.cost();
        String description = addSugarTwoMilk.getDescription()；
        System.out.println("Cost is " + cost + " Description is " + description);
    }
}
```

