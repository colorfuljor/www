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