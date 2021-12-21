#ifndef _HIERARCHICALZBUFFER_
#define _HIERARCHICALZBUFFER_

#include"Render.h"
#include"BaseHead.h"

class HZBuffer : public Render
{
public:
    ftree* deepbuffertree_;
    double* deepbuffer_;


    HZBuffer();
    HZBuffer(int width, int height, Vector4unchar nkcolor);
    ~HZBuffer();


    bool RenderTriangle(Vec2i* twodimcoord, Vec3d& deep, Vector4unchar& color);
    bool JudgeTriInRec(Vec2i ld, Vec2i ru, Vec2i* coord);  // left down  /  right up
    void DeleteTree(ftree* curtree);
    bool JudgeRender(ftree* tree, Vec2i ld, Vec2i ru, Vec2i* coord, double deep);
    void BuildTree(ftree* curtree, Vec2i ld, Vec2i ru);
    void UpdateTreeNode(ftree* curtree, Vec2i ld, Vec2i ru, Vec2i& target);
    void UpdateTree(ftree* curtree, Vec2i ld, Vec2i ru);
    virtual int RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh) override;  // 绘制所有的物体
    virtual void ClearDeepBuffer() override;
};


#endif