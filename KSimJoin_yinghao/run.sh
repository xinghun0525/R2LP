g++ -march=core2 -pthread -std=c++11 -DSFMT_MEXP=607 -I SFMT-src-1.4.1 -O3 -o KSimJoin SFMT-src-1.4.1/SFMT.c main.cpp

fn=("CG" "WV")
#  "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
max_lengths=("2")
#  "4")
times=("100000")
e=("0.01")  # eps没啥用啊
for t in ${times[*]}; do
    for f in ${fn[*]}; do
    # 创建log文件夹和结果文件夹
        mkdir -p ./log/eps_query/$f/
        mkdir -p ./result/$f/
        for max_length in ${max_lengths[*]}; do
            echo "KSimJoin $f times-$t eps-$e max_length-$max_length start!!"
            ./KSimJoin $f $t $max_length $e  2>&1 > ./log/eps_query/$f/times$t-l$max_length-eps$e.log
            echo "KSimJoin $f times-$t eps-$e max_length-$max_length finished!!"
        done
    done
done