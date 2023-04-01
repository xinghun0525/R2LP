# threshold="0.1"
cd code
make main_eps_small_graph
fn=("CG" "WV" "EN" "SD")
# fn=("WF")
eps=("0.1" "0.01" "0.001")
#  "0.95" "0.99")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_small_graph $i $k 2>&1 > ../rbslog/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done

done