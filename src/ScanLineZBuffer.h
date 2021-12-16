#ifndef _SCANLINEZBUFFER_
#define _SCANLINEZBUFFER_

#include"BaseHead.h"
#include"Render.h"

class ScanlineZBuffer : public Render
{
public :
    double* deepbuffer_;
    std::vector<TriTable>* triTableList_;

    ScanlineZBuffer();
    ScanlineZBuffer(int width, int height, Vector4unchar bkcolor);
    ~ScanlineZBuffer();

    bool CreateTriTable(std::vector<TriMesh>& alltrimesh);
    virtual int RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh) override;
    virtual void ClearDeepBuffer() override;
};



#endif