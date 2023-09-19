cd ../../code
make main_thres_small_graph
fn=("SD" "DB" "WF" "ND")
thres=("0.05" "0.01" "0.005")
rou=("0.9" "0.95" "0.97")
for i in ${fn[*]}; do
    for j in ${thres[*]}; do
        mkdir -p ../log/thres_query/$i-thres_$j
        for k in ${rou[*]}; do
            echo "$i $j $k start!!"
            ./main_thres_small_graph $i $j $k 2>&1 > ../log/thres_query/$i-thres_$j/rou-$k.log
            echo "$i $j $k finished!!"
        done
    done
done