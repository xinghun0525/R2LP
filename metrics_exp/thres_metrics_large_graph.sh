make thres_metrics_large_graph
# fn=("CP" "LJ" "WZ")
# thres=("0.1" "0.05" "0.01")
# rou=("0.7" "0.8" "0.9")
fn=("LJ")
thres=("0.05")
rou=("0.8")
algo_name=("rbs")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/thres_query/$a/$i/
        for k in ${thres[*]}; do
            for j in ${rou[*]}; do
                echo "$a $i $k $j start!!"
                ./thres_metrics_large_graph $i $k $j $a  2>&1 > ./log/thres_query/$a/$i/t$k-r$j.log
                echo "$a $i $j finished!!"
            done
        done
    done
done


