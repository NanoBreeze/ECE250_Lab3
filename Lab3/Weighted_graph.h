//#pragma once
	//ref class Weighted_graph
	//{
	//public:
	//	Weighted_graph();
	//};


/*****************************************
* Instructions
*  - Replace 'uwuserid' with your uWaterloo User ID
*  - Select the current calendar term and enter the year
*  - List students with whom you had discussions and who helped you
*
* uWaterloo User ID:  l43cheng @uwaterloo.ca
* Submitted for ECE 250
* Department of Electrical and Computer Engineering
* University of Waterloo
* Calender Term of Submission:  (Fall) 2015
*
* By submitting this file, I affirm that
* I am the author of all modifications to
* the provided code.
*
* The following is a list of uWaterloo User IDs of those students
* I had discussions with in preparing this project:
*    -
*
* The following is a list of uWaterloo User IDs of those students
* who helped me with this project (describe their help; e.g., debugging):
*    - mg2kim 
*****************************************/

#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#ifndef nullptr
#define nullptr 0
#endif

#include <iostream>
#include <limits>
#include "Exception.h"
#include <vector>
#include <map>

using namespace std;

class Weighted_graph {
private:
	static const double INF; 

	//first vector holds all vertices
	//map holds the adjacent vertex and the weight
	vector<map<int, double> > vertices;
	
	//vector shows which vertices have been visited
	vector<bool> visited;

	//this exists because changing mst_weight is not allowed in a const function. Thus we store them all
	//vector<double> minimum_weights;

	//total number of edges in graph
	int edge_number;

	
	//total number of nodes in graph. Used to compare to the number of nodes in MST to determine if graph is connected
	int node_number;

	//total number of nodes in MST. Used to compare with node_number
	int mst_node_number;


	void check_valid_vertices(const int&, const int&) const;


public:


	Weighted_graph(int = 50);
	~Weighted_graph();

	int degree(int) const;
	int edge_count() const;
	double adjacent(int, int) const;
	double minimum_spanning_tree(int) ;
	bool is_connected() ;

	void insert(int, int, double);

	

	// Friends

	friend std::ostream &operator<<(std::ostream &, Weighted_graph const &);
};


const double Weighted_graph::INF = std::numeric_limits<double>::infinity();

//set edge number and first vector with vertices
Weighted_graph::Weighted_graph(int i) : edge_number(0), node_number(i), mst_node_number(0)
{
	for (int k = 0; k < i; k++)
	{
		vertices.push_back(map<int, double>());
		visited.push_back(false);
	}

}

Weighted_graph::~Weighted_graph()
{
	
}

//returns the degree of the selected vertex
int Weighted_graph::degree(int v) const
{
	if (v > vertices.size() - 1)	
	{
		throw illegal_argument();
	}
	else
	{
		return vertices[v].size();
	}
}

//returns the total number of edges in the graph
int Weighted_graph::edge_count() const
{
	return edge_number;
}

//returns the weight of the edge connecting two vertices
//if the vertices are equal, return 0
//if the vertices are not adjacent, return infinity
//if arguments do not correspond to existing vertices, throw illegal exception
double Weighted_graph::adjacent(int v1, int v2) const
{
	check_valid_vertices(v1, v2);
	if (v1 == v2) { return 0; }
	else if (vertices[v1].count(v2) == 0) { return INF; }
	else { return vertices[v1].at(v2); }	
}

