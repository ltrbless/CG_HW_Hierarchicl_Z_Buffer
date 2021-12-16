#ifndef _ZBUFFER_
#define _ZBUFFER_

#include"Eigen/Dense"
#include"BaseHead.h"
#include "Render.h"

class ZBufferRender : public Render
{
public:
    double* deepbuffer_;

    ZBufferRender(){};
    ZBufferRender(int width, int height, Vector4unchar nkcolor);
    ~ZBufferRender();

    bool RenderTriangle(Vec2i* twodimcoord, Vec3d& deep, Vector4unchar& color);

    virtual int RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh) override;  // 绘制所有的物体
    virtual void ClearDeepBuffer() override;
};

#endif