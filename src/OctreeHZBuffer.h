#ifndef _OCTREEHZBUFFER_
#define _OCTREEHZBUFFER_

#include"Render.h"
#include"BaseHead.h"
#include"BaseHead.h"

class OctHZBuffer : public Render
{
public:
    octree* deepbufferoctree_;
    ftree* deepbuffertree_;
    double* deepbuffer_;
    int treedeep_;


    OctHZBuffer();
    OctHZBuffer(int width, int height, Vector4unchar nkcolor, int treedeep);
    ~OctHZBuffer();


    bool RenderTriangle(Vec2i* twodimcoord, Vec3d& deep, Vector4unchar& color);
    bool JudgeRecInRec(Vec2i& ld, Vec2i& ru, Vec2i* coord);
    bool JudgeRender(ftree* tree, Vec2i& ld, Vec2i& ru, Vec2i* coord, double deep);

    void DeleteQuadTree(ftree* curtree);
    void BuildQuadTree(ftree* curtree, Vec2i& ld, Vec2i& ru);
    void UpdateQuadTreeNode(ftree* curtree, Vec2i& ld, Vec2i& ru, Vec2i& target);
    void UpdateQuadTree(ftree* curtree, Vec2i& ld, Vec2i& ru);

    void InitOctree(octree* curtree, int curdeep);
    void BuildOctree(std::vector<TriMesh>& trimesh, Vec3d& minn, Vec3d& maxx);
    void InsertTriNode(octree* curtree, Vec3d& minn, Vec3d& maxx, triNode& trinode, int curdeep);
    bool JudgeTriNodeInBox(triNode& trinode, Vec3d& minn, Vec3d& maxx);
    void OctreeRender(octree* curtree, Vec3d& minn, Vec3d& maxx, int curdeep, int id);

    virtual int RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh) override;  // 绘制所有的物体
    virtual void ClearDeepBuffer() override;
};


#endif