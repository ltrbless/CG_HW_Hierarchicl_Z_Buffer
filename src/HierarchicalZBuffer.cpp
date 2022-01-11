#include"HierarchicalZBuffer.h"


HZBuffer::HZBuffer()
{

}

HZBuffer::HZBuffer(int width, int height, Vector4unchar nkcolor) : Render(width, height, nkcolor)
{
    this->deepbuffertree_ = new ftree();
    this->deepbuffer_ = new double[width * height];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            this->deepbuffer_[i * width + j] = initdeep;
        }
    }
    BuildTree(this->deepbuffertree_, Vec2i(0, 0), Vec2i(width - 1, height - 1));
}

HZBuffer::~HZBuffer()
{
    delete(this->deepbuffer_);
    DeleteTree(deepbuffertree_);
}

void HZBuffer::DeleteTree(ftree* curtree)
{
    if(curtree->t1 == nullptr && curtree->t2 == nullptr && curtree->t3 == nullptr && curtree->t4 == nullptr )
    {
        delete curtree;
        return;
    }
    if(curtree->t1 != nullptr) DeleteTree(curtree->t1);
    if(curtree->t2 != nullptr) DeleteTree(curtree->t2);
    if(curtree->t3 != nullptr) DeleteTree(curtree->t3);
    if(curtree->t4 != nullptr) DeleteTree(curtree->t4);
    return ;
}

void HZBuffer::ClearDeepBuffer()
{
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            this->deepbuffer_[i * width_ + j] = initdeep;
        }
    }
}


void HZBuffer::UpdateTreeNode(ftree* curtree, Vec2i& ld, Vec2i& ru, Vec2i& target)
{
    if(ld.x() == ru.x() && ld.y() == ru.y())
    {
        // std::cout << ld.x() << " " << ld.y() << '\n';
        curtree->deep = max(this->deepbuffer_[ld.x() + ld.y() * width_], curtree->deep);
        return ; 
    }
    if(ld.x() > ru.x() || ld.y() > ru.y())
    {
        curtree->deep = -initdeep; // 应设置为最大值，为了避免产生印象
        return ;
    }

    int ldru2x = (ld.x() + ru.x()) / 2;
    int ldru2y = (ld.y() + ru.y()) / 2;

    Vec2i ld_1 = Vec2i( ldru2x + 1,  ldru2y + 1 ); // 
    Vec2i ru_1 = Vec2i( ru.x(),      ru.y()     );

    Vec2i ld_2 = Vec2i( ld.x(),       ldru2y + 1);
    Vec2i ru_2 = Vec2i( ldru2x,       ru.y()    );

    Vec2i ld_3 = Vec2i( ld.x(),       ld.y()    );
    Vec2i ru_3 = Vec2i( ldru2x,       ldru2y    );

    Vec2i ld_4 = Vec2i( ldru2x + 1,   ld.y()    );
    Vec2i ru_4 = Vec2i( ru.x(),       ldru2y    ); 

    // std::cout << ld_1.x() << " " << ld_1.y() << " " << ru_1.x() << " " << ru_1.y() << std::endl;
    // std::cout << ld_2.x() << " " << ld_2.y() << " " << ru_2.x() << " " << ru_2.y() << std::endl;
    // std::cout << ld_3.x() << " " << ld_3.y() << " " << ru_3.x() << " " << ru_3.y() << std::endl;
    // std::cout << ld_4.x() << " " << ld_4.y() << " " << ru_4.x() << " " << ru_4.y() << std::endl;
    if(ld_1.x() <= target.x() && ld_1.y() <= target.y() && target.x() <= ru_1.x() && target.y() <= ru_1.y())
    UpdateTreeNode(curtree->t1, ld_1, ru_1, target);

    if(ld_2.x() <= target.x() && ld_2.y() <= target.y() && target.x() <= ru_2.x() && target.y() <= ru_2.y())
    UpdateTreeNode(curtree->t2, ld_2, ru_2, target);

    if(ld_3.x() <= target.x() && ld_3.y() <= target.y() && target.x() <= ru_3.x() && target.y() <= ru_3.y())
    UpdateTreeNode(curtree->t3, ld_3, ru_3, target);

    if(ld_4.x() <= target.x() && ld_4.y() <= target.y() && target.x() <= ru_4.x() && target.y() <= ru_4.y())
    UpdateTreeNode(curtree->t4, ld_4, ru_4, target);

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}



