rm check_basic_topk_large
g++ -std=c++11 check_basic_topk_large.cpp -o check_basic_topk_large
#fn=("WZ")
fn=("CP LJ WZ")
#fn=("LJ WZ")
topk=("500 1000 5000 10000")
algo_name=("rbs")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./acclog/eps_query/$a/$i/
        for k in ${topk[*]}; do
                echo "$a $i top_$k start!!"
                ./check_basic_topk_large $i $k $a  2>&1 > ./acclog/eps_query/$a/$i/top_$k.log
                echo "$a $i top_$k finished!!"
        done
    done
done


