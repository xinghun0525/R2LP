import matplotlib.pyplot as plt
import time

small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "WF", "ND", "DB"]
large_graph = ["CP", "LJ", "WZ"]

total_graph = small_graph + middle_graph
x_beta_list = [0.67, 0.72, 0.32, 0.38, 0.48, 0.27, 0.42, 0.76, 0.8, 0.51]
x_beta_list = [1.063, 0.495, 1.303, 1.098, 1.098, 0.964, 0.866, 0.76, 0.8, 0.51]

normal_para = [105620.81, 408951.89, 10582263.72, 8481857.14, 5607254.9, 252541815.1, 8261232.11]

y_query_time = [0.402857, 5.09463, 4.5926, 30.1782, 8.41731, 1273.01, 23.9921, 41.1441, 272.876, 84.1635]
y_query_time = [y_query_time[i] / normal_para[i] for i in range(len(small_graph+middle_graph))]
nor = y_query_time[0]
y_query_time = [a / nor for a in y_query_time]

marker = ['o', '^', 'd', 's', '>', '<', 'p', 'v', 'H', '*']
color = ['green', 'royalblue', 'red', 'darkviolet', 'magenta', 'orange', 'darkgoldenrod', 'gold', 'k', 'k']

xlabel = r'$\beta$'
ylabel = 'Normalized Query Time'
title = "fig10_"
savedpath = './fig10/'

fontsize = 60
plt.figure(figsize=(24, 18))
plt.xlabel(xlabel, fontsize=fontsize)
plt.ylabel(ylabel, fontsize=fontsize)

plt.yticks(size=fontsize)
plt.xticks(size=fontsize)
plt.tick_params(axis='x', pad=20)

plt.subplots_adjust(bottom=0.15)
plt.subplots_adjust(left=0.20)

ax = plt.gca()
ax_width = 4
ax.spines['bottom'].set_linewidth(ax_width)
ax.spines['left'].set_linewidth(ax_width)  
ax.spines['right'].set_linewidth(ax_width)
ax.spines['top'].set_linewidth(ax_width)


plt.grid(linewidth=3)
plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)

lineweight = 7.0
markersize = 50
marker_w = 5

for i in range(len(total_graph)):
    plt.plot([x_beta_list[i]], [y_query_time[i]], color=color[i], marker=marker[i], linestyle='', linewidth=lineweight,
             markersize=markersize, markerfacecolor='none', markeredgewidth=marker_w, clip_on=False)


t_s = time.time()
f = plt.gcf()
respath = savedpath + "fig10a.png"
f.savefig(respath)
f.clear()
print(f"saved {respath}, cost {time.time() - t_s}s")





