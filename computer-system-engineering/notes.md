# 导论

面对高复杂度系统的方法：

* Modularity：拆分成各种子系统
* Abstraction：关注外在效果，而非内部实现
* Layering：减少模块间的内部耦合，只与相邻层通信
* Hierarchy：一堆小的模块组成一个更大的模块（如行政区）

# 网站拓展性

## 加强拓展性的方法

当 application server、database、file server（存储图片文件等）处于同一台服务器，这个架构难以扩展：

* 单台设备的 CPU，内存，硬盘能力都有上限

![](images\lec21.png)

提升可扩展性的方法：

1. **拆分数据与应用**
   * application server、database、file server 分别放到独立的服务器上
   * 分别拓展各自硬件
   * 使用局域网通信
2. **缓存**
   * 数据在内存，减少硬盘操作
   * cache miss 时再去访问 database、file server
   * 可有多台缓存服务器，通过 hash(key) 选中某一台缓存服务器
   * 添加缓存服务器时，consistent hashing 可以保证命中率
3. **更多的 application server**
   * load balancer 作为统一入口，将请求分派给服务器
   * 服务器应是 stateless（状态保存在数据库中），保证灵活替代
   * load balancer 可以使用 hash、轮询等算法来负载均衡
4. **更多的 database**
   1. 主从读写分离
      * primary 用来写，数据同步到多台 secondary
      * secondary 用来读
   2. 大 table 拆分到多个服务器中
5. **更多的 file server**
   * NFS 等进行分布式文件系统
6. **CDN**
   * 缓存图片等静态资源
7. **拆分微服务**
   * MQ、RPC 通信
   * 也可使用数据库、cache 共享数据来通信

![](images\lec22.png)

## CAP 理论

分布式系统只能同时满足两个：

- 一致性（**C**onsistency）：所有节读到同样的最新数据，或者失败
- 可用性（**A**vailability）：每次请求都能获取到非失败的响应——但是不保证获取的数据为最新数据
- 分区容错性（**P**artition tolerance）：出现网络分区时，系统还能运行

P 是不可控的已发生现实，只能选择 C 或 A：

* CP：宁可转账失败也要保证金额正确（支付宝）
* AP：没货了也能下单，大不了不发货（淘宝）

# RPC

客户端可以直接调用服务端的函数，底层的具体通信由 RPC stub 代码自动生成。

![](images\lec51.png)

![](images\lec52.png)

* 文本编码数据（JSON、XML）
  * 可读性好
  * 不好表示二进制串（base64）；占用字节多
* 二进制编码数据
  * 占用更小、速度更快

## 错误处理

RPC 调用会因网络丢失、服务端忙而收不到响应，客户端可能会**重传**。根据调用的幂等性，RPC 有两种语义设置：

* at-least-once：客户端请求反复重传，服务端反复执行
* at-most-once：服务端记录请求 id，同一请求只会执行一次

# inode 文件系统

一个 **inode** 表示一个文件，记录了文件所有的硬盘 *block*  号，文件系统通过 inode 来找到并访问实际数据。inode 的结构：

* 元数据（权限、owner、修改时间等）
* 一级索引，直接指向 data block
* 二级索引，指向存储一级索引的 index block

![](images\lec31.png)

* 目录与文件名
  * 文件名保存在目录中（而非对应的 inode）
  * 目录也是文件，内容为文件名到 inode 的映射表
* 硬链接
  * 找到目标文件 inode，在父目录中创建指向其的目录项（共享 inode）
* 软链接
  * 创建符号文件，其保存目标文件路径的字符串

文件系统布局，使用 bitmap 标记已使用的数据

![](images\lec32.png)

## 文件系统容错性

文件系统写文件涉及多个步骤（写数据、写 inode、写空闲块 bitmap）

过程中如崩溃，重启后需要错误恢复机制保证数据的一致性（*crash consistency*）：

* fsck（ext2）
* 日志文件系统（ext3/4）

### fsck

扫描文件系统的元数据、inode、实际数据块，遇到异常时尝试修复

需要扫描整个文件系统，速度慢

### 日志文件系统

使用日志（journal）恢复数据：

1. 操作与数据记录在日志中（此时崩溃，直接丢弃数据）
2. commit
3. 日志的内容更新到硬盘中（此时崩溃，恢复时重新从日志更新）

![](images\lec101.png)

使用日志会导致修改写入两次（一次日志，一次数据块），需要在 crash consistency 与性能间权衡。ext4 的几种写入方式：

