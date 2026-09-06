建立对象间一对多的订阅依赖关系。`Observable`对象有变化时，可以通知所有订阅的`Observer`对象。

**优点**

* 通过订阅，对象之间松耦合
* 支持广播通信

![](images\image1.png)

![](images\observer-pattern.png)

**Observable接口**

```Java
public interface Observable {
    public void addObserver(Observer o);
    public void removeObserver(Observer o);
    public void notifyAllObservers();
}
```

**Observer接口**

```Java
public interface Observer {
    public void update(Object obj);
}
```

**ObservableImpl类**

```Java
public ObservableImpl implements Observable{
    private ArrayList<Observer> observers = new ArrayList<Observer>();
    private int state = 0;

    @Override
    public void addObserver(Observer o) {
        observers.add(o);
    }
    @Override
    public void removeObserver(Observer o) {
        observers.remove(o);
    }
    @Override
    public void notifyAllObservers() {
        for (Observer o : observers) {
            o.update((Object)getState());
        }
    }
    public int getState() {
        return state;
    }
    public void setState(int s) {
        state = s;
        // 更新时最好主动推送数据给观察者
        notifyAllObservers();
    }
}
```

**ObserverImpl类**

```Java
public class ObserverImpl implements Observer {
    private Observable observable = null;

    public ObserverImpl(Observable observable) {
        this.observable = observable;
    }

    @Override
    public void update(Object obj) {
        System.out.println("Update myself to" + (int)obj);
    }
    public void unsubscribe() {
        observable.removeObserver(this);
    }
}
```

**测试**

```Java
public class Main {
    public static void main(String[] args) {
        ObservableImpl subject = new ObservableImpl();
        ObserverImpl observer = new ObserverImpl(subject);

        subject.addObserver(observer);
        subject.setState(3);
        subject.removeObserver(observer);
        subject.setState(4);
    }
}
```



