/*
 * Graph.cpp
 *
 *  Created on: 2019��3��13��
 *      Author: yichen_shen
 */

#include "Graph.h"
#include "TextWriter.h"
#include "TextReader.h"
#include "Trim.h"
#include <math.h>
#include "IndexManager.h"
#include <time.h>
#include <iostream>
#include <algorithm>
Graph::Graph()
{}

Graph::~Graph(){}

void Graph::preprocess()
{
  unordered_set<Node *, hash_node1> remove;
  for (auto &n : nodes)
  {
    // if (n.in.size() < minNumEdges || n.out.size() < minNumEdges) {
    if (n.second.in.empty() || n.second.out.empty())
      remove.insert(&(n.second));
  }
  if (remove.empty())
    return;
  cout << remove.size() << " nodes to be removed..." << endl;

  for (auto &n : remove)
  {
    // remove from out neighbors' in
    for (auto &m : n->out)
      m->in.erase(find(m->in.begin(), m->in.end(), n));

    // remove from in neighbors' out
    for (auto &m : n->in)
      m->out.erase(find(m->out.begin(), m->out.end(), n));
    nodes.erase(n->id);
  }

  preprocess();
}

void Graph::saveToFile(string nodeFile, string edgeFile)
{
  TextWriter out(nodeFile);
  for (auto &n : nodes)
    out.writeln(n.second.id);
  out.close();
  TextWriter newout(edgeFile);
  int count = 0;
  for (auto &n : nodes)
    for (auto &m : n.second.out)
    {
      newout.writeln(n.second.id + "\t" + m->id);
      count++;
    }
  newout.close();
  cout << "# Nodes: " << nodes.size() << endl;
  cout << "# Edges: " << count << endl;
}

void Graph::clear()
{
  nodes.clear();
}


void Graph::loadGraphFromFile(string nodeFile, string edgeFile)
{
  clear();

  TextReader inN(nodeFile);
  TextReader inE(edgeFile);
  string line;

  clock_t startTime = clock();

  int count = 0;
  while ((line = inN.readln()).size() != 0)
  {
    int id = atoi(line.c_str()); 
    addNode(Node(id, count)); 
    nodeIndexToId[count] = id;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    count++;
    if (count % 100000 == 0)
      cout << ".";
  }
  while ((line = inE.readln()).size() != 0)
  {
    vector<string> ssplit;
    split(line, "\t", ssplit);
    int from = atoi(ssplit[0].c_str());
    int to = atoi(ssplit[1].c_str());
    addEdge(from, to);
  }
  cout << "Loading Graph End. cost " << (double)(clock() - startTime) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
  inN.close();
  inE.close();
  init();
}

void Graph::loadFromFile(string nodeFile, string edgeFile, bool identifyHubs)
{
  loadGraphFromFile(nodeFile, edgeFile);
  if (identifyHubs)
  {
    string hubNodeFile = IndexManager::getHubNodeFile();

    loadHubs(hubNodeFile);
  }
}

void Graph::loadFromFile(string nodeFile, string edgeFile, string hubFile)
{
  loadGraphFromFile(nodeFile, edgeFile);

  loadHubs(hubFile);
}

void Graph::loadHubs(string hubNodeFile)
{
  TextReader in(hubNodeFile);
  string line;

  unordered_set<Node *, hash_node1> hubs;

  while ((line = in.readln()).length() != 0)
  {
    if (hubs.size() == Config::numHubs)
      break;
    int id = atoi(line.c_str());
    Node *n = &getNode(id);
    //	            if (n == null)
    //	                n = new Node(id);
    n->isHub = true;
    hubs.insert(n);
  }

  in.close();
}

unordered_set<Node *, hash_node1> Graph::getHubs()
{
  return hubs;
}

void Graph::addNode(const Node &n)
{
  nodes[n.id] = n;
}

void Graph::addEdge(int from, int to)
{
  Node *nFrom = &getNode(from);
  Node *nTo = &getNode(to);
  nFrom->out.push_back(nTo);
  nTo->in.push_back(nFrom);
}

void Graph::resetPrimeG()
{
  unordered_set<int> tmp;
  is_visited.swap(tmp);
}