* **writeback**：数据部分不记入日志，直接写回硬盘。没有写回顺序，一致性较差
* **ordered**：同 writeback，但数据先写硬盘后记入日志
* **journal**：元数据与具体数据部分都记入日志

# 分布式文件系统

借助 RPC 实现远程的文件操作（create、delete、read、write）

可能因为网络导致：

* 速度过慢
* 反复请求

## NFS

NFS 客户端通过 RPC 进行文件操作

![](images\lec61.png)

### mount

1. 客户端（内核中的 NFS 驱动）请求挂载服务端的某目录
2. 服务端返回该目录的 *file handle*
3. 客户端创建 in-memory VFS inode，指向远程目录

### 读写

没有 `open()`对应的 RPC 函数，服务端**无状态**不维护任何数据，客户端每次请求传输所有所需数据（客户端维护）

每次 RPC 请求 `READ`，`WRITE`时，服务端都重新调用一轮 `open()`，`lseek()`，`read()`/`write()`，`close()`

![](images\lec62.png)

file handle 使用 inode 号标识文件，而非路径字符串。可以保证永远指向同一个文件，而不担心中途重命名的情况。

![](images\lec63.png)

### 缓存与一致性

* **缓存**
  * 客户端会将文件内容等数据缓存本地，减少远程调用次数
  * 服务端会将所有 write 直写入硬盘，防止断电
  * 服务端会将新修改推送给客户端的缓存
* **一致性**
  * 客户端只会在 `close()`时将修改保存至服务端
  * 完整的并发安全需要应用程序自己实现

## GFS

GFS（Google File System）设计适用背景：

* 文件大，保存时间长
* 服务器会经常故障
* 读多于写，且读多为整片读取，写多为 append
* 并发 append

设计原则：

* 一台 master 统一管理 **metadata** （map files to chucks）
  * 只有一台 master，方便保证一致性
  * 读多于写，并发压力小一台足够
* 文件拆为多个 chuck，每个 64 MB ，保存在 chuckserver 中
  * 减少通信次数
  * 更易管理
* 同一 chuck 备份在多个 chuckserver 中

![](images\lec64.png)

![](images\lec65.png)

### 读写

master 服务器用读写锁进行保护

**读文件**：

1. 从 master 获取文件的 chuck 列表
2. 获取每个 chuck 的地址
3. 从任一可用的 chuckserver 中获取 chuck 数据

**写文件**中，数据传输（data flow）与写入指令（control flow）被分开，用于传输速度：

1. 数据传输
   * 客户端发给最近的 chuckserver，chuckserver 相互同步
2. 数据写入
   * 客户端发给 primary，primary 发给所有 secondary

![](images\lec66.png)

![](images\lec67.png)

# K/V存储

## log-structured file

数据操作通过 *append* 形式追加尾部，不改动旧值，写速度更快（*sequential writes* 快于 *random writes*）

* *insert*、*update* 都在尾部 **append** 一个新的 K/V pair
* *get* **从尾往前**匹配 key
* *delete* 通过标记实现，在 *compact* 过程中删除
* 使用 *index* 实现 *O(1)* 访问，但需要将所有数据读入内存

![](images\lec91.png)

log-structured file 在追加中会越来越大，解决方法：

* 拆分成固定大小的 *segments*
* *compact* 操作删去一个 *segment* 里的重复项
* 合并 compacted segment

![](F:\我的坚果云\CS\courses\computer-system-engineering\images\lec92.png)

## B-Tree

* key 有序排序
* 支持**范围查找**
* 写操作为 *random write*，速度不及 log-structured file

![](images\lec93.png)

## LSM Tree

**Log-Structured Tree** 结合 log-structured file 与 B-Tree，为有序的 log-structured file：

* 写入速度快
* 适用大数据量
* 查找新加入的数据较快

先将将数据写入内存中的 *MemTable*，待其满足条件时作为 *SSTable* (Sorted String Table) 保存到硬盘：

* MemTable

  * 使用 log-structured 方式写入，速度快（*sequential writes* 快于 *random writes*）
  * 满足条件时保存为硬盘上的 SSTable
* SSTable
* 每个 SSTable 内部的 key 有序
  * SSTable 之间也有序，类似有序二叉树
  * 新数据保存在上层

![](images\lec94.png)

## 分布式 K/V

分布式 K/V：用 RPC 实现 *get*、*put*




# MapReduce

借助 *Map* 与 *Reduce* 完成并行计算，由各机器上的 worker 执行

