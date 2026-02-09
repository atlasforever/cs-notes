
# 平台驱动 (Platform Driver)

平台总线（Platform Bus）是内核中的一根“虚拟总线”。它将硬件描述（Device）与操作逻辑（Driver）彻底解耦。

* **Platform Device (硬件信息)**：`struct platform_device`，通常写在 `.dts` (设备树) 里。
* **Platform Driver (软件逻辑)**：负责具体的硬件操作代码。
* **Platform Bus (中介)**：当硬件的 `compatible` 属性与驱动的匹配表吻合时，调用驱动的 `.probe()`。

---

## 加载

驱动的匹配并非瞬间完成，而是一个“寻找与绑定”的过程：

1. **静态描述 (DTS)**：内核启动时解析 `.dtb` 。每个具有 `compatible` 属性的节点，内核会自动创建一个 `platform_device` 对象并注册到平台总线。
2. **驱动入驻 (insmod)**：加载 `.ko` 驱动，通过 `platform_driver_register()` 向总线注册。
3. **总线匹配 (Match)**：平台总线遍历设备，如果发现某个设备的 `compatible` 字符串与驱动的 `of_match_table` 完全一致，则匹配成功。
4. **逻辑激活 (Probe)**：匹配后，总线调用驱动的 `.probe()`。
   * **资源提取**：通过 `platform_get_resource()` 获取 DTS 里的寄存器地址。
   * **接口创建**：根据需要注册设备驱动（如作为字符设备 `cdev`）、创建 `/dev/` 等。

---

## DTS

DTS 是由 **节点 (Node)** 和 **属性 (Property)** 组成的树状结构。

* **`compatible`** 用于驱动程序匹配硬件
  * **示例**：`compatible = "fsl,imx6q-i2c", "fsl,imx21-i2c";`

* **`reg`** 描述设备的寄存器基地址以及地址长度
  * **示例**：`reg = <0x0100 0x10>;` (表示起始地址 0x0100，长度 16 字节)。

* **`status`** 控制该设备节点是否被内核加载。
  * **示例**：`status = "okay";`

* **`#address-cells`** 规定子节点 `reg` 中地址的 u32 个数
  * **示例**：`#address-cells = <1>;`

* **`#size-cells`** 规定子节点 `reg` 属性中长度的 u32 个数
  * **示例**：`#size-cells = <1>;`

```devicetree
/dts-v1/;                // 版本声明

// 根节点 (Root Node)
/ {
    node-name@unit-address {    // 子节点：节点名@单元地址
        label: node-name {      // 标签 (Label)，用于后续引用
            property-name = "value";      // 字符串属性
            property-name = <0x123>;      // 32位整型属性 (Cells)
            property-name = [00 11 22];   // 二进制字节流
            
            child-node {                  // 孙子节点
                ...
            };
        };
    };
};
```

### 引用与追加

通过 `&label` 可以在不修改原始 .dtsi 的情况下，追加或修改硬件配置。

```devicetree
// 在另一个文件中修改
&uart0 {
    status = "okay";
    current-speed = <115200>;
};
```
