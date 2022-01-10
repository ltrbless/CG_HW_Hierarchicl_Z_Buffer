#include "OctreeHZBuffer.h"

int insertnode = 0;

OctHZBuffer::OctHZBuffer()
{

}

OctHZBuffer::OctHZBuffer(int width, int height, Vector4unchar nkcolor, int treedeep) : Render(width, height, nkcolor)
{
    this->treedeep_ = treedeep;
    this->deepbuffertree_ = new ftree();
    this->deepbufferoctree_ = new octree();
    this->deepbuffer_ = new double[width * height];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            this->deepbuffer_[i * width + j] = initdeep;
        }
    }
    // std::cout << "This sis construct fun.\n";
    BuildQuadTree(this->deepbuffertree_, Vec2i(0, 0), Vec2i(width - 1, height - 1)); // 构建并且初始化初始值
    // std::cout << "buildQuadTree is success\n";
    InitOctree(this->deepbufferoctree_, 1); // 仅仅初始化构建，并不初始化值
    // std::cout << "deepbuffertree is success\n";
}

OctHZBuffer::~OctHZBuffer()
{
    delete(this->deepbuffer_);
    DeleteQuadTree(deepbuffertree_);
    
}

void OctHZBuffer::DeleteQuadTree(ftree* curtree)
{
    if(curtree->t1 == nullptr && curtree->t2 == nullptr && curtree->t3 == nullptr && curtree->t4 == nullptr )
    {
        delete curtree;
        return;
    }
    if(curtree->t1 != nullptr) DeleteQuadTree(curtree->t1);
    if(curtree->t2 != nullptr) DeleteQuadTree(curtree->t2);
    if(curtree->t3 != nullptr) DeleteQuadTree(curtree->t3);
    if(curtree->t4 != nullptr) DeleteQuadTree(curtree->t4);
    return ;
}

void OctHZBuffer::ClearDeepBuffer()
{
    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            this->deepbuffer_[i * width_ + j] = initdeep;
        }
    }
}


void OctHZBuffer::UpdateQuadTreeNode(ftree* curtree, Vec2i& ld, Vec2i& ru, Vec2i& target)
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
    UpdateQuadTreeNode(curtree->t1, ld_1, ru_1, target);
    if(ld_2.x() <= target.x() && ld_2.y() <= target.y() && target.x() <= ru_2.x() && target.y() <= ru_2.y())
    UpdateQuadTreeNode(curtree->t2, ld_2, ru_2, target);
    if(ld_3.x() <= target.x() && ld_3.y() <= target.y() && target.x() <= ru_3.x() && target.y() <= ru_3.y())
    UpdateQuadTreeNode(curtree->t3, ld_3, ru_3, target);
    if(ld_4.x() <= target.x() && ld_4.y() <= target.y() && target.x() <= ru_4.x() && target.y() <= ru_4.y())
    UpdateQuadTreeNode(curtree->t4, ld_4, ru_4, target);

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}



void OctHZBuffer::UpdateQuadTree(ftree* curtree, Vec2i& ld, Vec2i& ru)
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

        

    UpdateQuadTree(curtree->t1, ld_1, ru_1);
    UpdateQuadTree(curtree->t2, ld_2, ru_2);
    UpdateQuadTree(curtree->t3, ld_3, ru_3);
    UpdateQuadTree(curtree->t4, ld_4, ru_4);

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}

void OctHZBuffer::BuildQuadTree(ftree* curtree, Vec2i& ld, Vec2i& ru)
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
        std::cout << "Error \n";
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



    // abort();

    curtree->t1 = new ftree();
    curtree->t2 = new ftree();
    curtree->t3 = new ftree();
    curtree->t4 = new ftree();
    BuildQuadTree(curtree->t1, ld_1, ru_1);
    BuildQuadTree(curtree->t2, ld_2, ru_2);
    BuildQuadTree(curtree->t3, ld_3, ru_3);
    BuildQuadTree(curtree->t4, ld_4, ru_4);

    // std::cout << "-----------------------\n";

    curtree->deep = min4(curtree->t1->deep, curtree->t2->deep, curtree->t3->deep,curtree->t4->deep);    
}


