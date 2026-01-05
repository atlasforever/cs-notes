# Introduction

网络部件：

* network edge：终端设备、应用程序等
* access network：接入网络的链路与物理媒介（光纤、LTE）
* network core：交换机构成的总体网络
  * **分组交换**（packet switching）：数据被拆成独立的 packet，由分组交换机传送
    * 优点：高效利用链路（不会被某连接占用）、稳定性更强
    * 缺点：无法预测性能、packet 排队延时
  * **电路交换**（circuit switching）：为了通信，链路会特别维持一个连接

# 网络安全

网络中的安全通信涉及如下几点：

* 保密性 Confidentiality：通过**加密/解密**保证截获者无法理解内容
* 完整性 Integrity：保证数据**未被篡改**
* 身份鉴别 Authentication：确认通信的另一方确实为其声称的身份
* 运行安全性 Operational Security：防止被**网络攻击**，如病毒、DDoS等

## 保密性

两种密码体制：

* 对称加密：加密和解密的密钥**相同且保密**
  * 有 AES、DES 等
* 非对称加密：公钥加密，保密的私钥解密
  * RSA 基于大素数分解
  * 避免对称加密中分配 key 的问题

**会话密钥**（session key）：RSA 运算的耗时，而对称密钥又较快，可以将两者结合。先通过公开密钥传输称为**会话密钥**的对称密钥，然后再传输用会话密钥加密的数据

## 数字签名

**数字签名**（digital signature）用于确认一个独一无二的身份，常用公钥算法实现：

* 利用**公钥也能解密私钥加密的信息**的特性，私钥签名，公钥验签
* 解密的信息经比较是正确的，就说明对方的私钥是正确的
* 私钥是独有的，如果私钥是正确的就能验证对方身份

## 完整性

使用不易重复的哈希函数实现：

* 发送方计算报文`m`的散列`H(m)`，将`<m, H(m)>`一起发送
* 接收方用同样方法计算`H(m)`，比较散列确定`m`未被修改
* 常见由 MD5、SHA-1

为防止攻击方同时篡改了数据及散列，需要用**MAC**（Message Authentication Code）保护

* 哈希函数生成
  * 双方共享密码`s`，哈希函数生成`H(m+s)`作为 MAC
* 加密生成
  * 用加密函数对散列加密

## 回放攻击

**回放攻击**（playback attack）：加密报文认证身份时（如使用 password），攻击者无须破译报文，而是直接把加密报文发给认证方以冒充身份

认证方需要使用基于时间戳产生、不会重复的**不重数**（nonce）

![](images\nonce.png)

## 公钥分发

防止有人冒充身份并分发公钥，公钥分发需要依赖 **CA**（Certificate Authority）的证书机制：

* 机构 E 向 CA 注册自己的公钥
  * E 向 CA 提供公钥与身份材料
  * CA 针对 E 的公钥创建一个证书
  * 证书包含 **E 的公钥**与 **CA 对该公钥的签名**
* 用户想获取 E 的公钥
  * 获取 E 的证书（可以直接由 E 发送）
  * 使用 CA 的公钥（操作系统、浏览器内置）验签并接受其中的公钥

## TLS

**TLS** 是安全版本的 TCP，提供安全服务：

* 允许用户认证服务器身份
* 提供数据加密、完整性

![](images\ssl.png)

## 防火墙与 IDS

防火墙在**网络间**对网络包做处理与检测

* **ACL**（Access Control Lists）用于定义通过/拒绝的规则

**入侵监测系统**（Intrusion detection systems）做更复杂的检测

* 检测各种网络攻击

# 传输层

传输层用于进程间交付：

* **多路复用**（Multiplexing）于发送端，将各 socket 的数据集合后交给网络层
* **多路分解**（Demultiplexing）于接收端，将数据递给对应的 socket
  * UDP socket 由二元组`<dest addr, dest port>`决定
  * TCP socket 由四元组`<src addr, src port, dest addr, dest port>`决定

## UDP

![](images\udp.png)

优点：

* header 开销小
* 无建立连接的延时
* 不限制发送速率（没有拥塞、流量控制）

## 可靠传输

数据不会出现损坏、丢失、乱序，需要依靠的机制：

