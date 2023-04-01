make thres_metrics_small_graph
fn=("SD" "DB" "WF" "ND")
thres=("0.05" "0.01" "0.005")
rou=("0.9" "0.95" "0.97")

algo_name=("rbs")
for a in ${algo_name[*]}; do
    for i in ${fn[*]}; do
        mkdir -p ./log/thres_query/$a/$i/
        for k in ${thres[*]}; do
            for j in ${rou[*]}; do
                echo "$a $i $k $j start!!"
                ./thres_metrics_small_graph $i $k $j $a  2>&1 > ./log/thres_query/$a/$i/t$k-r$j.log
                echo "$a $i $j finished!!"
            done
        done
    done
done