inline bool OctHZBuffer::JudgeRecInRec(Vec2i& ld, Vec2i& ru, Vec2i* coord)
{
    bool f = 1;
    for(int i = 0; i < 2; i++)
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

bool OctHZBuffer::JudgeRender(ftree* tree, Vec2i& ld, Vec2i& ru, Vec2i* coord, double deep)
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


    int j1 = JudgeRecInRec(ld_1, ru_1, coord);
    if(j1){
        return JudgeRender(tree->t1, ld_1, ru_1, coord, deep);
    } 
    int j2 = JudgeRecInRec(ld_2, ru_2, coord);
    if(j2){
        return JudgeRender(tree->t2, ld_2, ru_2, coord, deep);
    }
    int j3 = JudgeRecInRec(ld_3, ru_3, coord);
    if(j3){
        return JudgeRender(tree->t3, ld_3, ru_3, coord, deep);
    }
    int j4 = JudgeRecInRec(ld_4, ru_4, coord);
    if(j4){
        return JudgeRender(tree->t4, ld_4, ru_4, coord, deep);
    }
    return 1; // 当不存在包含该矩形的层次时，返回
}

int OctHZBuffer::RenderAllObj(Vec3d view, std::vector<TriMesh>& alltrimesh)
{
    // Mat3 view = getlookatmatrix(eye, Vec3d(0.0, 0.0, 0.0), Vec3d(0.0, 1.0, 0.0));
    // lookatafter(alltrimesh, view);
    double Init = 10000;
    Vec3d minnn = Vec3d(Init, Init, Init);
    Vec3d maxxx = Vec3d(-Init, -Init, -Init);
    for(int i = 0; i < alltrimesh.size(); i++)
    {
        for(int j = 0; j < alltrimesh[i].vecCoords.size(); j++)
        {
            minnn.x() = min(minnn.x(), alltrimesh[i].vecCoords[j].x());
            minnn.y() = min(minnn.y(), alltrimesh[i].vecCoords[j].y());
            minnn.z() = min(minnn.z(), alltrimesh[i].vecCoords[j].z());

            maxxx.x() = max(maxxx.x(), alltrimesh[i].vecCoords[j].x());
            maxxx.y() = max(maxxx.y(), alltrimesh[i].vecCoords[j].y());
            maxxx.z() = max(maxxx.z(), alltrimesh[i].vecCoords[j].z());
        }
    }
    std::cout << "Octree box is" << minnn.x() << " " << minnn.y() << " " << minnn.z() << " | " << maxxx.x() << " " << maxxx.y() << " " << maxxx.z() << '\n';
    BuildOctree(alltrimesh, minnn, maxxx);
    clock_t endTime;
    endTime = clock();
    std::cout << "Build octree time is : "  << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "Build octree is success.\n";
    insertnode = 0;
    OctreeRender(this->deepbufferoctree_, minnn, maxxx, 1, 0);
    // OctreeRender(this->deepbufferoctree_, Vec3d(0, 0, 0), Vec3d(1.0, 1.0, 1.0), 1);
    endTime = clock();
    std::cout << "Build octreeRender time is : "  << " the run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
    std::cout << "insert node is " << insertnode << '\n';
    std::cout << "Render is success.\n";
    return 1;
}

bool OctHZBuffer::RenderTriangle(Vec2i* coord, Vec3d& deep, Vector4unchar& color)
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
                UpdateQuadTreeNode(this->deepbuffertree_, Vec2i(0,0), Vec2i(width_ - 1, height_ - 1), Vec2i(x, y));
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
                UpdateQuadTreeNode(this->deepbuffertree_, Vec2i(0,0), Vec2i(width_ - 1, height_ - 1), Vec2i(x, y));
                framebuffer_[ (x + width_ * y) * 3 + 0] = color.x(); 
                framebuffer_[ (x + width_ * y) * 3 + 1] = color.y(); 
                framebuffer_[ (x + width_ * y) * 3 + 2] = color.z(); 
            }
        }
    }
    return 1;
}

void OctHZBuffer::InitOctree(octree* curtree, int curdeep)
{
    if(curdeep > this->treedeep_)
    {
        return ;
    }

    for(int i = 0; i < 8; i++)
    {
        curtree->t[i] = new octree();
		InitOctree(curtree->t[i], curdeep + 1);
    }
	
	return;
}

