#include"ScanLineZBuffer.h"


ScanlineZBuffer::ScanlineZBuffer()
{

}

ScanlineZBuffer::ScanlineZBuffer(int width, int height, Vector4unchar bkcolor) : Render(width, height, bkcolor)
{
    this->deepbuffer_ = new double[width * height];
    this->triTableList_ = new std::vector<TriTable>[height];  
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            this->deepbuffer_[i * width + j] = -1000000;
        }
    }

}

ScanlineZBuffer::~ScanlineZBuffer()
{
    delete deepbuffer_;
}


void ScanlineZBuffer::ClearDeepBuffer()
{
    for(int i = 0; i < this->height_; i++)
    {
        for(int j = 0; j < this->width_; j++)
        {
            this->deepbuffer_[i * width_ + j] = -1000000;
        }
    }
}

bool ScanlineZBuffer::CreateTriTable(std::vector<TriMesh>& alltrimesh)
{
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
                // std::cout << "------------------\n";
                // std::cout << width_ << " " << height_ << " " << curPoint << " " << screenCoord[k].x() << " " << screenCoord[k].y() << '\n';
                deep[k] = curPoint.z();
            }
            
            Vec3d trinormal = (spaceCoord[1] - spaceCoord[0]).cross(spaceCoord[2] - spaceCoord[0]);
            trinormal.normalize();
            if(trinormal.dot(Vec3d(0, 0, 1)) <= 0.0001) continue; // 背面剔除 + 排除法向与视点方向垂直的三角形
            double costh = trinormal.dot(Vec3d(0,0,1));
            Vector4unchar curtricolor( curmesh.color.x() * costh, curmesh.color.y() * costh, curmesh.color.z() * costh, 255 );

            // 从小到大排序
            if(screenCoord[0](1) > screenCoord[1](1))
            {
                std::swap(screenCoord[0], screenCoord[1]);
                std::swap(deep[0], deep[1]);
            }
            if(screenCoord[0](1) > screenCoord[2](1))
            {
                std::swap(screenCoord[0], screenCoord[2]);
                std::swap(deep[0], deep[2]);
            }
            if(screenCoord[1](1) > screenCoord[2](1))
            {
                std::swap(screenCoord[1], screenCoord[2]);
                std::swap(deep[1], deep[2]);
            }

            Vec2i* coord = screenCoord; // 为了代码重用做的替换

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


            TriTable curTriTable;
            curTriTable.e_01.x = start_01;
            curTriTable.e_01.z = deep_01;
            curTriTable.e_01.dx = dx_01;
            curTriTable.e_01.dz = dz_01;
            curTriTable.e_01.len_y = t_01;

            curTriTable.e_02.x = start_02;
            curTriTable.e_02.z = deep_02;
            curTriTable.e_02.dx = dx_02;
            curTriTable.e_02.dz = dz_02;
            curTriTable.e_02.len_y = t_02;

            curTriTable.e_12.x = start_12;
            curTriTable.e_12.z = deep_12;
            curTriTable.e_12.dx = dx_12;
            curTriTable.e_12.dz = dz_12;
            curTriTable.e_12.len_y = t_12;

            curTriTable.color = curtricolor;
            // std::cout << "t1 " << this->triTableList_->size() << " " << coord[0].x() << " " << coord[0].y() << "\n"; 
            this->triTableList_[coord[0].y()].push_back(curTriTable);

            // std::cout << "*************** \n";
            // std::cout << curTriTable.e_01.x << " " << curTriTable.e_02.x << " " << curTriTable.e_12.x << "\n";
        }
    }
    return 1;
}

