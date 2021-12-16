#include "ZBuffer.h"
#include "MeshIO.h"
#include "Tool.h"

ZBufferRender::ZBufferRender(int width, int height, Vector4unchar bkcolor):Render(width, height, bkcolor){
    this->deepbuffer_= new double[width * height];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            this->deepbuffer_[i * width + j] = -10000.0; 
        }
    }
}

ZBufferRender::~ZBufferRender()
{
    delete deepbuffer_;
}

void ZBufferRender::ClearDeepBuffer()
{
    for(int i = 0; i < this->height_; i++)
    {
        for(int j = 0; j < this->width_; j++)
        {
            this->deepbuffer_[i * this->width_ + j] = -10000.0;
        }
    }
}

int ZBufferRender::RenderAllObj(Vec3d eye, std::vector<TriMesh>& alltrimesh) 
{
    // Mat3 view = getlookatmatrix(eye, Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 1.0, 0.0));
    // lookatafter(alltrimesh, view);
	std::cout << "VecTopo number is " << alltrimesh[0].vecTopos.size();
    for(int i = 0; i < alltrimesh.size(); i++)  // 拿出来一个体
    {
        TriMesh curmesh = alltrimesh[i];
        
        for(int j = 0; j < curmesh.vecTopos.size(); j++)  // 拿出来一个三角形
        {
            Vec3i curtritopo = curmesh.vecTopos[j];
            Vec2i screenCoord[3];
            Vec3d deep;
            Vec3d spaceCoord[3];
            for(int k = 0; k < 3; k++)  // 把当前三角形的所有的点转换到二维的屏幕坐标中
            {
                Vec3d curPoint = curmesh.vecCoords[curtritopo[k] - 1]; // 由于obj文件，三角拓扑索引是从 1 开始
                spaceCoord[k].x() = curPoint.x();
                spaceCoord[k].y() = curPoint.y();
                spaceCoord[k].z() = curPoint.z();
                screenCoord[k](0) = width_ * (curPoint(0) + 1.0) / 2;
                screenCoord[k](1) = height_ * (curPoint(1) + 1.0) / 2;
                // std::cout << screenCoord[k].x() << " " << screenCoord[k].y() << '\n';
                deep[k] = curPoint.z();
            }
            
            Vec3d trinormal = (spaceCoord[1] - spaceCoord[0]).cross(spaceCoord[2] - spaceCoord[0]);
            trinormal.normalize();
            if(trinormal.dot(Vec3d(0, 0, 1)) <= 0.0001) continue; // 背面剔除 + 排除法向与视点方向垂直的三角形
            double costh = trinormal.dot(Vec3d(0,0,1));
            // std::cout << costh << "\n";
            Vector4unchar tmp( curmesh.color.x() * costh, curmesh.color.y() * costh, curmesh.color.z() * costh, 255 );
            // std::cout << (int)(tmp.x()) << " " << (int)tmp.y() << " " << (int)tmp.z() << " " << (int)tmp.w() << '\n';
            RenderTriangle(screenCoord, deep, tmp);
        }
    }
    return 1;
}