void OctHZBuffer::BuildOctree(std::vector<TriMesh>& alltrimesh, Vec3d& minn, Vec3d& maxx)
{
    //  构建八叉树
    int all_tri = 0;
    for(int i = 0; i < alltrimesh.size(); i++)
    {
        TriMesh curmesh = alltrimesh[i];
        for(int j = 0; j < curmesh.vecTopos.size(); j++)
        {
            Vec3i curtritopo = curmesh.vecTopos[j];
            triNode curnode;
            for(int k = 0; k < 3; k++)
            {
                curnode.point[k] = (curmesh.vecCoords[curtritopo[k] - 1] + Vec3d(1.0, 1.0, 1.0)) / 2.0;  // 归一化到 0,0,0 - 1,1,1
            }
            Vec3d trinormal = (curnode.point[1] - curnode.point[0]).cross(curnode.point[2] - curnode.point[0]);
            trinormal.normalize();
            if(trinormal.dot(Vec3d(0, 0, 1)) <= 0.0001) continue; // 背面剔除 + 排除法向与视点方向垂直的三角形
            double costh = trinormal.dot(Vec3d(0,0,1));
            Vector4unchar tmp( curmesh.color.x() * costh, curmesh.color.y() * costh, curmesh.color.z() * costh, 255 );
            curnode.color = tmp;
            all_tri ++;
            // InsertTriNode(this->deepbufferoctree_, Vec3d(0, 0, 0), Vec3d(1.0, 1.0, 1.0), curnode, 1);
            InsertTriNode(this->deepbufferoctree_, minn, maxx, curnode, 1);
        }
    }
    std::cout << "all insert node is " << insertnode << '\n';
    std::cout << "all tri mesh is " << all_tri << '\n';
}

bool OctHZBuffer::JudgeTriNodeInBox(triNode& trinode, Vec3d& minn, Vec3d& maxx)
{

    int f = 1;
    for(int i = 0; i < 3; i++)
    {

        if( minn.x() <= trinode.point[i].x() && 
            minn.y() <= trinode.point[i].y() && 
            minn.z() <= trinode.point[i].z() && 
            
            maxx.x() >= trinode.point[i].x() && 
            maxx.y() >= trinode.point[i].y() && 
            maxx.z() >= trinode.point[i].z() )
        {
            continue;
        }
        else{
            f = 0;
            break;
        }
    }
    return f;
}

