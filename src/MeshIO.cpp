#include"MeshIO.h"
#include"Tool.h"
#include <fstream>
#include <sstream>
#include <iostream>

int MeshIO::id = 0;


MeshIO::MeshIO()
{

}

MeshIO::~MeshIO()
{

}

int MeshIO::ReadObjFile(std::string filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        std::cout << "This file is cannot open !\n";
        throw "This file is cannot open!";
    }

    TriMesh trimesh;
    trimesh.id = MeshIO::id++;
    
    std::string line;
    std::string word;
    std::stringstream input;

    Vec3d coord;
    Vec3i topo;

    while (std::getline(file, line))
    {
        input.clear();
        input.str(line);
        input >> word;
        if(word[0] == '#') continue;
        if(word[0] == 'v')
        {
            input >> coord(0) >> coord(1) >> coord(2);
            trimesh.vecCoords.push_back(coord);
            continue;
        }
        if(word == "vn") continue;
        if(word[0] == 'f')
        {
            input >> topo(0) >> topo(1) >> topo(2);
            trimesh.vecTopos.push_back(topo);
            continue;
        }
    }

    

    this->alltrimesh_.push_back(trimesh);

    return trimesh.id;
}

void MeshIO::SetColorById(int id, Vector4unchar color)
{
    this->alltrimesh_[id].color = color;
}


void MeshIO::Scale(int id, double scale)
{
    for(int i = 0; i < alltrimesh_[id].vecCoords.size(); i++)
    {
        alltrimesh_[id].vecCoords[i]  = alltrimesh_[id].vecCoords[i] * scale;
    }
}

void MeshIO::SetLocation(int id, Vec3d loc)
{
    Vec3d centerLoc(0, 0, 0);
    Vec3d orient(0, 0, 0);
    for(int i = 0; i < alltrimesh_[id].vecCoords.size(); i++)
    {
        centerLoc += alltrimesh_[id].vecCoords[i];
    }
    if(alltrimesh_[id].vecCoords.size() != 0)
    centerLoc /= (double)alltrimesh_[id].vecCoords.size();
    orient = loc - centerLoc;
    for(int i = 0; i < alltrimesh_[id].vecCoords.size(); i++)
    {
        alltrimesh_[id].vecCoords[i] += orient;
    }
    return ;
}