void HZBuffer::UpdateTree(ftree* curtree, Vec2i& ld, Vec2i& ru)
{
    if(ld.x() == ru.x() && ld.y() == ru.y())
    {
        // std::cout << ld.x() << " " << ld.y() << '\n';
        curtree->deep = max(this->deepbuffer_[ld.x() + ld.y() * width_], curtree->deep);
        return ; 
    }
    if(ld.x() > ru.x() || ld.y() > ru.y())
    {
        curtree->deep = -initdeep; // 应设置为最大值，为了避免产生印象
        return ;
    }

    int ldru2x = (ld.x() + ru.x()) / 2;
    int ldru2y = (ld.y() + ru.y()) / 2;

    Vec2i ld_1 = Vec2i( ldru2x + 1,  ldru2y + 1 ); // 
    Vec2i ru_1 = Vec2i( ru.x(),      ru.y()     );

    Vec2i ld_2 = Vec2i( ld.x(),       ldru2y + 1);
    Vec2i ru_2 = Vec2i( ldru2x,       ru.y()    );

    Vec2i ld_3 = Vec2i( ld.x(),       ld.y()    );
    Vec2i ru_3 = Vec2i( ldru2x,       ldru2y    );

    Vec2i ld_4 = Vec2i( ldru2x + 1,   ld.y()    );
    Vec2i ru_4 = Vec2i( ru.x(),       ldru2y    ); 

        

    
    // std::cout << ld_1.x() << " " << ld_1.y() << " " << ru_1.x() << " " << ru_1.y() << std::endl;
    // std::cout << ld_2.x() << " " << ld_2.y() << " " << ru_2.x() << " " << ru_2.y() << std::endl;
    // std::cout << ld_3.x() << " " << ld_3.y() << " " << ru_3.x() << " " << ru_3.y() << std::endl;
    // std::cout << ld_4.x() << " " << ld_4.y() << " " << ru_4.x() << " " << ru_4.y() << std::endl;

    UpdateTree(curtree->t1, ld_1, ru_1);
    UpdateTree(curtree->t2, ld_2, ru_2);
    UpdateTree(curtree->t3, ld_3, ru_3);
    UpdateTree(curtree->t4, ld_4, ru_4);

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}

void HZBuffer::BuildTree(ftree* curtree, Vec2i& ld, Vec2i& ru)
{
    // std::cout << "**********************\n";
    // std::cout << ld.x() << " " << ld.y() << " " << ru.x() << " " << ru.y() << std::endl;
    if(ld.x() == ru.x() && ld.y() == ru.y())
    {
        curtree->deep = this->deepbuffer_[ld.x() + ld.y() * width_];
        return ; 
    }
    if(ld.x() > ru.x() || ld.y() > ru.y())
    {
        // std::cout << "Error HierarchicalZBuffer.cpp 161 line\n";
        curtree->deep = -initdeep; // 应设置为最大值，为了避免产生印象
        return ;
    }
    
    int ldru2x = (ld.x() + ru.x()) / 2;
    int ldru2y = (ld.y() + ru.y()) / 2;

    Vec2i ld_1 = Vec2i( ldru2x + 1,  ldru2y + 1 ); // 
    Vec2i ru_1 = Vec2i( ru.x(),      ru.y()     );

    Vec2i ld_2 = Vec2i( ld.x(),       ldru2y + 1);
    Vec2i ru_2 = Vec2i( ldru2x,       ru.y()    );

    Vec2i ld_3 = Vec2i( ld.x(),       ld.y()    );
    Vec2i ru_3 = Vec2i( ldru2x,       ldru2y    );

    Vec2i ld_4 = Vec2i( ldru2x + 1,   ld.y()    );
    Vec2i ru_4 = Vec2i( ru.x(),       ldru2y    ); 

    // std::cout << ld_1.x() << " " << ld_1.y() << " " << ru_1.x() << " " << ru_1.y() << std::endl;
    // std::cout << ld_2.x() << " " << ld_2.y() << " " << ru_2.x() << " " << ru_2.y() << std::endl;
    // std::cout << ld_3.x() << " " << ld_3.y() << " " << ru_3.x() << " " << ru_3.y() << std::endl;
    // std::cout << ld_4.x() << " " << ld_4.y() << " " << ru_4.x() << " " << ru_4.y() << std::endl;

    // abort();

    curtree->t1 = new ftree();
    curtree->t2 = new ftree();
    curtree->t3 = new ftree();
    curtree->t4 = new ftree();
    BuildTree(curtree->t1, ld_1, ru_1);
    BuildTree(curtree->t2, ld_2, ru_2);
    BuildTree(curtree->t3, ld_3, ru_3);
    BuildTree(curtree->t4, ld_4, ru_4);

    // std::cout << "-----------------------\n";

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}

