make eps_metrics_large_graph
# fn=("CP" "LJ" "WZ")
fn=("CP" "WZ" "LJ")
eps=("0.1" "0.05" "0.01")
# eps=("0.1" "0.05" "0.01" "0.005")
algo_name=("flp")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/eps_query/$a/$i/
            for j in ${eps[*]}; do
                echo "$a $i $j start!!"
                ./eps_metrics_large_graph $i $j $a  2>&1 > ./log/eps_query/$a/$i/eps$j.log
                echo "$a $i $j finished!!"
            done
    done
done


