cd Local-Push/build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j
fn=("LJ")
# fn=("DB" "wiki_fy" "ND")

rou=("0.7" "0.8" "0.9")
#  "0.95" "0.97")
thres=("0.1" "0.05")
#  "0.01" "0.005")
for i in ${fn[*]}; do
    for k in ${rou[*]}; do
        for j in ${thres[*]}; do
            mkdir -p ../log/thres_query/$i-t$j
            echo "$i $j $k start!!"
            ./rlp $i $j $k 2>&1 > ../log/thres_query/$i-t$j/r_$k.log
            echo "$i $j $k finished!!"
        done
    done
done


