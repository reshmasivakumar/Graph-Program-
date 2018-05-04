#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>

#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */

/** constructor, empty graph */
Graph::Graph() {}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph()
{
	for (const auto& var : vertices)
	{
		delete var.second;
	}
}

/** return number of vertices */
int Graph::getNumVertices() const
{
	return vertices.size();
}

/** return number of Edges */
int Graph::getNumEdges() const
{
	int numEdges = 0;
	for (const auto& var : vertices)
	{
		numEdges += var.second->getNumberOfNeighbors();
	}
	return numEdges;
}

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight)
{
	Vertex *startVertex, *endVertex;
	//create startVertex if not already exists
	startVertex = findOrCreateVertex(start);
	//create endVertex if not already exists
	endVertex = findOrCreateVertex(end);
	//connect the start and end to create an edge and assign weight
	return startVertex->connect(end, edgeWeight);
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const
{
	//return INT_MAX if start and end vertex doesnot exist
	if (findVertex(start) == nullptr || findVertex(end) == nullptr)
	{
		return INT_MAX;
	}
	else
	{
		//get weight of the edge between start and end
		int edgeWeight = findVertex(start)->getEdgeWeight(end);
		//if no weight is assigned return INT_MAX
		if (edgeWeight < 0)
			return INT_MAX;
		else
		     return edgeWeight;
	}
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
 void Graph::readFile(std::string filename) {
 	 int noOfEdges;
	 int edgeWeight;
	 string startLabel, endLabel;
	 ifstream inFile;
	  inFile.open(filename,ios::in);
  	 if(!inFile.eof())
	 {
  		 //first line is number of edges
  		inFile>>noOfEdges;
  		//read start , end and edge weight
 	     while(inFile>>startLabel>>endLabel>>edgeWeight)
        {
 	    	 //create edge in graph
  	    	add(startLabel, endLabel, edgeWeight);
	     }
	 }
  	inFile.close();
}




/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&))
{
	unvisitVertices();
	depthFirstTraversalHelper(findVertex(startLabel), visit);
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&))
{
	unvisitVertices();
	queue<Vertex*> vertexQ;
	Vertex *startVertex = findVertex(startLabel);
	vertexQ.push(startVertex);
	breadthFirstTraversalHelper(startVertex, visit);
	while (vertexQ.empty() == false)
	{
		Vertex *w = vertexQ.front();
		vertexQ.pop();
		w->resetNeighbor();
		string neighbor = w->getNextNeighbor();
		while (neighbor != w->getLabel())
		{
			Vertex *u = findVertex(neighbor);
			if (u->isVisited() == false)
			{
				breadthFirstTraversalHelper(u, visit);
				vertexQ.push(u);
			}
			neighbor = w->getNextNeighbor();
		}
	}
}

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous)
{
	priority_queue<pair<int,string>,vector<pair<int, string>>,
	                            greater<pair<int, string>>> pq;

	weight.clear();
	previous.clear();

	for (auto it : vertices)
	{
		weight.emplace(it.first, INT_MAX);
	}

	Vertex *startVertex = findVertex(startLabel);
	startVertex->resetNeighbor();
	string neighbor = startVertex->getNextNeighbor();
	while (neighbor != startLabel)
	{
		weight[neighbor]= getEdgeWeight(startLabel,neighbor);
		previous[neighbor] = startLabel;

		pq.push(make_pair(weight.at(neighbor), neighbor));
		neighbor = startVertex->getNextNeighbor();
	}
	set<string> vertexSet;
	vertexSet.emplace(startLabel);

	while (pq.empty() == false)
	{
		string v = pq.top().second;
		pq.pop();
		if (vertexSet.find(v) == vertexSet.end())
		{
			vertexSet.emplace(v);
			Vertex *vert = findVertex(v);
			vert->resetNeighbor();
			string u = vert->getNextNeighbor();
			while (u != v)
			{
				int v2ucost = getEdgeWeight(v,u);
				if (weight.find(u) == weight.end())
				{
					weight[u] = weight.find(v)->second + v2ucost;
					previous[u]= v;
					pq.push(make_pair(weight.at(u), u));
				}
				else
				{
					if (weight.find(u)->second > weight.find(v)->second
                                          					+ v2ucost)
					{
						weight[u] = weight.find(v)->second + v2ucost;
						previous[u] = v;
						pq.push(make_pair(weight.at(u), u));
					}
				}
				u = vert->getNextNeighbor();
			}
		}
	}
	weight[startLabel]= INT_MAX;
	previous.erase(startLabel);
}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
                                      void visit(const std::string&))
{
	visit(startVertex->getLabel());
	startVertex->visit();
	startVertex->resetNeighbor();
	string neighbor = startVertex->getNextNeighbor();
	while (neighbor != startVertex->getLabel())
	{
		Vertex *n = findVertex(neighbor);
		if (n->isVisited() == false)
		{
			depthFirstTraversalHelper(n, visit);
		}
		neighbor = startVertex->getNextNeighbor();
	}
}

/** helper for breadthFirstTraversal */
void Graph::breadthFirstTraversalHelper(Vertex*startVertex,
                                        void visit(const std::string&))
{
	visit(startVertex->getLabel());
	startVertex->visit();
}

/** mark all verticies as unvisited */

void Graph::unvisitVertices()
{
	std::map<string, Vertex*>::const_iterator iter;
	for (iter= vertices.begin(); iter != vertices.end(); ++iter)
	{
		iter->second->unvisit();
	}
}




/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const
{
	std::map<string, Vertex*>::const_iterator it;
	it = vertices.find(vertexLabel);
	if (it == vertices.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel)
{
	Vertex* vertex = findVertex(vertexLabel);
	if (vertex == nullptr)
	{
		vertex = new Vertex(vertexLabel);
		vertices.emplace(vertexLabel,vertex);
	}
	return vertex;

}
