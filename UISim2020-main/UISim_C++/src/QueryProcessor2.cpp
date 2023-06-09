/*
 * QueryProcessor2.cpp
 *
 *  Created on: 2019��3��16��
 *      Author: yichen_shen
 */

#include <QueryProcessor2.h>
#include <iostream>
#include <unordered_map>
#include <ctime>
double QueryProcessor2::checkMeetigNodeImportance(PrimeSim &inSim, int mid)
{
  double sum = 0.0;
  int count = 0;
  for (int length = 1; length < inSim.getMap().size(); length++)
  {
    unordered_map<int, double> &tmp = inSim.getMap()[length];
    if (tmp.find(mid) == tmp.end())
      continue;

    double rea = inSim.getMap()[length][mid];
    sum += rea;
    count++;
  }
  return sum / count;
}

QueryProcessor2::QueryProcessor2(Graph &graph)
{
  this->graph = &graph;
}

void QueryProcessor2::graphExp(Node &q, string graphType, PrimeSim &sim)
{

  graph->resetPrimeG();
  double expansion = Config::eta;
  double expansiondemical = expansion - (float)(int)expansion;
  if (expansiondemical == 0)
  {
    expansiondemical = 1;
  }

  if (false)
  {
    sim.loadFromDisk(q.id, graphType);
  }
  else
  {
    if (graphType == "out")
      graph->computeOutPrimeSim(q, 1, sim);
    else if (graphType == "in")
      graph->computeInPrimeSim(q, 1, sim);
    else
    {
      cout << "Type of prime graph should be either out or in.";
      exit(0);
    }
  }

  if (expansion == 0 || sim.numHubs() == 0)
    return;

  PrimeSim expSim = sim.duplicate();

  if (graphType == "in")
    expSim.addMeetingNodes(sim.getMeetingNodes());


  unordered_map<int, unordered_map<int, double>> borderHubsScore; // hub->(length,value)

  // extracting borderHubs information
  for (auto &length : sim.getMap())
  {
    //added 8-27
    if (length.first == 0)
      continue; //don't expand the query node if itself is a hub
    for (auto &nid : sim.getMap()[length.first])
    {
      Node &node = graph->getNode(nid.first);
      //added 8-27
      //if(node==q) continue; q should also be expanded, it can affect the reachability of other nodes, just s(q,q) wouldn't be affected
      if (node.isHub)
      {
        // store the reachability to hub
        //				if (borderHubsScore.find(nid.first) == borderHubsScore.end()) {
        //					borderHubsScore[nid.first] = unordered_map<int, double>();
        //				}
        if (borderHubsScore[nid.first].find(length.first) == borderHubsScore[nid.first].end())
        {
          borderHubsScore[nid.first][length.first] = sim.getMap()[length.first][nid.first];
        }
        else
        {
          cout << "shouldn't go to here.";
          double old_value = borderHubsScore[nid.first][length.first];
          borderHubsScore[nid.first][length.first] = old_value + sim.getMap()[length.first][nid.first];
        }
      }
    }
  }

  // recursively adding outG of hubs
  //int i = 0;

  while (expansion > 0)
  {

    expansion = expansion - 1;
    //			i++;
    //			if (gType == "out")
    //				System.out.println("@@@Iteration " + i);
    // expansion--;
    unordered_map<int, unordered_map<int, double>> borderHubsNew;
    if (borderHubsScore.size() == 0)
    {
      sim = expSim;
      return;
    }
    int p = 0;
    int maxnode = (int)borderHubsScore.size() * expansiondemical;
    for (auto &hid : borderHubsScore)
    {
      //add expanding threshold for hubs: Config.delta
      //	double hubScore = 0;
      double maxHubScore = 0;
      if (p > maxnode)
        break;
      p++;
      for (auto &len : hid.second)
      {
        double current_hubscore = hid.second[len.first];
        if (current_hubscore > maxHubScore)
        {
          maxHubScore = current_hubscore;
        }
        //	hubScore += current_hubscore;
      }

      //	if(hubScore<Config.delta)
      //		continue;
      //end. 2/18/2015fw
      PrimeSim nextSim;
      //commented the next line to prevent using hub scores 04/17
      //				nextSim.loadFromDisk(hid, graphType);

      // modify start 04/17 to do online computation even if it is hub
      Node &h = graph->getNode(hid.first);

      if (graphType == "out")
        graph->computeOutPrimeSim(h, maxHubScore, nextSim);
      else if (graphType == "in")
        graph->computeInPrimeSim(h, maxHubScore, nextSim);

      //modify end
      //	System.out.println(graphType+" Graph of hub: " + hid);
      //	System.out.println(nextSim.getMap());
      if (graphType == "in")
        expSim.addMeetingNodes(nextSim.getMeetingNodes());

      expSim.addFrom(nextSim, hid.second); // expand
                                           // graph
      // PrintInfor.printDoubleMap(expSim.getMap(),
      // "&&&=&=&=&& Expanded graph out sim");

      if (expansion > 0)
      {
        //store border hubs in nextSim
        for (int i = 0; i < nextSim.numHubs(); i++)
        {
          int newHub = nextSim.getHubId(i);
          for (int l = 1; l < nextSim.getMap().size(); l++)
          {
            unordered_map<int, double> &tmp = nextSim.getMap()[l];
            if (tmp.find(newHub) != tmp.end())
            {

              double addScore = nextSim.getMap()[l][newHub];

              //set borderHubsNew
              if (borderHubsNew.find(newHub) == borderHubsNew.end())
                borderHubsNew[newHub] = unordered_map<int, double>();
              auto &tmp = borderHubsNew[newHub];
              for (auto &oldLen : hid.second)
              {
                double oldScore = hid.second[oldLen.first];
                double existScore;
                if (tmp.find(l + oldLen.first) == tmp.end())
                  existScore = 0.0;
                else
                  existScore = tmp[l + oldLen.first];
                tmp[l + oldLen.first] = existScore + oldScore * addScore;
              }
            }
          }
        }

      } //end if

    } //end for
    //			if (borderHubsNew.size() == 0)
    //				return expSim;

    borderHubsScore = borderHubsNew;
  }
  sim = expSim;
  return;
}

void QueryProcessor2::query(Node &q, unordered_map<int, double> &result)
{
  PrimeSim inSim;
  graphExp(q, "in", inSim);

  if (inSim.getMeetingNodes().size() == 0)
  {
    cout << "No meeting nodes in the graph " << q.id << endl;
    result[q.id] = 1.0;
    return;
  }

  for (int mid : inSim.getMeetingNodes())
  {
    PrimeSim outSim;
    graphExp(graph->getNode(mid), "out", outSim);

    int minLen = inSim.numLength() > outSim.numLength() ? outSim.numLength() : inSim.numLength();
    for (int length = 1; length < minLen; length++)
    {
      unordered_map<int, double> &tmp = inSim.getMap()[length];
      if (tmp.find(mid) == tmp.end())
        continue;
      double reaTomeetingNode = inSim.getMap()[length][mid];

      for (auto &endingNode : outSim.getMap()[length])
      {
        if (endingNode.first == q.id)  
          continue;
        double oldValue = (result.find(endingNode.first) == result.end()) ? 0.0 : result[endingNode.first];
        result[endingNode.first] =
            oldValue + reaTomeetingNode * endingNode.second * (1 - Config::alpha/(graph->getNode(mid).in.size()));

      }
    }
  }

  result[q.id] = 1.0;
  return;
}

QueryProcessor2::~QueryProcessor2()
{}
