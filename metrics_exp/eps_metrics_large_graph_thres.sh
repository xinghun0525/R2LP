make eps_metrics_large_graph_thres
fn=("CP" "LJ" "WZ")
eps=("0.1" "0.05" "0.01")
thres=("0.1" "0.01" "0.001")
# eps=("0.00001")
#  "0.00001")
algo_name=("rbs" "optlp")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        for j in ${eps[*]}; do
            mkdir -p ./log/eps_query_thres_figure5/$a/$i/
            for k in ${thres[*]}; do
                echo "$a $i $j $k start!!"
                ./eps_metrics_large_graph_thres $i $j $a $k 2>&1 > ./log/eps_query_thres_figure5/$a/$i/eps$j-t-$k.log
                echo "$a $i $j $k finished!!"
            done
        done
    done
done


