相较于源代码，主要改动了以下几点。
1. 在KSimJoin.h中增加了Cre_VsMap()函数，用来提前计算超节点。补全了UpperBound()函数的功能

2. 对Sim_pair结构体内重载了 < 符号，去掉了mycomp()函数。否则IncrementalSimRank()函数中的 K.push(Sim_pair(v, u, Sim[v][u])); 会报错。
这里改成了小根堆，即K.top()是最小的。

3. IterativePruneTopk()函数内的 while (l < max_length) 由 <= 改成了 <，因为看论文中算法2中是<

4. Init_NP()函数内的 for (int tl = 1; tl <= max_l; ++tl) 由 < 改成了 <=，max_length=3则NP_l的大小为4

5. 增加了一个简单的结果保存功能

6. 在IterativePruneTopk()函数结尾，增加了对Sim的遍历来保存K的功能。




存在的问题：
get_second_possibility()函数，应该将times参数传进来。测一下在大图上的该函数耗时。
    - 小图、中图和大图可能不一样
算super node，看定义。遍历一遍NP


