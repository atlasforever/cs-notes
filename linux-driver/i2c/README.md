# Linux I2C 设备驱动子系统核心架构总结

Linux I2C 子系统由三部分组成：

1. **I2C 核心 (I2C Core)**：中间层，提供核心 API（如 `i2c_add_driver`），处理驱动与设备匹配。
2. **I2C 总线驱动 (I2C Adapter)**：针对 SOC 内部 I2C 控制器的驱动（如 `i2c-versatile.c`）。
3. **I2C 设备驱动 (I2C Client Driver)**：针对挂在总线上的具体芯片（如 RTC、传感器）的驱动。

关键结构体

* `struct i2c_driver`: 驱动实体，定义 `probe`、`remove` 和匹配表。
* `struct i2c_client`: 代表一个具体的硬件实例，包含从机地址、所属总线。
* **自定义封装**: 通常将 `i2c_client` 嵌入到业务结构体中。

---

## 流程

### 1. 注册

直接使用 `module_i2c_driver` 宏注册驱动

### 2. 匹配

通过 `struct i2c_driver`的匹配表匹配设备树 `compatible` 属性，触发 `probe`

在 `probe` 中可以注册为字符驱动（`cdev`），提供读写与 `/dev/xxx` 接口

### 3. 字符设备关联逻辑

为了让用户态能访问 I2C 设备，通常在驱动中嵌入 `cdev`：

* **关联**: 在 `open` 函数中使用 `container_of` 通过 `inode->i_cdev` 找到自定义结构体。
* **存储**: 将私有数据指针存入 `file->private_data`。
* **访问**: 在 `read/write/ioctl` 中通过 `private_data` 获取 `i2c_client` 指针，从而调用通信 API。
  * 常用 I2C Core 提供的统一 API 如 `i2c_smbus_read_byte_data()`
