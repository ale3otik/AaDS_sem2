
/*Alexey Zotov*/

#ifndef __TravellingSalesmanProblem__TravellingProblemSolver__
#define __TravellingSalesmanProblem__TravellingProblemSolver__

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "DisjointSetUnion.h"

struct Edge
{
    double length;
    size_t point1;
    size_t point2;
};

bool EdgeComparator (const Edge & first, const Edge & second);

class TravellingProblemSolver
{
public:
    void CreateVertex(int sigma_centers, int n_centers, int sigma_points, int n_points);
    
    double GetLastRouteLength() const;
    std::vector<size_t> GetTheRoute() const;
    std::vector<Edge> GetMinimalTree() const;
    std::vector<Edge> GetFindedEdgesRoute() const;
    std::vector< std::pair<double,double> > GetVertexArray() const;
    void FindTheRoute();
private:
    std::vector<std::pair <double, double> > vertex;
    std::vector<Edge> edges; // indexes in vertex
    std::vector<Edge> min_spanning_tree; //edges of tree
    std::vector<size_t> finded_route; // indexes of vertex array
    std::vector<Edge> finded_route_edges; // edges
    std::vector<std::vector<size_t> > graph; //to dfs
    
    double last_route_length;
    
    void MakeEdges(); // to sort and make spanning tree
    void MakeGraph(); // to dfs
    void RestoreRoute();
    void DFS(std::vector<bool> & used ,size_t index);
    double CountDistance(size_t i, size_t j) const;
};


#endif /* defined(__TravellingSalesmanProblem__ TravellingProblemSolver__) */
