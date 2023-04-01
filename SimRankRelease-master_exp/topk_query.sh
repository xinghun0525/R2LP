cd Local-Push/build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j
fn=("CG" "WV" "EN")
topk=("500" "1000" "5000" "10000")
#  "0.95" "0.97")
eps=("1e-6")
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


