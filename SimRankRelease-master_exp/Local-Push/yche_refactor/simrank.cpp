#include "simrank.h"

void indegree_mat(const GraphYche &g, SparseMatrix<float> &P) {
    // column normalized adjacency matrix
    typedef Triplet<float> T;
    vector<T> tripleList;
    for (auto u = 0; u < g.n; u++) {
        for (auto off_u = g.off_out[u]; off_u < g.off_out[u + 1]; off_u++) {
            tripleList.push_back(T(u, g.neighbors_out[off_u], 1));
        }
    }
    P.setFromTriplets(tripleList.begin(), tripleList.end());
    for (int k = 0; k < P.outerSize(); ++k) { // column normalize the matrix
        auto indeg_k = g.in_deg_arr[k];
        for (SparseMatrix<float>::InnerIterator it(P, k); it; ++it) {
            it.valueRef() = 1.0 / indeg_k;
        }
    }
}

// ground truth for simrank
TruthSim::TruthSim(string name, GraphYche &g, double c_, double epsilon_) {
    g_name = name;
    n = static_cast<size_t >(g.n);
    c = c_;
    epsilon = epsilon_;
    sim.resize(n, n);
    cout << get_file_path() << endl;
    // if (file_exists(get_file_path())) {
    //     cout << "loading ground truth...." << endl;
    //     load();
    // } else {
        cout << "setzero" << endl;
        sim.setZero();
        for (size_t i = 0; i < n; i++) {
            sim(i, i) = 1;
        }
        cout << "finished init" << endl;
        if (n < 1000000) {
            run(g);
            // save();
        }
    // }
    // cout << "simrank matrix size: " << sim.size() << endl;
}

void TruthSim::run(GraphYche &g) {
    // compute the ground truth
    cout << "fget into run " << endl;
    SparseMatrix<float> P(n, n);
    cout << "finished init SparseMatrix" << endl;
    indegree_mat(g, P);
    cout << "finished init P" << endl;
    auto PT = P.transpose();
    // size_t maxIter = size_t(ceil(log(epsilon) / log(c)));
    size_t maxIter = 30; // the groud thruth, iterate enough time
    size_t k = 0;
    cout << "max iterations: " << maxIter << endl;
    while (k < maxIter) {
        cout << "the " << k << "  -th iteration" << endl;
        sim = PT * sim;
        sim = sim * P;
        sim = c * sim;
        for (size_t i = 0; i < n; i++) {
            sim(i, i) = 1;
        }
        k++;
    }

    // cout << "going to compute groundtruth!! " << gt << endl;
    // ofstream fgt;
    // fgt.open(gt.data());
    // assert(fgt.is_open());
    // if(n < 1000000){
    //     TruthSim ts(g_name, g, c, eps);
    //     cout << "finished ts matrix " << endl;
    //     for(int i = 0; i < n; ++i){
    //         for(int j = 0; j <= i; ++j){
    //             float asim = ts.sim(i,j);
    //             if(asim >= 1e-5)
    //                 fgt << i << " " << j << " " << ts.sim(i,j) << endl;
    //         }
    //     }
    // }
    // fgt.close();
    
    // for(int i = 0; i < n; ++i){
    //     for(int j = 0; j < n; ++j){
            
    //     }
    // }
}

#ifdef NAIVE_GROUND_TRUTH
void basic_simrank(GraphYche &g, double c, SimRank_matrix &sim) {
    int K = 50;
    int n = num_vertices(g);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                sim[i][j] = 1;
            } else {
                sim[i][j] = 0;
            }
        }
    }
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    double current = 0; // accumulated current simrank
                    int i_indeg = in_degree(i, g);
                    int j_indeg = in_degree(j, g);
                    GraphYche::in_edge_iterator i_in, i_end;
                    tie(i_in, i_end) = in_edges(i, g);
                    for (; i_in != i_end; i_in++) {
                        GraphYche::in_edge_iterator j_in, j_end;
                        tie(j_in, j_end) = in_edges(j, g);
                        for (; j_in != j_end; j_in++) {
                            auto source_i = source(*i_in, g);
                            auto source_j = source(*j_in, g);
                            current += ((c * sim[source_i][source_j]) / (i_indeg * j_indeg));
                        }
                    }
                    sim[i][j] = current;
                }
            }
        }
    }
}
#endif