* checksum：检测比特错误
* timer：发送端一定时间没收到 ACK，进行重传
* sequence：报文带 seq，避免因为重传导致重复接收
* ACK/NAK：接收方告知发送端已成功接收，或是报文出错
  * 必须携带 seq 以表明响应的是哪个报文（否则只能 stop-and-wait）
* pipeline：同时传输多个报文，不必对 ACK 做 stop-and-wait

### Go-Back-N

![](images\gbn.png)

**滑动窗口**（sliding window）的一种实现方法，窗口大小限制为 N：

* 发送方
  * 检查窗口是否已满（sent-not-acked 达到 N），未满继续发送，已满则停止等待
  * 对第一个 sent-not-acked 报文设置定时器，超时未收到 ACK 重传所有 sent-not-acked
* 接收方
  * 采用**累计确认**（cumulative acknowledgment），仅当序号 n 之前的报文都正确按序接收才返回 ACK n
  * 乱序的报文都丢弃

### Selective Repeat

GBN 中的累计确定导致接收方会因为一个序号 n 的错误报文，而丢掉序号 n 之后正确收到的报文，导致大量重传。SR 在 GBN 的基础上：

* 采用**选择确认**（selective acknowledgment）接收方单独地为每个数据报发送 ACK
* 发送方对每个数据报设置定时器，只重传没收到 ACK 的报文

相比 GBN：

* SR 适用高错误率网络，避免无用重传
* GBN 适用低错误率网络，避免多余 ACK 节省带宽

## TCP

![](images\tcp.png)

序号字段：

* sequence number：表示**第一个**数据字节在字节流的位置
* acknowledgement number：表示**期待接收**的下一个 seq

**延迟确认**（delayed acknowledgment）减少 ACK 数据包的数量，提升网络性能：

* 收到期待数据时不会立即发 ACK，而是等待（最多 500ms）下一个数据报，等不到再 ACK
* 等待时，可以在发送数据时捎带 ACK，或收到对端下一个数据时直接回复第二个 ACK（累积确认）

**Nagle 算法**用于将小分组报文合并发送，提升网络性能：

* 大报文直接发出
* buffer 中合并小报文，收到 ACK 或合并出大报文时再发出
* 和延迟确认机制会联动会有坏性能（发送方等 ACK 发数据，接收方等数据发 ACK）

**重传机制**（retransmission）结合 GBN 与 SR：

* 接收方使用累积确认（类似 GBN）
* 接收方收到乱序数据不丢弃（类似 SR），返回 ACK 中依旧使用先前的期待 seq
* 发送方等待 ACK 超时只重传第一个 sent-not-acked（而非 GBN 重传全部）
  * 超时取值太长降低速度、太短导致频繁重传
  * 需要先估算 **RTT**（往返时间），再用 Karn 算法计算合适的超时值
* 发送方收到 *三个同序号 ACK*，**快速重传**（fast retransmit）对应序号数据报（仅靠超时太慢）

![](images\retransmit.png)

### 流量控制

![](images\flow-control.png)

接收方处理太慢时，使用**流量控制**（flow control）遏制发送方的发送速度，避免溢出：

* 接收方将接收窗口`rwnd`放入 header 中，告知发送方
* 窗口为 0 时停止发送
  * 窗口可用时，接收方使用 *window update* 报文告知发送方窗口大小
  * 发送方定时发 *window probes* 探测窗口大小（接收方的 window update 可能丢失，发送方因窗口关闭停止发送，导致死锁）

**糊涂窗口**（silly window）问题会导致带宽浪费：

* 接收方不断发送微小窗口通告，发送方不断发送微小报文
* 可以被 Nagle 算法解决

### 连接

#### ![](images\three-way-handshake.png)

![](images\two-way-handshake.png)

* 建立连接（三次握手）
  * 前两次`SYN`报文告知对方起始 seq（某种算法生成）；
  * 第三次握手确认服务器 seq，并可以携带数据
  * 不使用两次握手，是为避免客户端重发`SYN`时，服务端响应了过期的`SYN`
* 关闭连接（四次挥手）
  * `FIN-WAIT-1`等待被动端的 ACK，因为发生协议内部，所以较快
  * `FIN-WAIT-2`等待被动端应用程序调用`close()`，因为半传输，被动端在`CLOSE-WAIT`下还能发数据
  * `TIME-WAIT`确保被动端没收到 ACK 而重传`FIN`时，主动端能回 ACK

### 状态

![](images\tcp-states.png)

## 拥塞控制

