this code is for experiment of Table 6 in our paper.

bash eps_query_small_graph_Table4_*.sh # calculate R2LP's deterministic and randomized push number in terms of edge traversal with or without pruning for small and middle graph
bash eps_query_large_graph_Table4_*.sh # calculate R2LP's deterministic and randomized push number in terms of edge traversal with or without pruning for large graph

bash eps_query_*_graph_topk_Table6.sh # Run the R2LP algorithm for small, middle and large graph
bash eps_acc_*_graph_Table6.sh # Calculate the  practical accuracy of R2LP algorithm for small, middle and large graph

bash gene_topk_medium_graph.sh # Generates the groundtruth of the selected node set in middle graph
bash gt_sort.sh # Sort the groundtruth values
