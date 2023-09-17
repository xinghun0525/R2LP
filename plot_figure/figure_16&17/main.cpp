#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <cmath>
#include <unordered_map>

using namespace std;

double MAXNUM = 1e30;
string filename = "CG";

// "Record the information for each round of beta."
struct bbbeta_info
{
    double b1 = 1, b2 = MAXNUM, beta = MAXNUM;
    unordered_map<long, double> U_x_beta;
    bbbeta_info() {}
};


void fit_bbbeta(vector<double> slist, long nnz, int x_start_i)
{
    clock_t t1 = clock();

    // "First, calculate all S_x values."
    unordered_map<long, double> S_x;
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        long j_start = pow(2, x_);
        long j_end = min(2 * j_start - 1, nnz);
        S_x[x_] = 0.0;
        for (long j = j_start; j <= j_end; j++)
        {
            S_x[x_] += slist[j];
        }
        S_x[x_] /= double(j_start);
    }

    // "Record the beta info for which b2/b1 is the smallest."
    bbbeta_info stru_info;

    for (long beta_ = 1; beta_ <= 2000; beta_++)
    {
        double beta = double(beta_) / 1000.0;
        // store max(1, j-t)^(-beta)
        unordered_map<long, double> U_x_beta;
        // "First, calculate U_x_beta using integration and then calculate the gap size."
        double cur_gap = 0.0;
        for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
        {
            long j_start = pow(2, x_-x_start_i+1);
            long j_end = min(2 * j_start - 1, nnz);
            long jifen_start = j_start;

            if (j_start == 1)
            {
                U_x_beta[x_] = 1.0;
            }
            else
            {
                if (beta >= 1.0 - 1e-7 && beta <= 1.0 + 1e-7)
                { // beta=1
                    U_x_beta[x_] = log(j_end) - log(jifen_start);
                }
                else
                {
                    U_x_beta[x_] = pow(1 - beta, -1) * (pow(j_end, 1 - beta) - pow(jifen_start, 1 - beta));
                }
            }
            U_x_beta[x_] /= double(j_start);
        }

        // get b1
        double best_b1 = MAXNUM;
        for (long b1_ = 1; b1_ <= 5000; b1_++)
        {
            double b1 = double(b1_) / 1000.0;
            double b1_gap = 0.0;
            for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
            {
                double cur_gap1 = S_x[x_] - b1 * U_x_beta[x_];
                if (cur_gap1 >= 0)
                    b1_gap += cur_gap1;
                else
                {
                    b1_gap = MAXNUM;
                    break;
                }
            }
            if (b1_gap != MAXNUM)  // get max b1
            {
                best_b1 = b1;
            }
        }
        // get b2
        double best_b2 = MAXNUM;
        for (long b2_ = 1; b2_ <= 50000; b2_++)
        {
            double b2 = double(b2_) / 1000.0;
            double b2_gap = 0.0;
            for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
            {
                double cur_gap2 = b2 * U_x_beta[x_] - S_x[x_];
                if (cur_gap2 >= 0)
                    b2_gap += cur_gap2;
                else
                {
                    b2_gap = MAXNUM;
                    break;
                }
            }
            if (b2_gap != MAXNUM)  // get min b2
            {
                best_b2 = b2;
                break;
            }
        }

        if(best_b1 != MAXNUM && best_b2 != MAXNUM){
            if(best_b2 / best_b1 < stru_info.b2/ stru_info.b1){
                stru_info.b1 = best_b1;
                stru_info.b2 = best_b2;
                stru_info.beta = beta;
                stru_info.U_x_beta = U_x_beta;
            }
        }

        if (beta_ % 100 == 0) // "Output the information every 100 rounds."
        {
            // log U_x_beta[x_]
            cout << "U_x_beta: \n[";
            for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
            {
                cout << U_x_beta[x_] << ", ";
            }
            cout << "]"<< endl;
            cout << "epoch：" << beta_ << " finished";
            cout << ", cur_beta: " << beta;
            cout << ", cur best beta: " << stru_info.beta;
            cout << ", cur best b1: " << stru_info.b1;
            cout << ", cur best b2: " << stru_info.b2;
            cout << ", total cost " << (clock() - t1) / (double)CLOCKS_PER_SEC << " s\n" << endl;
        }
    }

    // log best_beta
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << "best_U_x_beta[" << x_ << "]: " << stru_info.U_x_beta[x_] << ", ";
    }
    cout << "===============================" << endl;
    cout << " run finished. total cost " << (clock() - t1) / (double)CLOCKS_PER_SEC << " s" << endl;
    cout << "best b1 is " << stru_info.b1;
    cout << ", best b2 is " << stru_info.b2;
    cout << ", best beta is " << stru_info.beta<<endl;

    // "Save the results for easy plotting."
    // log x_
    cout << "x_: " << endl;
    cout << "[";
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << x_ << ", ";
    }
    cout << "]"<< endl;
    // log S_x[x_]
    cout << "S_x: " << endl;
    cout << "[";
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << S_x[x_] << ", ";
    }
    cout << "]"<< endl;
    // log best_U_x_beta
    cout << "best_U_x_beta: " << endl;
    cout << "[";
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << stru_info.U_x_beta[x_] << ", ";
    }
    cout << "]"<< endl;
    // log b1* U_x
    cout << "b1* U_x: " << endl;
    cout << "[";
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << stru_info.b1*stru_info.U_x_beta[x_] << ", ";
    }
    cout << "]"<< endl;
    // log b1* U_x
    cout << "b2* U_x: " << endl;
    cout << "[";
    for (long x_ = x_start_i; x_ <= long(log2(nnz)); x_++)
    {
        cout << stru_info.b2*stru_info.U_x_beta[x_] << ", ";
    }
    cout << "]\n\n"<< endl;


    return;
}

int main()
{
    // "The SimRank results sorted in ascending order."
    string rootpath = "../middle_file/";
    vector<string> filelist = {"CG", "WV", "EN", "SD", "wiki_fy", "ND", "DB"};

    // "Record the starting value of x_, and ignore numbers where simrank > 0.1."
    vector<int> x_start = {9, 2, 17, 17, 16, 22, 15};
    // "Record the ending value of x_. Only save the data in the range [x_start, x_end]."
    vector<int> x_end = {-1, -2, -4, -1, -1, 0, -1};

    for (long i = 0; i < filelist.size(); i++)
    {
        clock_t t1 = clock();
        string infilepath = rootpath + filelist[i];
        filename = filelist[i];

        ifstream infile(infilepath);
        vector<double> slist;

        double svalue;
        while (infile.good())
        {
            infile >> svalue;
            if (svalue >= 0.6 - 1e-7)
                continue;
            slist.emplace_back(svalue);
        }
        infile.close();

        long nnz = slist.size();
        cout << "\nold nnz is " << nnz << endl;
        
        // "Sort in descending order."
        reverse(slist.rbegin(), slist.rend());
        slist.insert(slist.begin(), 0);

        long x_ = long(log2(nnz)) + x_end[i];
        long split_start = pow(2, x_);
        slist.erase(slist.begin() + split_start, slist.end());

        nnz = slist.size() - 1;
        cout << "new nnz is " << nnz <<", slist[nnz]: " << slist[nnz] << ", reading in " << infilepath << " graph takes " << (clock() - t1) / (double)CLOCKS_PER_SEC << " s" << endl;
        t1 = clock();

        fit_bbbeta(slist, nnz, x_start[i]);
    }

    return 0;
}