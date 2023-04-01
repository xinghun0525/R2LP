# threshold="0.1"
cd code
make main_eps_small_graph
fn=("SD" "DB" "WF" "ND")
eps=("0.1" "0.05" "0.01" "0.005")
#  "0.95" "0.99")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_small_graph $i $k 2>&1 > ../rbslog/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done

done