void OctHZBuffer::InsertTriNode(octree* curtree, Vec3d& minn, Vec3d& maxx, triNode& trinode, int curdeep)
{

    if(curdeep == this->treedeep_)
    {
        insertnode++;
        curtree->triVec.push_back(trinode);
        return ; 
    }

    Vec3d f0, f1, f2, f3; // front_id_0 -> d0
    Vec3d c0, c1, c2, c3; // center_id_0 -> c0
    Vec3d c4, c5, c6, c7;
    Vec3d b4, b5, b6, b7;

    double center_x = (minn.x() + maxx.x()) / 2.0;
    double center_y = (minn.y() + maxx.y()) / 2.0;
    double center_z = (minn.z() + maxx.z()) / 2.0;

//****************************************************
// 将一个Box分成8个子Box，分别是前面4个，后面4个
// 前面的4个编号从左到右，从上到下为：0 1 2 3
// 后面的4个编号从左到右，从上到下为：4 5 6 7
// 分别对应最小和最大的子box为：
// (c0, f0), (c1, f1), (c2, f2), (c3, f3) 
// (b4, c4), (b1, c1), (b2, c2),l(b3, c3) 
//****************************************************

// Box 00 ********************************************

    c0.x() = minn.x();
    c0.y() = center_y;
    c0.z() = center_z;

    f0.x() = center_x;
    f0.y() = maxx.y(); 
    f0.z() = maxx.z();

    if(JudgeTriNodeInBox(trinode, c0, f0))
    {
        InsertTriNode(curtree->t[0], c0, f0, trinode, curdeep + 1);
        return ;
    }

// Box 01 ********************************************

    c1.x() = center_x;
    c1.y() = center_y;
    c1.z() = center_z;

    f1.x() = maxx.x();
    f1.y() = maxx.y(); 
    f1.z() = maxx.z();

    if(JudgeTriNodeInBox(trinode, c1, f1))
    {
        InsertTriNode(curtree->t[1], c1, f1, trinode, curdeep + 1);
        return ;
    }

// Box 02 ********************************************

    c2.x() = minn.x();
    c2.y() = minn.y();
    c2.z() = center_z;

    f2.x() = center_x;
    f2.y() = center_y; 
    f2.z() = maxx.z();

    if(JudgeTriNodeInBox(trinode, c2, f2))
    {
        InsertTriNode(curtree->t[2], c2, f2, trinode, curdeep + 1);
        return ;
    }

// Box 03 ********************************************

    c3.x() = center_x;
    c3.y() = minn.y();
    c3.z() = center_z;

    f3.x() = maxx.x();
    f3.y() = center_y; 
    f3.z() = maxx.z();

    if(JudgeTriNodeInBox(trinode, c3, f3))
    {
        InsertTriNode(curtree->t[3], c3, f3, trinode, curdeep + 1);
        return ;
    }

// Box 04 ********************************************

    b4.x() = minn.x();
    b4.y() = center_y;
    b4.z() = minn.z();

    c4.x() = center_x;
    c4.y() = maxx.y(); 
    c4.z() = center_z;

    if(JudgeTriNodeInBox(trinode, b4, c4))
    {
        InsertTriNode(curtree->t[4], b4, c4, trinode, curdeep + 1);
        return ;
    }

// Box 05 ********************************************

    b5.x() = center_x;
    b5.y() = center_y;
    b5.z() = minn.z();

    c5.x() = maxx.x();
    c5.y() = maxx.y(); 
    c5.z() = center_z;

    if(JudgeTriNodeInBox(trinode, b5, c5))
    {
        InsertTriNode(curtree->t[5], b5, c5, trinode, curdeep + 1);
        return ;
    }

// Box 06 ********************************************

    b6.x() = minn.x();
    b6.y() = minn.y();
    b6.z() = minn.z();

    c6.x() = center_x;
    c6.y() = center_y; 
    c6.z() = center_z;

    if(JudgeTriNodeInBox(trinode, b6, c6))
    {
        InsertTriNode(curtree->t[6], b6, c6, trinode, curdeep + 1);
        return ;
    }

// Box 07 ********************************************

    b7.x() = center_x;
    b7.y() = minn.y();
    b7.z() = minn.z();

    c7.x() = maxx.x();
    c7.y() = center_y; 
    c7.z() = center_z; 

    if(JudgeTriNodeInBox(trinode, b7, c7))
    {
        InsertTriNode(curtree->t[7], b7, c7, trinode, curdeep + 1);
        return ;
    }

// intersection ********************************************
// 如果不完全属于任何一个子Box，则放到当前Box里面

    insertnode++;
    curtree->triVec.push_back(trinode);
    return ;
}

