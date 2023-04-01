# threshold="0.1"
cd code
make main_thres_large_graph
fn=("LJ")
thres=("0.05")
rou=("0.8")
for i in ${fn[*]}; do
    for j in ${thres[*]}; do
        mkdir -p ../r2lplog/thres_query/$i-thres_$j
        for k in ${rou[*]}; do
            echo "$i $j $k start!!"
            ./main_thres_large_graph $i $j $k 2>&1 > ../r2lplog/thres_query/$i-thres_$j/rou-$k.log
            echo "$i $j $k finished!!"
        done
    done
done