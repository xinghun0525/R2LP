g++ -std=c++11 -o check_top_acc check_top_acc.cpp

# ./check_top_acc SD 500

# fn=("CG" "WV" "EN")
# fn=("CG" "WV" "EN" "SD" "WF")
fn=("DB" "ND")
# topk=("10000")
topk=("500" "1000" "5000" "10000")

for i in ${fn[*]}; do
    for j in ${topk[*]}; do
        mkdir -p log/topk_acc
        echo "$i $j $k start!!"
        ./check_top_acc $i $j $k 2>&1 > log/topk_acc/$i-topk$j.log
        echo "$i $j $k finished!!"
    done
done