// {

//     if(width == 0 && height == 0)
//     {
//         curtree->deep = initdeep;
//         return ;
//     }

//     int m_width = width / 2;
//     int m_height = height / 2;

//     curtree->t1 = new ftree();
//     curtree->t2 = new ftree();
//     curtree->t3 = new ftree();
//     curtree->t4 = new ftree();
//     BuildTree(curtree->t1, width - m_width, height - m_height, initdeep);
//     BuildTree(curtree->t2,         m_width, height - m_height, initdeep);
//     BuildTree(curtree->t3,         m_width,          m_height, initdeep);
//     BuildTree(curtree->t4, width - m_width,          m_height, initdeep);

//     curtree->deep = min4( curtree->t1->deep, curtree->t2->deep, curtree->t3->deep, curtree->t4->deep );

//     return ;
// }

inline bool HZBuffer::JudgeTriInRec(Vec2i& ld, Vec2i& ru, Vec2i* coord)
{
    bool f = 1;
    for(int i = 0; i < 3; i++)
    {
        if(ld.x() <= coord[i].x() && coord[i].x() <= ru.x() 
        && ld.y() <= coord[i].y() && coord[i].y() <= ru.y()){
            continue;
        }else{
            f = 0;
            break;
        }
    }
    return f;
}

bool HZBuffer::JudgeRender(ftree* tree, Vec2i& ld, Vec2i& ru, Vec2i* coord, double deep)
{
    if( tree->deep >= deep ) 
    {
        return 0;  // 如果被完全遮挡则不用绘制
    }

    int ldru2x = (ld.x() + ru.x()) / 2;
    int ldru2y = (ld.y() + ru.y()) / 2;

    Vec2i ld_1 = Vec2i( ldru2x + 1,  ldru2y + 1 ); // 
    Vec2i ru_1 = Vec2i( ru.x(),      ru.y()     );

    Vec2i ld_2 = Vec2i( ld.x(),       ldru2y + 1);
    Vec2i ru_2 = Vec2i( ldru2x,       ru.y()    );

    Vec2i ld_3 = Vec2i( ld.x(),       ld.y()    );
    Vec2i ru_3 = Vec2i( ldru2x,       ldru2y    );

    Vec2i ld_4 = Vec2i( ldru2x + 1,   ld.y()    );
    Vec2i ru_4 = Vec2i( ru.x(),       ldru2y    ); 

    // std::cout << "This layer is " << deep << " th\n";
    // std::cout << ld.x() << " " << ld.y() << " * " << ru.x() << " " << ru.y() << '\n';
    // std::cout << ld_1.x() << " " << ld_1.y() << " * " << ru_1.x() << " " << ru_1.y() << '\n';
    // std::cout << ld_2.x() << " " << ld_2.y() << " * " << ru_2.x() << " " << ru_2.y() << '\n';
    // std::cout << ld_3.x() << " " << ld_3.y() << " * " << ru_3.x() << " " << ru_3.y() << '\n';
    // std::cout << ld_4.x() << " " << ld_4.y() << " * " << ru_4.x() << " " << ru_4.y() << '\n';

    int j1 = JudgeTriInRec(ld_1, ru_1, coord);
    if(j1){
        return JudgeRender(tree->t1, ld_1, ru_1, coord, deep);
    } 
    int j2 = JudgeTriInRec(ld_2, ru_2, coord);
    if(j2){
        return JudgeRender(tree->t2, ld_2, ru_2, coord, deep);
    }
    int j3 = JudgeTriInRec(ld_3, ru_3, coord);
    if(j3){
        return JudgeRender(tree->t3, ld_3, ru_3, coord, deep);
    }
    int j4 = JudgeTriInRec(ld_4, ru_4, coord);
    if(j4){
        return JudgeRender(tree->t4, ld_4, ru_4, coord, deep);
    }
    return 1; // 当不存在包含该三角形的层次时，返回
}

