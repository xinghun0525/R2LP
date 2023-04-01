make eps_metrics_small_graph
fn=("EN")
# eps=("0.05" "0.01" "0.005" "0.001")
eps=("7338" "14676" "22015" "29353") 
#  "0.00001")
algo_name=("UISim")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/eps_query/$a/$i/
        for j in ${eps[*]}; do
            echo "$a $i $j start!!"
            ./eps_metrics_small_graph $i $j $a  2>&1 > ./log/eps_query/$a/$i/eps$j.log
            echo "$a $i $j finished!!"
        done
    done
done


