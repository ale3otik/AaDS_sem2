
/*Alexey Zotov*/

#include <iostream>
#include "DisjointSetUnion.h"
#include "TravellingProblemSolver.h"
#include "opengl.h"
using namespace std;

#define _OPEN_GL_VISUALIZE_
int main()
{
    int sigma_center,sigma_point,n_centers,n_points;
    cin>> sigma_center >>sigma_point>>n_centers>>n_points;
    cerr<<"end reading"<<endl;
    TravellingProblemSolver solver;
    
    solver.CreateVertex(sigma_center, n_centers, sigma_point, n_points);
    cerr<<"end creating"<<endl;
    
    solver.FindTheRoute();
    cerr<<"end finding"<<endl;
    
    vector<Edge> tree = solver.GetMinimalTree();
    vector<pair<double,double> > vertex = solver.GetVertexArray();
    vector<Edge> route = solver.GetFindedEdgesRoute();
    vector<size_t> route_vertex = solver.GetTheRoute();
    double length = solver.GetLastRouteLength();
    cerr<<"all"<<endl;
    
    for(size_t i = 0; i < route_vertex.size(); ++i)
    {
        cout<<"<"<<vertex[route_vertex[i]].first<<" , "<<vertex[route_vertex[i]].second<<">"<<endl;
    }
    cout<<"length:" <<length<<endl;
    
#ifdef _OPEN_GL_VISUALIZE_
    visualizer(vertex, tree, route, 0);
#endif
    
    cerr<<"all"<<endl;
    
    return 0;
}
