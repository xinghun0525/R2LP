g++ -std=c++11 -o check_error check_error.cpp
./check_error CG 100000 2 2>&1 > ./log/eps_query/CG/times100000-l2-acc.log