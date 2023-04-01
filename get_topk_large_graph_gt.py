import json
import sys
import os
filename = 'CP'
sim = dict()
path = '/data/geQian/packing_gq/ExactSim-master/results/' + filename + '/1e-06/'
for idx in os.listdir(path):
    # print(idx)
    with open(path + idx, 'r') as f:
        lines = f.readlines()
    a = int(idx.split('.')[0])
    for line in lines:
        line = line.strip()
        ls = line.split(' ')
        b = int(ls[0])
        simrank = float(ls[1])
        if simrank > 1e-4:
            if a == b:
                continue
            if a > b:
                sim[(a, b)] = simrank
            else:
                sim[(b, a)] = simrank

sim_list = sorted(sim.items(), key = lambda kv:(kv[1], kv[0][0], kv[0][1]), reverse=True)
for k,v in sim_list:
    print(k, v)
    break
print(len(sim))
# with open('')

topk_ls = [500, 1000, 5000, 10000]
output_path = './large_graph_gt/'
for topk in topk_ls:
    with open(output_path + filename + 'top' + str(topk), 'w') as f:
        for i in range(topk):
            f.write(str(sim_list[i][0][0]) + ' ' + str(sim_list[i][0][1]) + ' ' + str(sim_list[i][1]) + '\n')
        lst_sim = sim_list[i][1]
        for i in range(topk,len(sim_list)):
            if sim_list[i][1] == lst_sim:
                f.write(str(sim_list[i][0][0]) + ' ' + str(sim_list[i][0][1]) + ' ' + str(sim_list[i][1]) + '\n')
            else:
                break
