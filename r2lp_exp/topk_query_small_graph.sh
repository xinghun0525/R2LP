# threshold="0.1"
cd code
rm main_topk_small_graph
make main_topk_small_graph
# fn=("SD" "DB" "WF" "ND")
fn=("CG")
topk=("1000" "5000" "10000")
# eps=("0.05" "0.01" "0.005" "0.001")
#  "0.95" "0.99")
for i in ${fn[*]}; do
    mkdir -p ../r2lplog/topk_query/$i
    for k in ${topk[*]}; do
        echo "$i $k start!!"
        ./main_topk_small_graph $i $k 2>&1 > ../r2lplog/topk_query/$i/topk-$k.log
        echo "$i $k finished!!"
    done

done