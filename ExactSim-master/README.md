# ExactSim: Exact Single-Source SimRank Computation on Large Graphs
Authors: Hanzhi Wang (hanzhi_wang@ruc.edu.cn), Zhewei Wei (zhewei@ruc.edu.cn)

Please cite our paper if you choose to use our code.
```
@inproceedings{10.1145/3318464.3389781,
author = {Wang, Hanzhi and Wei, Zhewei and Yuan, Ye and Du, Xiaoyong and Wen, Ji-Rong},
title = {Exact Single-Source SimRank Computation on Large Graphs},
year = {2020},
isbn = {9781450367356},
publisher = {Association for Computing Machinery},
address = {New York, NY, USA},
url = {https://doi.org/10.1145/3318464.3389781},
doi = {10.1145/3318464.3389781},
booktitle = {Proceedings of the 2020 ACM SIGMOD International Conference on Management of Data},
pages = {653–663},
numpages = {11},
keywords = {SimRank, exact computation, ground truths},
location = {Portland, OR, USA},
series = {SIGMOD ’20}
}
```

## Tested Environment:
- Ubuntu 16.04.10
- C++ 11
- GCC 5.4.0


## Compile the code:
```
make
```


## Parameters:
- -d \<dataset\> 
- -f \<filelabel\>
- -algo \<algorithm\>
- [-e \<epsilon\> (default 0.001)]
- [-qn \<querynum\> (default 50)]
- [-c \<damping factor\> (default 0.6)]


## Run the example:
(1) Generate query nodes:   
```
./EXSim -d dataset/toy_graph.txt -f toy_graph -algo GEN_QUERY -qn 50
./EXSim -d dataset/CG.txt -f CG -algo GEN_QUERY -qn 5242
./EXSim -d dataset/CH.txt -f CH -algo GEN_QUERY -qn 9877
./EXSim -d dataset/PG.txt -f PG -algo GEN_QUERY -qn 8717
./EXSim -d dataset/WV.txt -f WV -algo GEN_QUERY -qn 7115
```
(2) ExactSim algorithm: 
```
./EXSim -d dataset/toy_graph.txt -f toy_graph -algo ExactSim -e 1.0e-05 -qn 50
```
./EXSim -d dataset/CG.txt -f CG -algo ExactSim -e 1.0e-05 -qn 5242
./EXSim -d dataset/CH.txt -f CH -algo ExactSim -e 1.0e-05 -qn 9877
./EXSim -d dataset/PG.txt -f PG -algo ExactSim -e 1.0e-05 -qn 8717
./EXSim -d dataset/WV.txt -f WV -algo ExactSim -e 1.0e-05 -qn 7115

./EXSim -d ../dataset/DB.txt -f DB -algo ExactSim -e 1.0e-06 -qn 100

## Instructions:
(1) datatset/: datasets files.  
(2) query/: query nodes files.  
(3) results/: SimRank results files (only output nonzero SimRank values).  
