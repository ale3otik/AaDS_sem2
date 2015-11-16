
/*Alexey Zotov*/

#include <math.h>
#include <cassert>

#include "TravellingProblemSolver.h"

using std::vector;
using std::pair;
using std::make_pair;

vector<Edge> TravellingProblemSolver::GetMinimalTree() const{
    return min_spanning_tree;
}

std::vector<Edge> TravellingProblemSolver::GetFindedEdgesRoute() const{
    return finded_route_edges;
}

vector< pair<double,double>> TravellingProblemSolver::GetVertexArray() const{
    return vertex;
}

vector<size_t> TravellingProblemSolver::GetTheRoute() const{
    return finded_route;
}

double TravellingProblemSolver::GetLastRouteLength() const{
    return last_route_length;
}

double TravellingProblemSolver::CountDistance(size_t i, size_t j) const
{
    double dx = vertex[i].first - vertex[j].first;
    double dy = vertex[i].second- vertex[j].second;
    return sqrt(pow(dx,2) + pow(dy,2));
}

void TravellingProblemSolver::MakeEdges()
{
    edges.clear();
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        for(size_t j = i+1; j < vertex.size(); ++j)
        {
            double length = CountDistance(i, j);
            edges.push_back({length,i,j});
        }
    }
}

void TravellingProblemSolver::MakeGraph()
{
    graph.clear();
    graph.resize(vertex.size());
    for(size_t i = 0 ; i < edges.size(); ++i)
    {
        graph[edges[i].point1].push_back(i);
        graph[edges[i].point2].push_back(i);
    }
}

void TravellingProblemSolver::DFS(vector<bool> & used , size_t index)
{
    used[index] = true;
    finded_route.push_back(index);
    
    for(size_t i = 0; i < graph[index].size(); ++i)
    {
        Edge next_edge = edges[graph[index][i]];
        size_t next_vertex;
        if(!used[next_edge.point1])
        {
            next_vertex = next_edge.point1;
        }
        else if(!used[next_edge.point2])
        {
            next_vertex = next_edge.point2;
        }
        else continue;
        
        DFS(used,next_vertex);
    }
}
void TravellingProblemSolver::RestoreRoute()
{
    finded_route.clear();
    finded_route_edges.clear();
    last_route_length = 0;
    
    MakeGraph();
    vector <bool> used(vertex.size(),false);
    
    DFS(used,0);
    finded_route.push_back(0);
    
    for(size_t i = 0; i < finded_route.size()-1; ++i)
    {
        double length = CountDistance(finded_route[i],finded_route[i+1]);
        last_route_length += length;
        finded_route_edges.push_back( {length,finded_route[i],finded_route[i+1]} );
    }
}

void TravellingProblemSolver::FindTheRoute()
{
    if(vertex.size() == 0)
    {
        min_spanning_tree.clear();
        finded_route.clear();
        return;
    }
    
    DisjointSetUnion<size_t> disjoint_set;
    MakeEdges();
    
    sort(edges.begin(), edges.end(), EdgeComparator);
    
    for(size_t i = 0; i < vertex.size();++i)
    {
        disjoint_set.MakeSet(i);
    }

    size_t k = 0;
    for(size_t i = 0; i < edges.size(); ++i)
    {
        if(disjoint_set.Find(edges[i].point1) != disjoint_set.Find(edges[i].point2))
        {
            disjoint_set.Union(edges[i].point1, edges[i].point2);
            min_spanning_tree.push_back(edges[i]);
            
            ++k;
            if(k == vertex.size() - 1) break;
        }
    }
    
    RestoreRoute();
}

void TravellingProblemSolver::CreateVertex(int sigma_centers, int n_centers, int sigma_points, int n_points)
{
    vertex.clear();
    
    std::default_random_engine generator ((int)std::chrono::system_clock::now().time_since_epoch().count()); //(c)Laktionov
    std::normal_distribution<double> generation_centres(0, sigma_centers);

    for(int i = 0; i < n_centers; ++i)
    {
        double center_x,center_y;
        center_x = generation_centres(generator);
        center_y = generation_centres(generator);
        std::normal_distribution<double> generation_point_x(center_x, sigma_points);
        std::normal_distribution<double> generation_point_y(center_y, sigma_points);
        
        for(int j = 0; j < n_points; ++j)
        {
            double point_x,point_y;
            point_x = generation_point_x(generator);
            point_y = generation_point_y(generator);
            vertex.push_back(make_pair(point_x, point_y));
        }
    }
}

/***************EDGE COMPARATOR***********************/

bool EdgeComparator(const Edge & first, const Edge & second)
{
    return first.length < second.length;
}