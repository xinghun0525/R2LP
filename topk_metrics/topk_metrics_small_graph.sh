make check_topk_small
fn=("CG")
# eps=("0.0001")
topk=("1000" "5000" "10000")
# "0.0001")
# threshold=("0.25")
algo_name=("rbs")
# algo_name=("rbs" "optlp")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/topk_query/$a/
        for k in ${topk[*]}; do
                echo "$a $i $k start!!"
                ./check_topk_small $i $k $a  2>&1 > ./log/topk_query/$a/topk-$k.log
                echo "$a $i $k finished!!"
            
        done
    done
done


