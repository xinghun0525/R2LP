import matplotlib.pyplot as plt

small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "WF", "ND", "DB"]

total_graph = small_graph+middle_graph

Gap_list = [[0.00807,0.003887, 0.011368, 0.0079175], [0.0034569, 0.0003629, 0.0009711, 0.0007136], [0,0,0.0065904,0.00185413], [0.0076482,0.004869,0.00058402,0.00017142],
            [0.0146263,0.0028063,0.00037259,0.00016516], [0.026841,0.041184,0.0032865,0.0014978], [0.006973,0.014717,0.004925,0.0028187], [0.0042857,0.0052748,0.00125331,0.00064935],
            [0.0090563,0.0033393,0.0015744,0.00076742], [0,0,0,0.0410821]]
y_query_time = [[1.44543, 2.64637, 1.42476, 1.40033], [10.8385, 82.0683, 24.6367, 47.016], [6.96513, 6.90376, 87.9975, 284.901], [160.59, 276.12,2423.79,11563.6],
                [66.4398,506.056,3487.75,6776.76], [5359.89,2211.44,11748.7,23895.4], [404.493,103.174,333.681,631.138], [1126.15,1168.05,1189.9,1062.77],
                [8952.64,8844.64,8823.6,8926.14], [333.85,7277.46,7170.63,3762.42]]

marker = ['o', '^', 'd', 's', '>', '<', 'p', 'v', 'H', '*']
color = ['green', 'royalblue', 'red', 'darkviolet', 'magenta', 'orange', 'darkgoldenrod', 'gold', 'k', 'k']


xlabel = r'Gap'
ylabel = 'Query Time'
title = "fig11_"
savedpath = './fig11/'
fontsize = 60
lineweight = 7.0
markersize = 50
marker_w = 5

def init_plt():
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

    plt.yscale('log')
    plt.xscale('log')

    plt.grid(linewidth=3)
    plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)

for i in range(len(total_graph)):
    xdata = Gap_list[i]
    ydata = y_query_time[i]

    if(i == 0 or i == 3 or i == 7): init_plt()
    plt.plot([xdata[0]], [ydata[0]], color=color[i], marker=marker[i], linestyle='', linewidth=lineweight, markersize=markersize, markeredgewidth=marker_w, fillstyle='none')
    plt.plot([xdata[1]], [ydata[1]], color=color[i], marker=marker[i], linestyle='', linewidth=lineweight, markersize=markersize, markeredgewidth=marker_w, fillstyle='bottom')
    plt.plot([xdata[2]], [ydata[2]], color=color[i], marker=marker[i], linestyle='', linewidth=lineweight, markersize=markersize, markeredgewidth=marker_w, fillstyle='left')
    plt.plot([xdata[3]], [ydata[3]], color=color[i], marker=marker[i], linestyle='', linewidth=lineweight, markersize=markersize, markeredgewidth=marker_w, fillstyle='full')

    if(i == 2):  # small graph
        f = plt.gcf() 
        respath = savedpath + "fig11a_small.png"
        f.savefig(respath)
        f.clear()
        print(f"saved {respath}")
    elif(i == 6):  # medium graph
        f = plt.gcf() 
        respath = savedpath + "fig11b_middle.png"
        f.savefig(respath)
        f.clear()
        print(f"saved {respath}")