**拥塞控制**（congestion control）通过感知网络拥堵程度，来限制发送速率，避免网络崩溃：

* TCP 通过丢包感知网络拥塞
  * 超时、三次重复 ACK
* 通过拥塞窗口`cwnd`限制 sent-not-acked 报文数量，以控制速度
  * 与流量控制的接收窗口`rwnd`共同生效，sent-not-acked 不能超过`min(rwnd, cwnd)`
* 与流量控制的区别
  * 流量控制保护接收方处理能力
  * 拥塞控制保护网络处理能力

![](images\congestion.png)

发送方不停增加发送速率（拥塞窗口）试探网络，直到出现丢包：

* **慢启动**（Slow Start）
  * 开始时`cwnd`为 1 MSS，每个 RTT 后将`cwnd`翻倍（**指数增长**），抵达**阈值**（threshold）后进行**拥塞避免**
  * 出现丢包时重设`cwnd`为 1 MSS 并继续慢启动
* **拥塞避免**（Congestion Avoidance）
  * 线性增长
  * 出现丢包（ACK 超时）重新进行**慢启动**
  * 出现三次重复 ACK（快速重传），说明虽然丢包但网络情况没有特别恶劣（还能收到 ACK），进行**快速恢复**
* **快速恢复**（fast recovery）
  * `cwnd`变为向前一半，每个 RTT 增加 3 MSS，抵达**阈值**（threshold）后进行**拥塞避免**

# 网络层

网络层的两大功能：

* 计算网络的路由，构建**转发表**（forwarding table）
* 在端口间转发数据

转发表决定将数据转发到哪个端口，其两种生成方式：

* 路由器间相互通信，用**路由选择算法**（routing algorithm）生成表
* **SDN**（Software-defined networking）， 由远程主机计算生成表，下发给各路由器

## 路由器

![](images\router.png)

路由器的两大功能：

* 计算网络的路由，构建**转发表**（forwarding table）
* 对数据做端口转发（forwarding）

路由器入口功能：

* 物理层数据接收、链路层处理（拆包、CRC 等）
* 查转发表决定出口端口
  * **基于目的地址转发**（destination-based forwarding），保存地址前缀，使用**最长前缀匹配**；表用*树结构*存储，加快匹配速度
  * 通用转发（generalized forwarding），匹配项不限于目的地址
* 数据报来的过快会**排队**（queuing），造成阻塞与丢包

路由器出口功能：

* **分组调度**（packet scheduling）在出口处排队的数据报中，选出哪些需要被发送
  * FIFO
  * *priority queuing* 将数据包分成不同优先级（某些地址或端口的流量优先权更高）
  * *Weighted fair queuing* 同时保持优先级与公平性

**交换结构**（switching fabric）表示入口到出口间的内部结构：

![](images\switching-fabric.png)

* memory：控制器将数据从入口缓冲区搬到内存，再搬到出口缓冲区
* bus：入口缓冲区和出口缓冲区直接用共享总线通信
* crossbar：网状总线以支持并发传输

## IPv4

<img src="images\ipv4-format.png" style="zoom: 33%;" />

超过链路层 MTU 时需要**分片**（fragmentation）：

* 分片发生在路由器中
* 分片组装发生在端系统中，组装超时就丢弃
* 分片相关属性为`identifier`（分片所属数据报），`flags`（是否还有分片），`offset`（分片的位置）

 **CIDR**（Classless Inter-Domain Routing） 采用`a.b.c.d/n`的形式标记网络（A, B, C 分类方式已弃用）

## NAT

**NAT**（Network Address Translation）解决 IPv4 地址紧张的问题，NAT 里的主机在对外有**同一 IP 地址**

NAT 路由器需要维护 **NAT 表**，当 NAT 里的主机向远端服务器发数据时：

* 将源地址（局域网 IP）改为 NAT 路由器的地址，将原 IP 记录表中
* 将传输层中的目的端口号替换为 NAT 路由器新申请的端口（最多65536个），记录原端口，发送新数据报
* 收到回应时，根据表内容修改还原数据报中的 IP 地址和端口，发给局域网主机

因为需要 NAT 后的主机先发起连接才能产生表记录，该主机**无法作为服务器**，除非使用 **NAT 穿越**：

