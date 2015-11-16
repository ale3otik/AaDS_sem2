
//
//  Copy Alexander Laktionov
//

#ifndef __voyager__opengl__
#define __voyager__opengl__
#include <vector>
#include "TravellingProblemSolver.h"



void visualizer(std::vector<std::pair<double,double> >  &data, std::vector<Edge> &edges, std::vector<Edge>finded_route,int centers);

void reshape(int width, int height);

void display();

void DrawPoints(float x1,float y1);



#endif /* defined(__voyager__opengl__) */