void Graph::computeOutPrimeSim(Node &q, double maxhubScore, PrimeSim &outSim)
{
  auto &simmap = outSim.getMap();
  unordered_map<int, unordered_map<int, double>>::iterator simit;
  vector<Node *> expNodes;
  unordered_map<int, double> valInLastLevel;
  vector<Node *> newExpNodes;
  unordered_map<int, double> valInThisLevel;
  double rea;
  expNodes.push_back(&q);
  is_visited.insert(q.id);
  //q.isVisited = true; //don't save query node as hub or meeting node
  outSim.addNewNode(q, false, is_visited);
  outSim.set(0, q, 1.0);
  valInLastLevel[q.id] = 1.0;
  int length = 1;

  while (length <= DEPTH)
  {
    if (expNodes.size() == 0)
      break;
    newExpNodes.resize(0);
    valInThisLevel.clear();
    for(auto &cur : expNodes)
    {
      for (auto &n : cur->out)
      { // an edge cur-->n, where cur is meeting node, so for n, should store the Reversed reachability: R(n->cur)=1*1/In(n)
        if (is_visited.find(n->id) == is_visited.end())
          outSim.addNewNode(*n, false, is_visited);
        rea = (double)valInLastLevel[cur->id] / (double)n->in.size() * (double)sqrt(Config::alpha);
        if (rea * maxhubScore > ITER_STOP)
        {
          if (valInThisLevel.find(n->id) == valInThisLevel.end())
          {
            valInThisLevel[n->id] = rea;
            if (n->out.size() > 0 && !n->isHub) newExpNodes.push_back(n);
            
            else Config::i_hubs_end += 1;
          }
          else
            valInThisLevel[n->id] = valInThisLevel[n->id] + rea;
        }
        else{
          Config::i_iter_stop += 1;
        }
        Config::i_total += 1;
      }
    }
    outSim.set(length, valInThisLevel);
    expNodes = newExpNodes;
    valInLastLevel = valInThisLevel;
    length++;
  }
  // Config::i_hubs_end = a;
  // Config::i_iter_stop = b;
  // Config::i_total = c;

  //	System.out.println("Remove empty length (Out) for node " + q.id);
  for (simit = simmap.begin(); simit != simmap.end();)
  {
    if (simit->second.size() == 0)
    {
      //			System.out.println("No such length in Insim " + l);
      simit = simmap.erase(simit);
    }
    else
    {
      simit++;
    }
  }
}

void Graph::computeInPrimeSim(Node &q, double maxhubScore, PrimeSim &inSim)
{ 
  // unordered_map<int, unordered_map<int, double>> map;
  auto &simmap = inSim.getMap();
  unordered_map<int, unordered_map<int, double>>::iterator simit;
  vector<Node *> expNodes;
  unordered_map<int, double> valInLastLevel; 

  expNodes.push_back(&q); // Nodes to be expanded, initially only q
  inSim.addNewNode(q, true, is_visited);


  inSim.set(0, q, 1.0);  
  valInLastLevel[q.id] = 1.0;
  int length = 1;
  vector<Node *> newExpNodes;  
  unordered_map<int, double> valInThisLevel;
  double rea;
  while (length <= DEPTH)
  {
    if (expNodes.size() == 0)
      break;
    newExpNodes.resize(0);
    valInThisLevel.clear();
    for (auto cur : expNodes)
    {
      for (auto n : cur->in)
      { //a edge n-->cur, if R(cur)==a, then R(n)==1/In(cur)*a, because of the reversed walk from cur to n.
        if (is_visited.find(n->id) == is_visited.end()) 
          inSim.addNewNode(*n, true, is_visited);  // mark n as visited and add n to meetingNodes in PrimeSim
        rea = valInLastLevel[cur->id] / (double)cur->in.size() * sqrt(Config::alpha);

        if (rea * maxhubScore > ITER_STOP)
        {
          if(valInThisLevel.find(n->id) == valInThisLevel.end())
          {
            valInThisLevel[n->id] = rea;
            if (n->in.size() > 0 && !n->isHub) 
              newExpNodes.push_back(n);
          }
          else
            valInThisLevel[n->id] = valInThisLevel[n->id] + rea;
        }
      }
    }

    inSim.set(length, valInThisLevel);
    expNodes = newExpNodes;
    valInLastLevel = valInThisLevel;
    length++;
  }

  for (simit = simmap.begin(); simit != simmap.end();)
  {
    if (simit->second.size() == 0)
    {
      simit = simmap.erase(simit);
    }
    else
    {
      simit++;
    }
  }
}

int Graph::numNodes()
{
  return nodes.size();
}

Node &Graph::getNode(int id)
{
  if (nodes.find(id) == nodes.end())
    nodes[id] = Node(id);
  return nodes[id];
}

vector<Node *> Graph::getNodes()
{
  vector<Node *> no;
  for (auto &it : nodes)
    no.push_back(&(it.second));
  return no;
}

bool Graph::containsNode(int id)
{
  return nodes.find(id) != nodes.end();
}

Graph::Graph(const Graph &Graph)
{
  this->nodes = Graph.nodes;
  this->hubs = Graph.hubs;
}
int Graph::findNode(int index)
{
  // for (auto &n : nodes){
  //   if (n.second.index == index)
  //     return n.second.id;
  // }
  return nodeIndexToId[index];
  // return -1;
}

void Graph::init()
{
  for (auto &n : nodes)
    n.second.initOutEdgeWeight();
}