//return size of the MST connected to vertex
//to find MST, check first vertex. Then examine all adjacent vertices and now examine only the adjacent vertices that haven'y been visited. 
//Select the vertex with the lowest weight, add to weight number, mark the other vertex. 
//Repeat but now check both vertices
double  Weighted_graph::minimum_spanning_tree(int v) 
{
	if (v > vertices.size() - 1 || v < 0) {
		throw illegal_argument();
	}

	mst_node_number = 0;

	//stores the vertices that are currently in the MST. We will examine each of these vertices adjacent vertices.
	vector<int> connected_vertices;
	connected_vertices.push_back(v);

	visited[v] = true;

	//stores the vertex that contains the min weight among all the vertices in connected_vertices
	int vertex_containing_min = -1;

	//stores the minimum weight among the non-checked adjacent vertices
	double min = INF;

	bool test_if_we_can_push_another_vertex_to_connected_vertices = false;

	//the weight of the MST
	double mst_weight = 0;

	for (int connected = 0; connected < connected_vertices.size(); connected++)
	{
		int check_examinedVertex = connected_vertices[connected];
		//iterate through adjacent vertices
		for (map<int, double>::iterator adjacent = vertices[connected_vertices[connected]].begin(); adjacent != vertices[connected_vertices[connected]].end(); adjacent++)
		{
			int check1_adjacentVertex = adjacent->first;
			//operate only with vertices that haven't been marked
			if (visited[adjacent->first] == false)
			{
				test_if_we_can_push_another_vertex_to_connected_vertices = true;
				//among the non-visited vertices, find the one with the lightest weight by interating through them
				if (adjacent->second < min)
				{
					min = adjacent->second;
					vertex_containing_min = adjacent->first;
				}
			}
		}

		//add to mst_weight and changed visited status only after all vertices in connected_vertices are examined
		if (connected == connected_vertices.size() - 1)
		{
			//no new vertex to ad to MST, thus, MST is completed
			if (test_if_we_can_push_another_vertex_to_connected_vertices == false)
			{
				//upon returning weight, set the visited matrix to false values, ready to be used next time
				//set mst_weight back to 0
				for (int i = 0; i < visited.size(); i++) { visited[i] = false; }
				return mst_weight;
			}
			else
			{
				visited[vertex_containing_min] = true;
				mst_weight += min;
				//one more vertex added to our current vertex
				connected_vertices.push_back(vertex_containing_min);
				connected= -1; //to ensure we start with the first vertex in connected_vertex again
				vertex_containing_min = -1;
				min = INF; test_if_we_can_push_another_vertex_to_connected_vertices = false;
				//this one is used in is_Connected
				mst_node_number++;
			
			}
		}


	}
}


//determines if the graph is connected. True if same number of nodes in graph as in MST
bool Weighted_graph::is_connected() 
{
	//node_number = 0;

	////to use with minimum_spanning_tree
	//int vertex_has_edge=0;
	//
	////sets node_number
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	if (vertices[i].size() >= 1)
	//	{
	//		vertex_has_edge = i;
	//		node_number++;
	//	}
	//}


	//sets mst_node_number
	double m = minimum_spanning_tree(0);

	//return mst_node_number;
	return (node_number == mst_node_number+1) ? true : false;
}

//insert an edge incident to two vertices and specify its weight
//throws exception for invalid vertex1 and vertex2
//if the edge already exists, replace it
//else add it to both vectors
void Weighted_graph::insert(int v1, int v2, double weight)
{
	//invalid values throw exceptions
	if (weight < 0 || weight == std::numeric_limits<double>::infinity() ) 
	{
		throw illegal_argument();
	}

	check_valid_vertices(v1, v2);

	if (v1 == v2) throw illegal_argument();

	//if weight is 0, remove the edge, if it exists between the entered vertices
	//else add new edge or replace existing one
	if (weight == 0)
	{
		//currently exists, thus, remove
		if (vertices[v1].count(v2) == 1)		//|| vertices[v2].count(v1) == 1
		{
			vertices[v1].erase(v2);
			vertices[v2].erase(v1);
			edge_number--;
		}
	}
	else
	{
		if (vertices[v1][v2] == 0 || vertices[v2][v1] == 0)
		{
			edge_number++;
		}
			vertices[v1][v2] = weight;
			vertices[v2][v1] = weight;
	}
}


std::ostream &operator<<(std::ostream &out, Weighted_graph const &graph) {
	// Your implementation

	return out;
}

//=============HELPER FUNCTIONS==================

//checks if the vertices exist
void Weighted_graph::check_valid_vertices(const int& v1, const int& v2) const
{
	if (v1 > vertices.size() - 1 || v2 > vertices.size() - 1 || v1 < 0 || v2 < 0)
	{
		throw illegal_argument();
	}

}


#endif