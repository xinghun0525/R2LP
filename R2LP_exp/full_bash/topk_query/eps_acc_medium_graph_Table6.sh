rm check_basic_topk_medium
g++ -std=c++11 check_basic_topk_medium.cpp -o check_basic_topk_medium
fn=("EN")
#fn=("SD DB wiki_fy ND")
topk=("500 1000 5000 10000")
algo_name=("rbs")
# algo_name=("rbs" "optlp")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./acclog/eps_query/$a/$i/
        for k in ${topk[*]}; do
                echo "$a $i top_$k start!!"
                ./check_basic_topk_medium $i $k $a  2>&1 > ./acclog/eps_query/$a/$i/top_$k.log
                echo "$a $i top_$k finished!!"
        done
    done
done


