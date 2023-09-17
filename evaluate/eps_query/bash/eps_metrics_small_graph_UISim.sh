make eps_metrics_small_graph_UISim

fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
# hubs 20%
eps=("1048" "1423" "7338" "16433" "13112" "63416" "65145" "754953" "969514" "357276")
# hubs 50%
# eps=("2620" "3557" "18345" "16433 41082" "13112 32780" "63416 158540" "65145 162862" "754953 1887384" "969514 2423785" "357276 893190")

st_rea=("5" "5" "50" "500" "500" "500" "500" "500" "500" "500")
algo_name=("UISim")
eta=("2")

for a in ${algo_name[*]}; do
    for((i=0;i<7;i++)); do
        mkdir -p ./log/eps_query/$a/${fn[$i]}/
        for j in ${eps[$i]}; do
            for sr in ${st_rea[$i]}; do
                for e in ${eta[*]}; do
                    echo "$a ${fn[$i]} $j $sr $e start!!"
                    ./eps_metrics_small_graph_UISim ${fn[$i]} $j $a $sr $e   2>&1 > ./log/eps_query/$a/${fn[$i]}/$e-$sr-eps$j-thres0.01.log
                    echo "$a ${fn[$i]} $j $sr $e finished!!"
                done
            done
        done
    done
done