make check_topk_graph_UISim


fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
eps=("1048" "1423" "7338" "16433" "13112" "63416" "65145" "754953" "969514" "357276")
st_rea=("1" "1" "10" "10" "10" "10" "10" "10" "10" "10")
eta=("2")
algo_name=("UISim")
topk=("500" "1000" "5000" "10000")

# UISim
for a in ${algo_name[*]}; do
for((i=0;i<10;i++)); do
    mkdir -p ./log/topk_query/$a/${fn[$i]}/
    for j in ${eps[$i]}; do
        for sr in ${st_rea[$i]}; do
            for e in ${eta[*]}; do
                for tk in ${topk[*]}; do
                echo "$a ${fn[$i]} $j $sr $tk $e start!!"
                ./check_topk_graph ${fn[$i]} $j $a $sr $tk $e  2>&1 > ./log/topk_query/$a/${fn[$i]}/eta$e-topk$tk-sr$sr.log
                echo "$a ${fn[$i]} $j $sr $tk $e finished!!"
                done
            done
        done
    done
done
done

# KSimJoin
fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
eps=("0.01" "0.01" "0.01" "0.01" "0.01" "0.01" "0.01" "0.01" "0.01" "0.01")
algo_name=("KSimJoin")
topk=("500" "1000" "5000" "10000")
max_length=("2")
times=("100000")

for a in ${algo_name[*]}; do
    for((i=0;i<2;i++)); do
        mkdir -p ./log/topk_query/$a/${fn[$i]}/
        for j in ${eps[$i]}; do
            for sr in ${times[*]}; do
                for e in ${max_length[*]}; do
                    for tk in ${topk[*]}; do
                        echo "$a ${fn[$i]} $j $sr $tk $e start!!"
                        ./check_topk_graph ${fn[$i]} $j $a $sr $tk $e  2>&1 > ./log/topk_query/$a/${fn[$i]}/max_length$e-topk$tk-times$sr.log
                        echo "$a ${fn[$i]} $j $sr $tk $e finished!!"
                    done
                done
            done
        done
    done
done


