Code Contributors: Qian Ge, Yu Liu, Yinghao Zhao, Yuetian Sun

If you have any questions, feel free to contact us. Our emails are: geqian@pku.edu.cn

Plase cite our papers if you choose to use our code.

> Qian Ge+, Yu Liu+*, Yinghao Zhao, Yuetian Sun, Lei Zou, Yuxing Chen, and Anqun Pan. Efficient and Accurate SimRank-based Similarity Joins: Experiments, Analysis, and Improvement. Accepted by VLDB 2024.

`dataset` save the needed graph dataset, we store the three small dataset in this github link, and the other medieum and large datasets can be downloaded from https://drive.google.com/drive/folders/1u99TgRftbVKoZD04f7kI5exvKuBWZ8gM?usp=drive_link under the `dataset`. 

`r2lp_exp` is the dataloader for our proposed algorithm `R2LP`.

`ground_idx` the index node selected for medieum graph and large graph

`ExactSim-master`  codes got from https://github.com/wanghzccls/ExactSim

`SimRankRelease-master_exp` codes modified from https://github.com/KeithYue/SimRankRelease

`UISim202-main` codes modified from https://github.com/UISim2020/UISim2020, 

`threshold_metrics` check accuracy of threshold query

`topk_metrics` check accuracy of topk query

`gperftools` is needed when facing the rlp/flp, you may reference on https://github.com/KeithYue/SimRankRelease