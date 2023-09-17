make
fn=("WZ" "LJ" "CP")

# set only store sim >= 0.001
for i in ${fn[*]}; do
    cp ../ground_idx/$i/idx1 query/$i.query
    ./EXSim -d ../dataset/$i.txt -f $i -algo ExactSim -e 1.0e-06 -qn 100
done