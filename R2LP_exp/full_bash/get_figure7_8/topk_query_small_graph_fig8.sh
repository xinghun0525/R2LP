# threshold="0.1"
cd code
rm main_thres_small_graph_fig8
make main_thres_small_graph_fig8
fn=("CG")
#  "WV" "EN" "SD" "DB" 
# fn=("WF" "ND")
# fn=("EN")
# thres=("0.05")
topk=("5000")
rou=("0.9")
for i in ${fn[*]}; do
    for j in ${topk[*]}; do
        mkdir -p ../r2lplog/topk_query_fig8
        for k in ${rou[*]}; do
            echo "$i $j $k start!!"
            ./main_thres_small_graph_fig8 $i $j $k 2>&1 > ../r2lplog/topk_query_fig8/$i-topk-$j-rou-$k.log
            echo "$i $j $k finished!!"
        done
    done
done