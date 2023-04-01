make thres_metrics_small_graph
fn=("CG" "WV" "EN" "SD")
thres=("0.1" "0.01" "0.001")
rou=("0.9" "0.95" "0.99")

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


