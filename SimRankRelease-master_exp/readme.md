These code are modified from https://github.com/KeithYue/SimRankRelease, and you need to build environment according to the https://github.com/KeithYue/SimRankRelease. Such as `libunwind` and `gperftools` to satisfy `-ltmalloc`. 


Need to copy dataset to `SimRankRelease-master_exp/dataset/edge_list` and remove the first line.


See `SimRankRelease-master_exp/dataset/edge_list/CG.txt` and `dataset/CG.txt` as an example.


for different queries, need to get into path `SimRankRelease-master_exp/Local-Push/playground/` and  rename `main_exec_reduced_local_push_topk_*.cpp` (* is the name of task, choice from {'eps', 'thres', 'topk'}) into `main_exec_reduced_local_push.cpp` and remake.

all shell scripts are under `full_bash` path.