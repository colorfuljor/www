# www
DBMS Project
## 系统基本说明
本系统主要是基于针对NVM优化的数据结构FPTree，实现一个简单的键值存储引擎FPTreeDB。我们通过将其包装成一个调用库，供用户程序使用并管理其数据存储，与LevelDB的使用方式类似。对外可用的数据基本操作有增（insert）删（remove）改（update）查（find）。在系统恢复操作上，使用BulkLoading的方式。系统实现上主要是非叶子结点节点的结构类B+Tree、叶子节点要用PMDK与NVM进行数据交互。
## 作者
温鸿玮（17308164）、王俊焕（17308164）、王鑫基（17309084）
## 实现时间计划
1. 5月4日前：系统说明书，PAllocator实现并通过utility测试，LevelDB的使用以及测试（对应lycsb.cpp，p_allocator.cpp的实现和运行，utility_test.cpp的运行）
2. 5月4日-5月11日：FPTreeDB插入和重载操作并通过相关测试（对应fptree.cpp的实现和fptree_test.cpp部分的运行）
3. 5月11日-5月18日：FPTreeDB查询和更新操作并通过相关测试（对应fptree.cpp的实现和fptree_test.cpp部分的运行）
4. 5月18日-5月31日：FPTreeDB删除操作和所有剩下实现以及测试（对应fptree.cpp的实现和fptree_test.cpp所有的运行）
## 使用方法（现仅供测试）
[2019/5/11]可进行系统分配叶子空间以及树节点插入的测试
1. 在终端进入到./Programing-FPTree/test 输入make编译运行出可运行文件
3. 进入./Programing-FPTree/test/bin 输入./utility_test即可测试系统分配叶子空间，./fptree_test即可测试叶子结点的插入操作
[2019/5/4]系统尚未完成，只能进行系统分配节点的测试，增删改查功能尚未完成。
1. 进入到./Programing-FPTree/test 新建data文件夹或修改p_allocator.h内路径
2. 在终端进入到./Programing-FPTree/test 输入make编译运行出可运行文件
3. 进入./Programing-FPTree/test/bin 输入./utility_test即可测试
## 实现进度
[2019/5/11]完成树的叶子节点的插入、重载
1. 完成InnerNode::findIndex(Key)二分查找找到键插入的合适下标
2. 完成InnerNode::insertNonFull(Key,Node *)、InnerNode::insert(Key,value&)、InnerNode::insertLeave(Keynode&)递归的插入非叶子键值，最终插入叶子结点
3. 完成InnerNode::split()中间结点的分裂
4. 完成FPTree::bulkloading(),利用pAllocator->startLeaf以链表形式载入，通过root->insertLeave()插入结点
[2019/5/4]PAllocator.cpp实现、通过utility测试、通过LevelDB测试
1. LevelDB是google实现的键值数据库。我们需要学会文件读写、调用LevelDB的接口完成相应写入、读取（增删改查）功能。我们通过编写lycsb.cpp实现将workload中的txt文件转化为键值对，存入到LevelDB中，并对LevelDB功能进行测试。
2. PAlloctor是NVM文件管理的主要对象，负责分配LeafNode在NVM的空间，映射数据文件并返回虚拟地址给LeafNode使用。主要实现叶结点的分配、get、free方法，该过程需要使用到PMDK的libpmem库对NVM进行操作。