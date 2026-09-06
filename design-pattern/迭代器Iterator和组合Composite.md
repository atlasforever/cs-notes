## 迭代器模式 Iterator Pattern
> 迭代器模式提供了一种方法**顺序**访问一个聚合对象中的各个元素，而又不暴露其内部的表示。

许多集合的内部结构可能很复杂（非列表），但通过迭代器模式可以依次访问其中的元素。

同时，迭代器模式把在元素间游走的算法**交给了迭代器**，而不是聚合对象。让聚合对象的接口和实现更加简洁，专注于其应该做的事情（管理对象集合），让责任明确。

### 结构

![](images\iterator.png)

1. **Iterator** 接口提供了迭代器该有的操作。
2. **ConcreteIterator** 实现了遍历集合的算法。同时其对象应保存遍历状态，能让多个迭代器遍历一个集合对象。
3. **IterableCollection** 提供让所有聚合对象都能创建迭代器的接口。聚合类只要把遍历的责任交给创建出的迭代器。
4. **ConcreteCollection** 聚合类。负责这个聚合类的功能（那些方法此图中省略），*createIterator()* 返回具体迭代器类的新对象。

### 代码

**Iterator**接口

```java
interface Iterator {
    String getNext();
    boolean hasMore();
}
```

**IterableCollection**接口
```java
interface IterableCollection {
    Iterator createIterator();
}
```

**ConcreteCollection**类
```java
class NameRepository implements IterableCollection{
    private String[] names = {"Robert", "John", "Julie", "Lora"};

    void operation() {/* Some operation to this repository */}

    @Override
    public Iterator createIterator() {
        return new ConcreteIterator(names);
    }
}
```

**ConcreteIterator**类

```java
class ConcreteIterator implements Iterator {
    private String[] names;
    private int index;
	// 获取的数据结构，用于遍历操作
    public ConcreteIterator(String[] names) {
        this.names = names;
    }

    @Override
    public boolean hasMore() {
        if (index < names.length) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String getNext() {
        if (this.hasMore()) {
            return names[index++];
        } else {
            return null;
        }
    }
}
```

**客户端测试**

```java
public class Main {
    public static void main(String[] args) {
        NameRepository repo = new NameRepository();
        Iterator iterator = repo.createIterator();
        while (iterator.hasMore()) {
            System.out.println(iterator.getNext());
        }
    }
}
```

### 优点

* 符合单一责任原则。让聚合类和客户端不用关心遍历的算法。
* 符合开闭原则。可以将新的聚合类及其迭代器传给现存代码而不破坏结构。
* 迭代器内部有状态，可以在一个聚合类上使用多个迭代器，以及暂停后继续用。

## 组合模式 Composite Pattern

> 把一组对象当作一个单一的对象。其依据树形结构来组合这一堆的对象，表示层次关系。客户端可以通过相同的接口统一地访问单个或组合的对象。

### 结构

![](images\composite.png)

1. **Common** 接口，描述了单个和组合元素共同的行为。

2. **Leaf** 类是数中的基本元素，不会在有任何子元素。通常 leaf 对象做了大部分的实际任务，因为它们没有可将任务委托给的对象了。

3. **Composite** （或 *container*）类，其可拥有多个子元素：leaf 和 其他的 composite。其不需要知道子元素的具体类型，只要通过 component 接口工作。

   收到请求后， composite 对象把任务**递归**委托给所有的子元素，处理结果后返回最终结果给 client。

4. **Client** 类，通过 component 接口来处理所有的元素。

### 应用场景

* 需要实现树形的对象结构。
* 通过接口能同一的处理多个对象，无论个数和具体类型是否一致。

### 优点

* 使用多态和递归更方便的操作复杂的树形结构。
* 符合开闭原则。可以增加往里添加新的节点类型。

### 缺点

* 当具体类的效果相差较大时，难以设计出那个共同的 component 接口。