void OctHZBuffer::OctreeRender(octree* curtree, Vec3d& minn, Vec3d& maxx, int curdeep, int id)
{
    if(curdeep > this->treedeep_)
    {
        return ;
    }
    // std::cout << "I am Here Render This Box " << minn.x() << " " << minn.y() << " " << minn.z() << " " << maxx.x() << " " << maxx.y() << " " << maxx.z() << '\n';

    Vec3d f0, f1, f2, f3; // front_id_0 -> d0
    Vec3d c0, c1, c2, c3; // center_id_0 -> c0
    Vec3d c4, c5, c6, c7;
    Vec3d b4, b5, b6, b7;

    double center_x = (minn.x() + maxx.x()) / 2.0;
    double center_y = (minn.y() + maxx.y()) / 2.0;
    double center_z = (minn.z() + maxx.z()) / 2.0;

//****************************************************
// 将一个Box分成8个子Box，分别是前面4个，后面4个
// 前面的4个编号从左到右，从上到下为：0 1 2 3
// 后面的4个编号从左到右，从上到下为：4 5 6 7
// 分别对应最小和最大的子box为：
// (c0, f0), (c1, f1), (c2, f2), (c3, f3) 
// (b4, c4), (b1, c1), (b2, c2),l(b3, c3) 
//****************************************************

// Box 00 ********************************************

    c0.x() = minn.x();
    c0.y() = center_y;
    c0.z() = center_z;

    f0.x() = center_x;
    f0.y() = maxx.y(); 
    f0.z() = maxx.z();

// Box 01 ********************************************

    c1.x() = center_x;
    c1.y() = center_y;
    c1.z() = center_z;

    f1.x() = maxx.x();
    f1.y() = maxx.y(); 
    f1.z() = maxx.z();

// Box 02 ********************************************

    c2.x() = minn.x();
    c2.y() = minn.y();
    c2.z() = center_z;

    f2.x() = center_x;
    f2.y() = center_y; 
    f2.z() = maxx.z();

// Box 03 ********************************************

    c3.x() = center_x;
    c3.y() = minn.y();
    c3.z() = center_z;

    f3.x() = maxx.x();
    f3.y() = center_y; 
    f3.z() = maxx.z();

// Box 04 ********************************************

    b4.x() = minn.x();
    b4.y() = center_y;
    b4.z() = minn.z();

    c4.x() = center_x;
    c4.y() = maxx.y(); 
    c4.z() = center_z;

// Box 05 ********************************************

    b5.x() = center_x;
    b5.y() = center_y;
    b5.z() = minn.z();

    c5.x() = maxx.x();
    c5.y() = maxx.y(); 
    c5.z() = center_z;

// Box 06 ********************************************

    b6.x() = minn.x();
    b6.y() = minn.y();
    b6.z() = minn.z();

    c6.x() = center_x;
    c6.y() = center_y; 
    c6.z() = center_z;

// Box 07 ********************************************

    b7.x() = center_x;
    b7.y() = minn.y();
    b7.z() = minn.z();

    c7.x() = maxx.x();
    c7.y() = center_y; 
    c7.z() = center_z;

// intersection ********************************************
// 如果不完全属于任何一个子Box，则放到当前Box里面

    OctreeRender(curtree->t[0], c0, f0, curdeep + 1, 1);
    OctreeRender(curtree->t[1], c1, f1, curdeep + 1, 2);
    OctreeRender(curtree->t[2], c2, f2, curdeep + 1, 3);
    OctreeRender(curtree->t[3], c3, f3, curdeep + 1, 4);
    OctreeRender(curtree->t[4], b4, c4, curdeep + 1, 5);
    OctreeRender(curtree->t[5], b5, c5, curdeep + 1, 6);
    OctreeRender(curtree->t[6], b6, c6, curdeep + 1, 7);
    OctreeRender(curtree->t[7], b7, c7, curdeep + 1, 8);

    if(curtree->triVec.size() == 0) return ;

    Vec2i coord[2];
    double deep;

    coord[0].x() = (minn.x() + 1) / 2 * this->width_;
    coord[0].y() = (minn.y() + 1) / 2 * this->height_;

    coord[1].x() = (maxx.x() + 1) / 2 * this->width_;
    coord[1].y() = (maxx.y() + 1) / 2 * this->height_;

    deep = max(minn.z(), maxx.z());

    std::cout << "cur box id is " << id << " cur box tri num is " << curtree->triVec.size() << "  cur box coord is : " << minn.x() << " " << minn.y() << " " << minn.z() << " | " << maxx.x() << " " << maxx.y() << " " << maxx.z() << "\n";


    if(JudgeRender(deepbuffertree_, Vec2i(0, 0), Vec2i(width_ - 1, height_ - 1), coord, deep))
    {
        std::cout << "Render This Box " << coord[0].x() << " " << coord[0].y() << " " << coord[1].x() << " " << coord[1].y() << " deep = " << deep << '\n';
        insertnode += curtree->triVec.size();
        if(curtree->triVec.size() != 0)
        // std::cout << "Cur Render tri number is " << curtree->triVec.size() << '\n';
        for(int i = 0; i < curtree->triVec.size(); i++)
        {
            Vec3d deep3d;
            double deep_max = initdeep;
            Vec2i screenCoord[3];
            for(int k = 0; k < 3; k++)  // 把当前三角形的所有的点转换到二维的屏幕坐标中
            {
                screenCoord[k](0) = width_ *  curtree->triVec[i].point[k].x();
                screenCoord[k](1) = height_ * curtree->triVec[i].point[k].y();
                deep3d[k] = curtree->triVec[i].point[k].z();
                deep_max = max(deep_max, deep3d[k]);
            }
            // std::cout << screenCoord[0].x() << " " << screenCoord[0].y() << " " << screenCoord[1].x() << " " << screenCoord[1].y() << " " << screenCoord[2].x() << " " << screenCoord[2].y() << "\n" ;
            RenderTriangle(screenCoord, deep3d, curtree->triVec[i].color);
        }
        // std::cout << "Render OK\n";
    }
    else{
        if(curtree->triVec.size() > 0)
            std::cout << "This Box is not render. Therefore free " << curtree->triVec.size() << '\n';
    }
    return ;
}
