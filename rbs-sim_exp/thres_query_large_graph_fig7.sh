# threshold="0.1"
cd code
rm main_thres_small_graph_fig7
make main_thres_small_graph_fig7
# fn=("CG" "WV" "EN" "SD" "DB" "WF" "ND")
fn=("CP" "LJ" "WZ")
thres=("0.05")
rou=("0.9")
for i in ${fn[*]}; do
    for j in ${thres[*]}; do
        mkdir -p ../rbslog/thres_query_fig7
        for k in ${rou[*]}; do
            echo "$i $j $k start!!"
            ./main_thres_small_graph_fig7 $i $j $k 2>&1 > ../rbslog/thres_query_fig7/$i-thres-$j-rou-$k.log
            echo "$i $j $k finished!!"
        done
    done
done