g++ -march=core2 -pthread -std=c++11 -DSFMT_MEXP=607 -I SFMT-src-1.4.1 -O3 -o KSimJoin_topk SFMT-src-1.4.1/SFMT.c main_topk_query.cpp



fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
max_length=("2")  # "3" "4"
times=("10000")
eps=("0.01")

for((i=0;i<2;i++)); do
    mkdir -p ./log/topk_query/${fn[$i]}/
    mkdir -p ./result/${fn[$i]}/
    for t in ${times[*]}; do
        for e in ${eps[*]}; do
            for ml in ${max_length[*]}; do
                echo "KSimJoin_topk ${fn[$i]} times-$t eps-$e max_length-$ml start!!"
                ./KSimJoin_topk ${fn[$i]} $t $ml $e  2>&1 > ./log/topk_query/${fn[$i]}/times$t-l$ml-eps$e.log
                echo "KSimJoin_topk ${fn[$i]} times-$t eps-$e max_length-$ml finished!!"
            done
        done
    done
done