#ifndef _TOOL_H_ 
#define _TOOL_H_ 

#include "BaseHead.h"
#include "MeshIO.h"

Mat3 getlookatmatrix(Eigen::Vector3d& eye, Eigen::Vector3d& center, Eigen::Vector3d& up);
void lookatafter(std::vector<TriMesh>&, Mat3&);


#endif