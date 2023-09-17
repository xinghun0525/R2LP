cd ../../code
make main_thres_large_graph
fn=("CP" "LJ" "WZ")
thres=("0.1" "0.05" "0.01")
rou=("0.7" "0.8" "0.9")
for i in ${fn[*]}; do
    for j in ${thres[*]}; do
        mkdir -p ../log/thres_query/$i-thres_$j
        for k in ${rou[*]}; do
            echo "$i $j $k start!!"
            ./main_thres_large_graph $i $j $k 2>&1 > ../log/thres_query/$i-thres_$j/rou-$k.log
            echo "$i $j $k finished!!"
        done
    done
done