g++ -march=core2 -pthread -std=c++11 -DSFMT_MEXP=607 -O3 -o fit_b_beta_c main.cpp

./fit_b_beta_c 2>&1 > ./log/out.log