bool ZBufferRender::RenderTriangle(Vec2i* coord, Vec3d& deep, Vector4unchar& color)
{
    // 先确定点的顺序，从下到上排序
    if(coord[0](1) > coord[1](1))
    {
        std::swap(coord[0], coord[1]);
        std::swap(deep[0], deep[1]);
    }
    if(coord[0](1) > coord[2](1))
    {
        std::swap(coord[0], coord[2]);
        std::swap(deep[0], deep[2]);
    }
    if(coord[1](1) > coord[2](1))
    {
        std::swap(coord[1], coord[2]);
        std::swap(deep[1], deep[2]);
    }

    // 先绘制 0-1 线段
    int t_01 = coord[1].y() - coord[0].y() + 1;
    int t_02 = coord[2].y() - coord[0].y() + 1;
    int t_12 = coord[2].y() - coord[1].y() + 1;

    // if(1 == t_01 && t_01 == t_02 && t_02 == t_12) return 0; // 因为通过法向排除了，所以这里不用再判定了，法向判定更加广泛
    // 所以，只有 t_01 和 t_12 可能为1, t_02一定大于 1. 为了代码统一都判断了下

    double dx_01 = 0, dx_02 = 0, dx_12 = 0;
    if(t_01 != 1) dx_01 = ( coord[1].x() - coord[0].x() ) / double(t_01 - 1); // 注意这里是错误的，t - 1 可能为0，一会处理
    if(t_02 != 1) dx_02 = ( coord[2].x() - coord[0].x() ) / double(t_02 - 1); // 注意这里是错误的，t - 1 可能为0，一会处理
    if(t_12 != 1) dx_12 = ( coord[2].x() - coord[1].x() ) / double(t_12 - 1); // 注意这里是错误的，t - 1 可能为0，一会处理

    double dz_01 = 0, dz_02 = 0, dz_12 = 0;
    if(t_01 != 1) dz_01 = (deep[1] - deep[0]) / (t_01 - 1); // 沿着线段方向的变化 Delta
    if(t_02 != 1) dz_02 = (deep[2] - deep[0]) / (t_02 - 1);
    if(t_12 != 1) dz_12 = (deep[2] - deep[1]) / (t_12 - 1);
    
    double start_02 = coord[0].x();  // 坐标起点x
    double start_01 = coord[0].x();
    double start_12 = coord[1].x();

    double deep_01 = deep[0];  // 深度值起点
    double deep_02 = deep[0];
    double deep_12 = deep[1];

    if(t_01 > 1)
    for(int line = 0; line < t_01; line++) // 第几条扫描线
    {
        start_01 = start_01 + dx_01 * (line != 0); 
        start_02 = start_02 + dx_02 * (line != 0);

        deep_01 = deep_01 + dz_01 * (line != 0);
        deep_02 = deep_02 + dz_02 * (line != 0);

        int l = min( int(start_01 + 0.5), int(start_02 + 0.5)); // 四舍五入
        int r = max( int(start_01 + 0.5), int(start_02 + 0.5));


        double dz;
        double curdeep;
        if(start_01 > start_02)
        {
            dz = (deep_01 - deep_02) / (r - l);
            curdeep = deep_02;
        }
        else{
            dz = (deep_02 - deep_01) / (r - l);
            curdeep = deep_01;
        }

        for(int x = l; x <= r; x++) //
        {
            if(x != l) curdeep += dz;
            int y = coord[0].y() + line;
            if( y * width_ + x < width_ * height_ && deepbuffer_[ y * width_ + x ] < curdeep ) // 对比深度缓存以更新帧缓存
            {
                deepbuffer_[x + width_ * y] = curdeep;
                framebuffer_[ (x + width_ * y) * 3 + 0] = color.x(); 
                framebuffer_[ (x + width_ * y) * 3 + 1] = color.y(); 
                framebuffer_[ (x + width_ * y) * 3 + 2] = color.z(); 
            }
        }
    }

    for (int line = 1; line < t_12; line++)
    {
        start_12 = start_12 + dx_12;
        start_02 = start_02 + dx_02;

        deep_12 = deep_12 + dz_12;
        deep_02 = deep_02 + dz_02;

        int l = min(int(start_12 + 0.5), int(start_02 + 0.5));
        int r = max(int(start_12 + 0.5), int(start_02 + 0.5));

        double dz;
        double curdeep;
        if(start_12 > start_02)
        {
            dz = (deep_12 - deep_02) / (r - l);
            curdeep = deep_02;
        }
        else{
            dz = (deep_02 - deep_12) / (r - l);
            curdeep = deep_12;
        }
        
        for(int x = l; x <= r; x++) //
        {
            if(x != l) curdeep += dz;
            int y = coord[1].y() + line;
            if(y * width_ + x < width_ * height_ && deepbuffer_[ y * width_ + x ] < curdeep ) // 对比深度缓存以更新帧缓存
            {
                deepbuffer_[x + width_ * y] = curdeep;
                framebuffer_[ (x + width_ * y) * 3 + 0] = color.x(); 
                framebuffer_[ (x + width_ * y) * 3 + 1] = color.y(); 
                framebuffer_[ (x + width_ * y) * 3 + 2] = color.z(); 
            }
        }
    }
    return 1;
}
