cd Local-Push/build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j
# fn=("SD" "DB" "WF" "ND")
fn=("CP" "LJ" "WZ")

# topk=("500")
topk=("500" "1000" "5000" "10000")
#  "0.95" "0.97")
eps=("5e-3")
#  "0.01" "0.005")
for i in ${fn[*]}; do
    for j in ${topk[*]}; do
        for k in ${eps[*]}; do
            mkdir -p ../log/topk_query/$i-topk$j
            echo "$i $j $k start!!"
            ./rlp $i $j $k 2>&1 > ../log/topk_query/$i-topk$j/eps_$k.log
            echo "$i $j $k finished!!"
        done
    done
done


