make eps_metrics_small_graph_rate
fn=("CG")
eps=("0.0001")
# "0.0001")
threshold=("0.25")
algo_name=("rbs")
# algo_name=("rbs" "optlp")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/eps_query/$a/$i/
        for k in ${threshold[*]}; do
            for j in ${eps[*]}; do
                echo "$a $i $k $j start!!"
                ./eps_metrics_small_graph_rate $i $k $j $a  2>&1 > ./log/eps_query/$a/$i/t$k-eps$j.log
                echo "$a $i $j finished!!"
            done
        done
    done
done


