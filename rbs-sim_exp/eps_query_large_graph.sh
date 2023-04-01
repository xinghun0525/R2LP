# threshold="0.1"
cd code
make main_eps_large_graph
fn=("CP" "LJ" "WZ")
eps=("0.005")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_large_graph $i $k 2>&1 > ../rbslog/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done

done