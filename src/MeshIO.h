#ifndef _MESHIO_H_
#define _MESHIO_H_

#include<vector>
#include<string>
#include"BaseHead.h"

struct TriMesh
{
    int id; // 当前对象的编号
    std::vector<Eigen::Vector3d> vecCoords;  // 当前对象的坐标
    std::vector<Eigen::Vector3i> vecTopos;  // 当前对象的拓扑连接关系
    Vector4unchar color;  // 当前对象的颜色
};


class MeshIO
{
public:
    static int id;

    std::vector<TriMesh> alltrimesh_;

    MeshIO();
    // MeshIO(std::vector<TriMesh> alltrimesh){};
    ~MeshIO();

    int ReadObjFile(std::string filename);  // 读取对象返回对象的id
    void SetColorById(int id, Vector4unchar color);  // 通过id设置对象的颜色
    void SetLocation(int id, Vec3d loc);
    void Scale(int id, double scale);
};

#endif