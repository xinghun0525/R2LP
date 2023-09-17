bash eps_query_small_graph.sh  # 对小图和中图进行r2lp实验，替换掉对应的文件名thres和eps即可
bash eps_query_large_graph.sh  # 对三个大图进行r2lp实验，替换掉对应的文件名thres和eps即可

bash thres_query_small_graph_fig*.sh # 对相应图做实验得到fig7、fig8的数据

bash topk_query_small_graph.sh # 对小图进行topk实验

bash eps_query_small_graph_Table4_*.sh # calculate R2LP's deterministic and randomized push number in terms of edge traversal with or without pruning for small and middle graph
bash eps_query_large_graph_Table4_*.sh # calculate R2LP's deterministic and randomized push number in terms of edge traversal with or without pruning for large graph

bash eps_query_*_graph_topk_Table6.sh # Run the R2LP algorithm for small, middle and large graph
bash eps_acc_*_graph_Table6.sh # Calculate the  practical accuracy of R2LP algorithm for small, middle and large graph