int ScanlineZBuffer::RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh)
{
    // Mat3 view = getlookatmatrix(eye, Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 1.0, 0.0));
    // lookatafter(alltrimesh, view);
    CreateTriTable(alltrimesh); // 构建面表
    ClearDeepBuffer();

    

    std::list<ActiveEdgeTable> activeEdgeList;
    for(int line = 0; line < height_; line++)
    {
        for(inr i = 0; i < triTableList_[line].size(); i++) // 遍历所有的活化面表
        {
            TriTable curtri = triTableList_[line][i];
            ActiveEdgeTable curActiveEdgeTable;

            curActiveEdgeTable.lx  = curtri.e_01.x; // 这里的左右不一定是正确的
            curActiveEdgeTable.lz  = curtri.e_01.z;
            curActiveEdgeTable.dlx = curtri.e_01.dx;
            curActiveEdgeTable.dlz = curtri.e_01.dz;
            

            curActiveEdgeTable.rx  = curtri.e_02.x; // 这里的左右不一定是正确的
            curActiveEdgeTable.rz  = curtri.e_02.z;
            curActiveEdgeTable.drx = curtri.e_02.dx;
            curActiveEdgeTable.drz = curtri.e_02.dz;

            curActiveEdgeTable.dy = min(curtri.e_01.len_y, curtri.e_02.len_y);
            curActiveEdgeTable.len = curActiveEdgeTable.dy;
            curActiveEdgeTable.used = 0;
            curActiveEdgeTable.tri = curtri;
            // std::cout << "Curtri is :\n";
            // std::cout << curtri.e_12.x << "\n";
            activeEdgeList.push_back(curActiveEdgeTable); 

        }

			// // if (line == 150)
			// // {
			// // 	std::cout << "Here\n";
			// // 	std::cout << activeEdgeList.size() << '\n';

			// }
        
        std::list<ActiveEdgeTable>::iterator ptr;
        // if(activeEdgeList.size() != 0)
        //     std::cout << "-------------------------------------\n";
        for(ptr = activeEdgeList.begin(); ptr != activeEdgeList.end();)
        {
            // std::cout << ptr->lx << " " << ptr->rx << " " << ptr->len << '\n';

            if(ptr->len != 1)
            {
                int l = min( (int)(ptr->lx + 0.5), int(ptr->rx + 0.5) );
                int r = max( (int)(ptr->lx + 0.5), int(ptr->rx + 0.5) );
                        
                double dz;
                double curdeep;
                if( int(ptr->lx + 0.5) > int(ptr->rx + 0.5))
                {
                    dz = (ptr->lz - ptr->rz) / (r - l);
                    curdeep = ptr->rz;
                }
                else{
                    dz = (ptr->rz - ptr->lz) / (r - l);
                    curdeep = ptr->lz;
                }

                for(int x = l; x <= r; x++) //
                {
                    if(x != l) curdeep += dz;
                    int y = line;
                    if( y * width_ + x < width_ * height_ && deepbuffer_[ y * width_ + x ] < curdeep ) // 对比深度缓存以更新帧缓存
                    {
                        deepbuffer_[x + width_ * y] = curdeep;
                        framebuffer_[ (x + width_ * y) * 3 + 0] = ptr->tri.color.x(); 
                        framebuffer_[ (x + width_ * y) * 3 + 1] = ptr->tri.color.y(); 
                        framebuffer_[ (x + width_ * y) * 3 + 2] = ptr->tri.color.z(); 
                    }
                }

                // std::cout << " * " << ptr->lx << " " << ptr->dlx << " " << ptr->dy << '\n';
                ptr->lx += ptr->dlx;
                ptr->rx += ptr->drx;
                ptr->lz += ptr->dlz;
                ptr->rz += ptr->drz;
                
                ptr->dy--; // 预计长度
            }

            if(ptr->used == 0 && (ptr->len == 1 || ptr->dy == 0)) // 加进来一条边
            {
                // std::cout << " - " << ptr->lx << " " << ptr->tri.e_12.x << " " << ptr->tri.e_12.len_y << '\n';
                ptr->lx  = ptr->tri.e_12.x;
                ptr->lz  = ptr->tri.e_12.z;
                ptr->dlx = ptr->tri.e_12.dx;
                ptr->dlz = ptr->tri.e_12.dz;
                ptr->dy  = ptr->tri.e_12.len_y;
                ptr->len = ptr->tri.e_12.len_y;
                ptr->used = 1;
                ptr++;
            }
            else if(ptr->used == 1 && (ptr->len == 1 || ptr->dy == 0))
            {
                ptr = activeEdgeList.erase(ptr);
            }
            else{
                ptr++;
            }
        }
    }
    return 1;
}