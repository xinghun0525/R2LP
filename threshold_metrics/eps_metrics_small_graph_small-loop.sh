make eps_metrics_small_graph
fn=("CG")
eps=("0.0005" "0.0001")
algo_name=("rbs")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/eps_query/$a/$i/
            for j in ${eps[*]}; do
                echo "$a $i $j start!!"
                ./eps_metrics_small_graph $i $j $a  2>&1 > ./log/eps_query/$a/$i/t$k-eps$j-loop.log
                echo "$a $i $j finished!!"
            done
        
    done
done


