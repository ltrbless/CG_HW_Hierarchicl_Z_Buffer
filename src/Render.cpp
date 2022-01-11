#include<iostream>
#include"Render.h"
#include "Tool.h"

Render::Render()
{
    
}

Render::~Render()
{
    delete framebuffer_;
}

void Render::ClearFrameBuffer()
{
    std::cout << "Clear\n";
    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < height_; j++)
        {
            framebuffer_[ (i * height_ + j) * 3 + 0 ] = bkcolor_.x();
            framebuffer_[ (i * height_ + j) * 3 + 1 ] = bkcolor_.y();
            framebuffer_[ (i * height_ + j) * 3 + 2 ] = bkcolor_.z();
        }
    }
}

Render::Render(int width, int height, Vector4unchar bkcolor) : width_(width), height_(height), bkcolor_(bkcolor) {
    framebuffer_ = new unsigned char [width * height * 3];
    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < height_; j++)
        {
            framebuffer_[ (i * height_ + j) * 3 + 0 ] = bkcolor_.x();
            framebuffer_[ (i * height_ + j) * 3 + 1 ] = bkcolor_.y();
            framebuffer_[ (i * height_ + j) * 3 + 2 ] = bkcolor_.z();
        }
    }
};