int HZBuffer::RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh)
{
    // Mat3 view = getlookatmatrix(eye, Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 1.0, 0.0));
    // lookatafter(alltrimesh, view);
    int saverender = 0;
    for(int i = 0; i < alltrimesh.size(); i++)  // 拿出来一个体
    {
        TriMesh curmesh = alltrimesh[i];
        
        for(int j = 0; j < curmesh.vecTopos.size(); j++)  // 拿出来一个三角形
        {
            Vec3i curtritopo = curmesh.vecTopos[j];
            Vec2i screenCoord[3];
            Vec3d deep;
            double deep_max = initdeep;
            Vec3d spaceCoord[3];
            for(int k = 0; k < 3; k++)  // 把当前三角形的所有的点转换到二维的屏幕坐标中
            {
                Vec3d curPoint = curmesh.vecCoords[curtritopo[k] - 1]; // 由于obj文件，三角拓扑索引是从 1 开始
                spaceCoord[k].x() = curPoint.x();
                spaceCoord[k].y() = curPoint.y();
                spaceCoord[k].z() = curPoint.z();
                screenCoord[k](0) = width_ * (curPoint(0) + 1.0) / 2;
                screenCoord[k](1) = height_ * (curPoint(1) + 1.0) / 2;
                deep[k] = curPoint.z();
                deep_max = max(deep_max, deep[k]);
            }
            
            Vec3d trinormal = (spaceCoord[1] - spaceCoord[0]).cross(spaceCoord[2] - spaceCoord[0]);
            trinormal.normalize();
            if(trinormal.dot(Vec3d(0, 0, 1)) <= 0.0001) continue; // 背面剔除 + 排除法向与视点方向垂直的三角形
            double costh = trinormal.dot(Vec3d(0,0,1));
            Vector4unchar tmp( curmesh.color.x() * costh, curmesh.color.y() * costh, curmesh.color.z() * costh, 255 );

            Vec2i ldt, rut;
            bool b_render = JudgeRender(deepbuffertree_, Vec2i(0, 0), Vec2i(width_ - 1, height_ - 1), screenCoord, deep_max);

            if(b_render) 
            {
                RenderTriangle(screenCoord, deep, tmp);
            }
            else{
                saverender++;
            }
        }
    }
    std::cout << "All avoid render triangle is " << saverender << '\n';
    return 1;
}

bool HZBuffer::RenderTriangle(Vec2i* coord, Vec3d& deep, Vector4unchar& color)
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
                UpdateTreeNode(this->deepbuffertree_, Vec2i(0,0), Vec2i(width_ - 1, height_ - 1), Vec2i(x, y));
                // abort();
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
                UpdateTreeNode(this->deepbuffertree_, Vec2i(0,0), Vec2i(width_ - 1, height_ - 1), Vec2i(x, y));
                framebuffer_[ (x + width_ * y) * 3 + 0] = color.x(); 
                framebuffer_[ (x + width_ * y) * 3 + 1] = color.y(); 
                framebuffer_[ (x + width_ * y) * 3 + 2] = color.z(); 
            }
        }
    }
    return 1;
}


// virtual void ClearDeepBuffer() override;