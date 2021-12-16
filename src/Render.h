#ifndef _RENDER_H_
#define _RENDER_H_

#include<iostream>
#include<vector>
#include"BaseHead.h"
#include "MeshIO.h"

/**
 * @brief 绘制基类，用于给普通z-buffer、扫描线z-buffer、层次z-buffer、八叉树优化的z-buffer作为基类使用
 * 
 */
class Render{
public:

    int width_;
    int height_;
    unsigned char* framebuffer_; // 帧缓存
    Vector4unchar bkcolor_;
    

    Render();
    Render(int width, int height, Vector4unchar bkcolor);
    virtual ~Render();

    void ClearFrameBuffer(); // 清理帧缓存

    virtual int RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh) = 0;  // 绘制所有的物体
    virtual void ClearDeepBuffer() = 0;  // 清理所有的Z缓存
};


#endif