将数据拆成 M 个 shard 给 M 个 map-worker，map-workers 的结果再聚合后给 R 个 reduce-worker 处理生成结果

![](images\lec71.png)

* **容错机制**
  * master 检测到 worker 无响应，会重新安排 worker 并执行
* **Locality**
  * map worker 与数据源通常在一台机器，减少网络传输耗时
* **适用性**
  * 并非适用所有情况，如社交网络等图结构

## 流程

1. 数据拆成 M 个 shard
2. master 给空闲 worker 分配 map task 或 reduce task
3. 每个 map-worker 把输入 (shard) 映射为 intermediate (键值对 *<key, value>*)
   * intermediate 会被保存到分布式文件系统上（才能被远程的 reduce-worker 继续使用）
4. *partitioning function* 决定每个 reduce-worker 对应要处理的 *key*
5. intermediates 被排序，相同 key 的 value 组合为列表后传给一个 reduce-worker
6. reduce-worker 进行 *reduce* 操作
7. R 个 reduce-worker 的 R 个结果返回给用户

![](images\lec72.png)

## 例：统计单词

![](images\lec73.png)

![](images\lec74.png)

# 数据库

数据库数据描述：

* record：表
* tuple：行
* attribute：某项数据

事务特性 **ACID**：

* **A**tomicity：原子性（*all-or-nothing*）
* **C**onsistency：要求用户代码正确，和 DBMS 无关
* **I**solation：并行的事务相互隔离（*before-or-after*）
* **D**urability：所有已 commit 的事务必须被存到硬盘中（借助 *log* 或 *shadow copy*）

## 存储

### page

数据读写的基础单元为 *page*，多个 page 构成数据库的存储文件

* 固定大小，1 KB ~ 16 KB
* 可以保存不同类型的数据，有 *data page*，*index page* 等

page 保存 tuple 的两种主要方式：

* *slotted page*： page header 保存该 page 中每个 tuple 的 offset
* *log-structured*：page 记录每一项修改动作（insert、update、delete）， 从最新往前读构建 tuple
  * page 可以通过 compact、merge 操作去除多余的操作记录，减少大小

### heap file

heap file 用于保存 pages

* 不关心内部 page 的顺序（顺序由 index 负责，heap 只是存储 pages）
* 记录着哪些 page 有剩余空间，用于寻找可用 page 保存新 tuple

![](images\lec172.png)

### tuple layout

![](images\lec171.png)

* header
  * 指示`NULL`值的 bitmap
  * 控制并发访问的属性
* attribute data
  * 该列数据的字节序列
  * attributes 的顺序与每项大小，由 DBMS 根据 schema 解释，无需存在 tuple 中

DBMS 里各数据库、表的 meta-data 保存在 DBMS 内部的 catalog 里（见`INFORMATION_SCHEMA`）

## Buffer Pool

buffer pool 作为硬盘 page 在内存上的缓存

* *frame*：用于存放 page
* meta-data
  * *page table*：跟踪当前在内存中的 pages
  * *dirty-flag*：是否被修改，如有则需写回硬盘
  * *pin count*：该 page 被多少个线程访问

![](images\lec181.png)

### OS 缓存

数据库不使用`mmap()`或写硬盘时 OS 自带的 page cache（使用`O_DIRECT`略过 OS 机制），而是自己维护 buffer pool 作缓存

* 出于安全性，需要自定 page 写回硬盘的方式
* 自定的缓存置换策略

### 优化

* *Multiple Buffer Pools*：给不同的 database、page type（index、data）分配不同的 buffer pool，避免竞争
* *Pre-fetching*：根据当前的 query，提前载入相邻的 page
* *Scan Sharing*：在不同 query 间复用数据结构，减少重复扫描的时间

### 缓存置换机制

需要空的 frame 存放新读出的 page 时，需要决定 evict 掉哪个 frame

不能单纯使用 *LRU/CLOCK* 算法，否则 *sequential flooding* 下会一直 cache miss， 需要根据当前 query 的类型来决定

*sequential flooding*：LRU 下连续的 `select *` 会导致不停读入新 page

1. buffer pool 有 *2* 个 frame，文件有三个 page `P1`, `P2`, `P3`
2. LRU 下，两次`select`（`P1`,`P2`,`P3`,`P1`,`P2`,`P3`）会一直 cache miss

## 事务：崩溃恢复

*crash recovery algorithm* 可以保证事务的原子性、持久性

**ARIES**（Algorithms for Recovery and Isolation Exploiting Semantics）是一种依赖日志的崩溃恢复算法：

