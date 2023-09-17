import matplotlib.pyplot as plt
import time


small_graph = ["EN", "CG", "WV"]
middle_graph = ["ND", "wiki_fy", "SD", "DB"]
large_graph = ["CP", "LJ", "wiki_zh"]

def plot_simrank_total(dataset_folder, simrank_file, title='Small'):
    xlabel = 'Rank'
    ylabel = 'SimRank Value'
    title = title + "_Rank"
    savedpath = './fig4/'
    fontsize = 80
    plt.figure(figsize=(24, 18))

    plt.xlabel(xlabel, fontsize=fontsize)  
    plt.ylabel(ylabel, fontsize=fontsize) 
    plt.tick_params(labelsize=fontsize) 
    plt.tick_params(axis='x', pad=20)

    plt.subplots_adjust(bottom=0.15)
    plt.subplots_adjust(left=0.18)

    ax = plt.gca()
    ax_width = 4
    ax.spines['bottom'].set_linewidth(ax_width) 
    ax.spines['left'].set_linewidth(ax_width)  
    ax.spines['right'].set_linewidth(ax_width)
    ax.spines['top'].set_linewidth(ax_width)

    plt.yscale('log')
    plt.xscale('log')


    plt.grid(linewidth=3)
    lineweight = 8.0
    markersize = 20

    # Record the minimum number for 0.6 in the four graphs, corresponding to the graphs ['ND', 'wiki_fy', 'SD', 'DB'].
    num_06_list = [19116510, 69528, 151147, 34007]
    min_06 = 34000

    for filename in simrank_file:
        t_s = time.time()
        filepath = dataset_folder + filename
        # Retrieve SimRank values sorted in ascending order
        s_list = read_simrank(filepath)

        print(f"read {filename} done, cost {time.time() - t_s}s")
        t_s = time.time()

        s_list.reverse()
        xdata = [i + 1 for i in range(len(s_list))]
        ydata = s_list

        xdata = xdata[min_06:]
        ydata = ydata[min_06:]

        if filename == "wiki_fy":
            filename = "WF"
        elif filename == "wiki_zh":
            filename = "WZ"
        plt.plot(xdata, ydata, marker='o', linestyle="", linewidth=lineweight,  label=filename, markersize=markersize)  
        print(f"ploat {filename} done, cost {time.time() - t_s}s")


    t_s = time.time()
    plt.legend(prop = {'size':fontsize-20}, loc='lower left')
    f = plt.gcf()  
    respath = savedpath + "{}.png".format(title)
    f.savefig(respath)
    f.clear()
    print(f"saved done, cost {time.time() - t_s}s")


def read_simrank(filepath_p):
    s_list = list()
    with open(filepath_p, 'r') as f:
        while True:
            line = f.readline()
            if not line:
                break
            sv = float(line)
            if sv > 0.6: continue
            s_list.append(sv)

    return s_list


if __name__ == "__main__":
    
    dataset_folder = "/data/geQian/packing_gq/SimRankRelease-master/Local-Push/ground_truth/"
    dataset_folder = "./middle_file/"

    # plot_simrank_total(dataset_folder, small_graph, title="Small")
    plot_simrank_total(dataset_folder, middle_graph, title="Middle")



