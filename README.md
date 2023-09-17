### Introduction
------------
+ Code Contributors: Qian Ge, Yinghao Zhao, Yuetian Sun

+ If you have any questions, feel free to contact us. Our emails are: geqian@pku.edu.cn

+ Plase cite our papers if you choose to use our code.

> Qian Ge+, Yu Liu+*, Yinghao Zhao, Yuetian Sun, Lei Zou, Yuxing Chen, and Anqun Pan. Efficient and Accurate SimRank-based Similarity Joins: Experiments, Analysis, and Improvement. Accepted by VLDB 2024.





### Dataset
----------
The necessary graph datasets are stored within the `dataset` directory. We have included three small datasets in this GitHub repository. For medium and large datasets, they can be downloaded from https://drive.google.com/drive/folders/1u99TgRftbVKoZD04f7kI5exvKuBWZ8gM?usp=drive_link under the `dataset` folder.

### Code Structure
-------------
The project directory is organized as follows:

+ `dataset`:  Contains essential graph datasets.
+ `R2LP_exp`: Provides a dataloader for our proposed algorithm, `R2LP`.
+ `ground_idx`: Stores index nodes selected for medium and large graphs.
+ `ExactSim-master`: Codebase sourced from [ExactSim](https://github.com/wanghzccls/ExactSim).
+ `SimRankRelease-master_exp`: Codebase modified from [SimRankRelease](https://github.com/KeithYue/SimRankRelease).
+ `UISim202-main`: Codebase modified from [UISim2020](https://github.com/UISim2020/UISim2020).
+ `KSimJoin`: This section details the replication of the KSimJoin algorithm based on the research paper.
+ `evaluate`: The evaluation code for different algorithms under different datasets.



### Running Process
------------
#### **1.Prepare the Dataset**

   Store the required graph datasets in the `dataset` folder. You can choose one from the existing datasets or use your own dataset as needed.

#### **2.Determine the Index Node**

   Select an appropriate index from the `ground_idx` folder to be used for experiments with medium and large graphs.

#### **3.Build the groundtruth**

   + For small graphs and medium graphs, get into `SimRankRelease-master_exp/full_bash` and run 

      ```shell
      bash generate_groundtruth.sh
      ```

      and will generate groundtruth in `SimRankRelease-master_exp/Local-Push/ground_truth`.

   + For large graphs, get into `ExactSim-master` and run

      ```shell
      bash run.sh
      ```

      and will generate groundtruth in `ExactSim-master/results`.

#### **4.Run Experiments**

   You can run experiments from the following folders:

   - `R2LP_exp`: Conduct experiments using the `R2LP` algorithm. Following the bash scripts in `full_bash`.
   - `KSimJoin`: Refer to its documentation and execute experiments.
   - `SimRankRelease-master_exp`: Refer to its documentation and execute experiments.
   - `UISim202-main`: Refer to its documentation and execute experiments.
   - `Ksim`

   Following the `full_bash` in `R2LP_exp` and `SimRankRelease-master_exp` to finish all-pair, threshold query and top query.

#### **5.Evaluate**

   You can evaluate the query results from the `evaluate` folders.