* **Write Ahead Logging**：所有改动步骤都先作为 *log record* 保存到 *log* 中，再修改实际位置
* **REDO**：事务 commit 但没做完，重做以保存数据
* **UNDO**：撤销不完整或 abort 的事务影响
* 流程
  1. 从尾到头扫描日志
  2. 未 commit 的事务动作，标记为 **ABORT** 后 UNDO
  3. REDO 已经 commit 的事务动作

`begin`与`commit`间的每一个写动作都作为一项 *log record*，包含字段：

* 事务 id
* 被写数据项 id
* 旧值（用于 UNDO）
* 新值（用于 REDO）

**checkpoint** 用于避免日志过大，checkpoint 之前的内容都保存到硬盘中去了，因此其之前 commit 的事务都可删除

![](images\lec111.png)

## 事务：并发控制

两个事务中的操作会产生 **conflict**，当：

1. 操作同一数据，且
2. 至少有一个在写入，且
3. 属于不同事务

*conflict* 的并发事务操作可能引起数据错乱， DBMS 多采用让操作执行流程满足 **Conflict Serializability** 以同时保证并发性能与隔离性：

* 两个事务的 *conflict* 操作不出现交错（用有向图表示事务冲突时，*不会出现环路*）

（图例右下角 *conflict* 出现交错，不符合 *Conflict Serializability*）

![](images\lec121.png)

并发下的隔离性需要用锁实现，以下两种**不满足** *Conflict Serializability* （必须 *two-phase lock*）：

* *global lock*：锁住从头到尾的整个事务
  * 无法并发处理事务，性能不行
* *simple fine-grained lock*：每个数据都有锁，用前加锁，用后解锁
  * 依然会被其他事务的改动影响，没有隔离性（下例`T1`会读到`T2`改动）

![](images\lec122.png)



### two-phase lock

借助 *two-phase lock* 实现 *Conflict Serializability*，保证隔离性：

* 每个共享数据都有对应的锁（也属于 *fine-grained lock*）
* 操作数据前获取锁
* **释放任一锁后，不能再获取任何锁（只能继续释放）**
  * 整个流程分为 *acquire phase* 和 *release phase*，只能在 acquire phase 获取锁，release phase 释放锁

![](images\lec123.png)

*two-phase lock* 缺点：

* 可能会死锁
* 有 insert、delete 动作时，无法避免 *phantom* （幻读）问题
  * 某一 tuple 在事务过程中，有时能读到、有时不能

![](images\lec124.png)

### 乐观并发控制

**乐观并发控制**（Optimistic Concurrency Control, OOC）不需要加锁，避免 two-phase lock 中的死锁问题

* **乐观**：认为冲突是少见的，因此执行中不加锁保护；如果发现事务最终结果有冲突，再另外 *abort & retry*
* 适用于事务 *冲突较少* 的情况，否则会一直 abort & retry 事务，导致 livelock

OOC 执行流程流程：

1. **Read Phase**：每个事务申请私有空间，读数据到 *read set*，写数据到 *write set*
2. **Validation Phase**：事务提交时，检查是否与其他事务冲突（其他事务的 read set 包含本 write set 修改的数据）
3. **Write Phase**：如果 validation 成功则保存，否则重试本事务

![](images\lec131.png)

### 多版本并发控制

**多版本并发控制**（Multi-Version Concurrency Control, MVCC）也可在 *无锁* 情况下解决并发问题

* 每个数据都有多个版本
* 写操作不覆盖原数据，而是保存为数据的新的版本
* 读操作选择最新版本作为 snapshot，在该版本上读数据（并发写的内容不会影响到该版本）

## 分布式事务

**two-phase commit** 可以保证分布式事务涉及不同设备时，事务总体具有原子性

* **prepared phase**：*coordinator* 下发 *PREPARE* 询问所有 clients 是否可以 commit，如果收到任一 client 的 *ABORT* 响应（或是响应超时），coordinator 则给所有 clients 发送 *ABORT* 撤销事务；否则给所有 clients 发 *COMMIT* 进行事务提交
* **commit phase**：coordinator 发送 *COMMIT* 或 *ABORT*，并等待响应
* 总之，*prepare phase* 中询问所有 clients 的同意，*commit phase* 中下发 commit 或 abort 指令

commit 的错误恢复：

* coordinator 下发 COMMIT 后事务就无法撤销，client 如果崩溃需要依靠 PREPARE 阶段保存的 log 进行错误恢复
* client 重启后扫描 "prepared but not committed" 的 log，并进行恢复

![](images\lec141.png)

