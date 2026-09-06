需要让系统中某些类**只能有一个实例**（例如一个系统只能由一个文件系统）时使用。

全局变量无法防止实例化多个对象。应让这个类：

1. 自身保存唯一实例。
2. 提供该实例访问方法。
3. 保证不会有其他实例的创建。

![](images\structuresl.png)

```java
class Singleton {
    private String uniqueInstance;
    private Singleton();
    
    public String getInstance() {
        // 双重检查加锁，防止多线程多次 new 的问题。又避免每次访问都同步带来的开销
        if (uniqueInstance == null) {
            synchronized (Singleton.class) {
                if (uniqueInstance == null) {
                    uniqueInstance = "Singleton";
                }
            }
        }
        return uniqueInstance;
    }
    
}
```

