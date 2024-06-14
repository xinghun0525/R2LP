# Efficient and Accurate SimRank-based Similarity Joins: Experiments, Analysis, and Improvement
------------
+ This is the implementation of our paper: Qian Ge, Yu Liu, Yinghao Zhao, Yuetian Sun, Lei Zou, Yuxing Chen, Anqun Pan. Efficient and Accurate SimRank-based Similarity Joins: Experiments, Analysis, and Improvement. Accepted by VLDB 2024.
+ Code Contributors: Qian Ge, Yinghao Zhao, Yuetian Sun
+ If you have any questions, feel free to contact us. Our emails are: geqian@pku.edu.cn, {yuetiansun, yul}@bjtu.edu.cn

## Tested Environment

- Ubuntu server (with an Hygon C86 7151 processor and 1TB memory)
- C++ 11
- GCC 4.8

## Dataset

We have included three small datasets in the `dataset` directory. For medium and large datasets, they can be downloaded from https://drive.google.com/drive/folders/1u99TgRftbVKoZD04f7kI5exvKuBWZ8gM?usp=drive_link under the `dataset` folder.

## Code Structure
The project directory is organized as follows:

+ `dataset`:  Contains essential graph datasets.
+ `R2LP_exp`: Provides a data loader for our proposed algorithm, `R2LP`.
+ `ground_idx`: Stores index nodes selected for medium and large graphs.
+ `ExactSim-master`: Codebase sourced from [ExactSim](https://github.com/wanghzccls/ExactSim).
+ `SimRankRelease-master_exp`: Codebase modified from [SimRankRelease](https://github.com/KeithYue/SimRankRelease).
+ `UISim202-main`: Codebase modified from [UISim2020](https://github.com/UISim2020/UISim2020).
+ `KSimJoin`: This section details the replication of the KSimJoin algorithm based on the research paper.
+ `evaluate`: The evaluation code for different algorithms under different datasets.


## Run the Code
### **1.Prepare the dataset**

   Store the required graph datasets in the `dataset` folder. You can choose one from the existing datasets or use your own dataset as needed.

### **2.Determine the index node**

   Select an appropriate index from the `ground_idx` folder to be used for experiments with medium and large graphs.

### **3.Build the ground truth**

   + For small graphs and medium graphs, get into `SimRankRelease-master_exp/full_bash` and run 

      ```shell
      bash generate_groundtruth.sh
      ```

      and the generated ground truth is in `SimRankRelease-master_exp/Local-Push/ground_truth`.

   + For large graphs, get into `ExactSim-master` and run

      ```shell
      bash run.sh
      ```

      and the generated ground truth is in `ExactSim-master/results`.

### **4.Run the experiments in our paper**

   You can run experiments from the following folders:

   - `R2LP_exp`: Conduct experiments using the `R2LP` algorithm. Follow the bash scripts in `full_bash`.
   - `KSimJoin`: Conduct experiments using the `KSimJoin`, follow the `*.sh`.
   - `SimRankRelease-master_exp`: Conduct experiments using the `Opt-LP` and `FLP` algorithm. Follow the bash scripts in `full_bash`.
   - `UISim202-main`: Conduct experiments using the `Opt-LP` and `FLP` algorithm. Follow the bash scripts in `UISim2020-main/UISim_C++/run.sh`.

### **5.Evaluate**

   You can evaluate the query results from the `evaluate` folder.


### **6.Plot figure**

   You can draw experimental result figures in the `plot_figure` folder.