* **连接反转**（connection reversal）：借助一个不在 NAT 后的中继主机
* **UPnP**：一种可以事先将 *（内网地址，不固定端口号）* 与 *（公网地址，公共端口号）* 相互映射的协议

## DHCP

**DHCP**（Dynamic Host Configuration Protocol）流程：

1. 客户端广播`DISCOVERY`报文
   * UDP 端口`67`、源地址`0.0.0.0`
2. 服务端广播`OFFER`，包含地址、子网掩码、租期等
3. 客户端使用新地址响应`REQUEST`
4. 服务端响应`ACK`

## IPv6

![](images\ipv6-format.png)

IPv6 特性：

* header 固定 40 字节（用`next header`连续拓展）
* **不做分片**，如果超过 MTU 路由器返回 ICMPv6 "packet too big"
* 没有广播，只有全节点组播组`ff02::1`
* 没有 ARP，用 **Neighbor Discovery** 代替（使用 ICMPv6 与组播）
* 128-bit 地址
  * 三种类型：unicast、multicast、anycast（发给组中任意一个地址）

**tunneling**：对于不支持 IPv6 的路由器，可以把 IPv6 数据报作为 IPv4 数据报的 payload

## 多播

**多播**（multicast）用于一对多通信：

* 主机只给多播地址发一次，由路由器复制分组
* 多播路由器会构建一个到所有多播成员的**多播树**（multicast tree）拓扑，数据报只会沿着树发送（而非广播）
* 多播地址（IPv4 Class D） 224.0. 0.0 – 239.255.255.255

![](images\multicast-tree.png)

多播涉及两种协议：

* **IGMP**（Internet Group Management Protocol）让**局域网**中的多播路由器知道本局域网内主机的加入、退出的情况
  * 主机加入时发`MEMBERSHIP`消息
  * 路由器周期地使用`QUERY`查询主机是否还在组内
* 路由器间使用 **multicast routing protocol** 构建多播树、转发多播报文
  * 有多种实现（DVMPR，MOSPF）

## SDN

**SDN**（Software-defined networking）中，**转发表**（forwarding table）不用路由算法自动生成，而是由远程服务器下发

**OpenFlow** 是一种的实现，下发的 **flow table** 包含：

* match：匹配各种 header 属性，包括链路层、传输层的
* actions：匹配后的动作，包括转发、丢弃、修改等

![](images\sdn.png)

## 路由算法

**路由选择算法**（routing algorithm）用于确定 ”最优“ 路由路线

**自治系统**（autonomous systems，AS）表示一堆路由器及其链路的集合，通常由 ISP 指定：

* gateway router 负责不同 AS 间路由
* 同一 AS 内使用相同的路由算法，不同 AS 的算法可以不同

![](images\as.png)

### OSPF

**OSPF**（Open Shortest Path First）用于 **AS 内部**（intra-AS routing），基于 **link-state** 算法：

1. 每个路由器都记录与相邻路由器的链路状态，并使用 OSPF 消息洪泛广播
2. 接受其他路由的广播信息，构成完整的 AS 链路拓扑，使用 Dijkstra 算法计算最佳路径
   * 洪泛仅限于 AS 内，路由器不知其他 AS 的内部拓扑

### BGP

**BGP** （Border Gateway Protocol）用于 **AS 间** （inter-AS routing），路由目标是网络（*a.b.c.d/n*）。使用 **distance-vector** / **bellman-ford** 算法：

1. 每个路由器保存一张最优路径的转发表（表内最初仅知如何路由到相邻节点），将其同步给相邻路由器
2. 接收者用它更新自身的最短路径转发表
3. 如果转发表出现更新，同步给相邻路由器，不断重复 2 与 3

# 链路层

链路层**主体位于网络适配器中**，提供的服务包括：

* 数据封装成帧（frame）
* 如何传输帧（点对点、广播）
* 通过 ACK 、重传来保证**可靠交付**（多见于无线网络）
* 差错检查和纠正

## 差错检查和纠正

* **奇偶校验**
  * 单奇偶校验位：对于 *d* 位数据，选择校验位的比特值，使 *d + 1* 位比特中有偶数个 1
  * 二位奇偶校验：用更多位来做校验，同时还有纠正功能
* **校验和**
  * 数据拆分成一堆16位整数，求和
* **CRC（循环冗余）**
  * 复杂的多项式计算，以太网使用该方法

## 多路访问

链路种类：

