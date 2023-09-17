cd Local-Push/playground
cp main_exec_reduced_local_push_eps.cpp main_exec_reduced_local_push.cpp
cd ../build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j

fn=("CP" "LJ" "WZ")
eps=("0.1" "0.05" "0.01")
for i in ${fn[*]}; do
    for k in ${rou[*]}; do
        for j in ${eps[*]}; do
            mkdir -p ../log/eps_query/$i-t$j
            echo "$i $j $k start!!"
            ./rlp $i $j $k 2>&1 > ../log/eps_query/$i-t$j/r_$k.log
            echo "$i $j $k finished!!"
        done
    done
done