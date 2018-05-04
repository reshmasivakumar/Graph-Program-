#include <climits>
#include "vertex.h"
#include <functional>
#include <map>
#include <string>
#include "edge.h"


/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label)
{
	vertexLabel = label;
	unvisit();
	adjacencyList.clear();
	resetNeighbor();
}

/** @return  The label of this vertex. */
std::string Vertex::getLabel() const
{
	return vertexLabel;
}

/** Marks this vertex as visited. */
void Vertex::visit()
{
	visited = true;
}

/** Marks this vertex as not visited. */
void Vertex::unvisit()
{
	visited = false;
}

/** Returns the visited status of this vertex.
 @return  True if the vertex has been visited, otherwise
    returns false/ */
bool Vertex::isVisited() const
{
	return visited;
}

/** Adds an edge between this vertex and the given vertex.
    Cannot have multiple connections to the same endVertex
    Cannot connect back to itself
 @return  True if the connection is successful. */
bool Vertex::connect(const std::string& endVertex, const int edgeWeight)
{
	if (vertexLabel == endVertex)
	{
		return false;
	}
	else
	{
		/*Edge newEdge(endVertex,edgeWeight );
		adjacencyList[endVertex]= newEdge;
		*/
		Edge *edge = new Edge(endVertex, edgeWeight);

		if (adjacencyList.emplace(endVertex, *edge).second)
		{
			delete edge;
			return true;
		}
		else
		{
			delete edge;
			return false;
		}


	}
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex)
{
	std::map<string, Edge>::iterator iter;
	iter = adjacencyList.find(endVertex);
	if (iter != adjacencyList.end())
	{
		adjacencyList.erase(iter);
		return true;
	}
	else
	{
		return false;
	}
}

/** Gets the weight of the edge between this vertex and the given vertex.
 @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex) const
{
	int edgeWeight = -1;
	std::map<string, Edge>::const_iterator iter;
	iter = adjacencyList.find(endVertex);
	if (iter == adjacencyList.end())
	{
		return edgeWeight;
	}
	else
	{
		edgeWeight = iter->second.getWeight();
		return edgeWeight;
	}

}

/** Calculates how many neighbors this vertex has.
 @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const
{
	return adjacencyList.size();
}

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor()
{
	currentNeighbor = adjacencyList.begin();
}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
 @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor()
{
	string returnLabel = vertexLabel;
	if(currentNeighbor!=adjacencyList.end())
	{
		returnLabel = currentNeighbor->first;
		currentNeighbor++;
	}
	return returnLabel;
}

/** Sees whether this vertex is equal to another one.
    Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const
{
	return vertexLabel==rightHandItem.vertexLabel;
}

/** Sees whether this vertex is < another one.
    Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const
{
	return vertexLabel < rightHandItem.vertexLabel;
}