* 点对点链路：链路两端点对点（PPP）
* 广播链路：所有节点共享同一信道，每个帧都被广播接收（以太网、WLAN）

**多路访问协议**（Multiple access protocol）协调广播链路上的传输，防止同时发帧产生碰撞，有三种方式：

* **channel partitioning**：拆分信道给每个节点使用
  * TDMA：每个节点独占时间片
  * FDMA：每个节点独占某频段
  * CDMA：多用于无线
* **random access**：所有节点自由传输，检测到碰撞时再做恢复
  * CSMA/CD：发送前先监听，信道空闲时再发数据；检测到碰撞时，随机延时并重复 “监听 - 空闲传输”（以太网）
    * 不适用于无线，因为难以检测无线信号碰撞
  * CSMA/CA：通过 ACK 而非碰撞检测来进行错误恢复（WLAN）
* **taking turns**
  * polling：由 master 决定哪个 slave 可以传输（蓝牙）
  * token-passing：节点间交换令牌，有令牌才能传输（FDDI）

## 局域网

### 地址、ARP

**MAC 地址**：标记网络适配器的 48-bit 地址

* 适配器根据帧的目标 MAC 来接收，不匹配的抛弃
* 广播 MAC 地址是 FF-FF-FF-FF-FF-FF

**ARP**：将 IP 解析成 MAC

* 每台主机都有 ARP 表，表项`< IP address; MAC address; TTL>`将 IP 映射为 MAC，以及映射超时
* MAC 广播地址上，发送 ARP 报文询问 IP 对应的 MAC

### 以太网

![](images\eth-frame.png)

* 使用星形交换机连接，CSMA/CD 进行通信
* payload MTU 1500 字节，超过需分片
* 不可靠传输，以太网帧的 CRC 校验失败直接丢弃

### 交换机

* 根据收到帧的源地址与入帧口，自动生成包含 “MAC - 接口号” 的**交换机表**（switch table）
* 查表确定目标地址网口，只转发到该口；查不到则在所有网口（除了入帧口）都转发

### 虚拟局域网 VLAN

![](images\vlan.png)

![](images\vlan-trunk.png)

* 不同部门共用一个交换机，使用 VLAN 隔离可以防止广播发到其他部门
* 根据 VLAN 编组网口，每个 VLAN 都相互隔绝（将一个物理交换机变为多个*独立的虚拟交换机*）
* **trunk port** 可以构建横跨不同物理交换机的 VLANs，此时以太网帧要加上 **VLAN ID** 来确定帧的 VLAN（802.1q）

### 多协议标签交换  MPLS

![](images\mpls.png)

* 使用标签（位于 link frame header 与 IP header 之间）来决定路由跳转，而非 IP 地址
* 需要路由器支持 MPLS，否则会把 MPLS header 当成 IP header
* 用于 VPN、QoS 等目的

# 无线网络

无线网络概念：

* **单跳**（single-hop）：设备在一个无线跳的网络中（蜂窝数据、蓝牙、WLAN）
* **多跳**（multi-hop）：无线组网，其中一些无线节点作为中继（mesh net）
* **SNR**：信噪比
* **BER**：误码率

## WLAN 802.11

![](images\wlan.png)

WLAN 频谱根据频率划分为 **channel**，AP 需要选择属于一个 channel。

新设备建立扫描并连接（associate）的方式：

* passive scanning
  1. AP 广播包含了 SSID 和 MAC 地址的**信标帧**（beacom frame）
  2. 主机扫描监听信标帧，发送 association request 给所选 AP
  3. AP 响应 association response
* active scanning
  1. 主机广播 probe request，各 AP 响应 probe response
  2. 主机发送 association request 给所选 AP
  3. AP 响应 association response

使用 CSMA/CA 作为多路访问协议，避免链路数据碰撞：

* 发送前监听，等待通道空闲（CSMA）
* 使用 ACK 保证正确接收（无线网络信号微弱，无法进行碰撞检测）

![](images\csmaca.png)

## 移动性管理

**移动性**（mobility）：设备在不同基站/网络间移动时，能保通信连接（IP 地址）

* 归属地（home）保存设备的身份与服务信息
* 拜访地（visited）用于将设备接入网络
* 设备在**拜访地注册**时，需要通知归属地，后者获取设备当前位置
* 归属地维持固定 IP，由归属地将数据再转发给拜访地的设备

![](images\mobility.png)
