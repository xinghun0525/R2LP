
#include <QueryProcessor3.h>
#include <iostream>
#include <unordered_map>
#include <ctime>
double QueryProcessor3::checkMeetigNodeImportance(PrimeSim &inSim, int mid)
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

QueryProcessor3::QueryProcessor3(Graph &graph)
{
  this->graph = &graph;
}

PrimeSim QueryProcessor3::graphExp(Node &q, string graphType)
{ 

  PrimeSim sim;
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
    return sim; 
  

  PrimeSim expSim = sim.duplicate();

  if (graphType == "in")
    expSim.addMeetingNodes(sim.getMeetingNodes());

  unordered_map<int, unordered_map<int, double>> borderHubsScore; // hub->(length,value)

  for (auto &length : sim.getMap())
  {
    if (length.first == 0)
      continue; //don't expand the query node if itself is a hub
    for (auto &nid : sim.getMap()[length.first])
    {
      Node &node = graph->getNode(nid.first);
      if (node.isHub)
      {
        if (borderHubsScore.find(nid.first) == borderHubsScore.end())
        {
          borderHubsScore[nid.first] = unordered_map<int, double>();
        }
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
  while (expansion > 0)
  {
    expansion = expansion - 1;
    unordered_map<int, unordered_map<int, double>> borderHubsNew;
    if (borderHubsScore.size() == 0)
      return expSim;
    int p = 0;
    int maxnode = (int)borderHubsScore.size() * expansiondemical;
    for (auto &hid : borderHubsScore)
    {
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
      }
      PrimeSim nextSim;

      Node &h = graph->getNode(hid.first);

      if (graphType == "out")
        graph->computeOutPrimeSim(h, maxHubScore, nextSim);
      else if (graphType == "in") 
        graph->computeInPrimeSim(h, maxHubScore, nextSim);

      if (graphType == "in")
        expSim.addMeetingNodes(nextSim.getMeetingNodes());

      expSim.addFrom(nextSim, hid.second); // expand

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
      }
    }

    borderHubsScore = borderHubsNew;
  }
  return expSim;
}

double QueryProcessor3::query(Node &q1, Node &q2)
{
  if (q1.id == q2.id)
    return 1;

  double result = 0;

  PrimeSim inSim1 = graphExp(q1, "in");
  PrimeSim inSim2 = graphExp(q2, "in");

  if (inSim1.getMeetingNodes().size() == 0 || inSim2.getMeetingNodes().size() == 0)
  {
    result = 0;
    return result;
  }

  PrimeSim smallPS = inSim1.numLength() < inSim2.numLength() ? inSim1 : inSim2;
  PrimeSim bigPS = inSim1.numLength() < inSim2.numLength() ? inSim2 : inSim1;

  for (auto &length : smallPS.getMap())
  {

    if (bigPS.getMap().find(length.first) == bigPS.getMap().end())
      continue;

    unordered_map<int, double> small_node_score_map = smallPS.getMap()[length.first].size() < bigPS.getMap()[length.first].size() ? smallPS.getMap()[length.first] : bigPS.getMap()[length.first];
    unordered_map<int, double> big_node_score_map = smallPS.getMap()[length.first].size() < bigPS.getMap()[length.first].size() ? bigPS.getMap()[length.first] : smallPS.getMap()[length.first];

    for (auto &nid : small_node_score_map)
    {
      if (big_node_score_map.find(nid.first) == big_node_score_map.end())
        continue;
      double rea = small_node_score_map[nid.first] * big_node_score_map[nid.first]; //multiply alpha here
      result += rea;
    }
  }
  return result;
}

QueryProcessor3::~QueryProcessor3()
{}
