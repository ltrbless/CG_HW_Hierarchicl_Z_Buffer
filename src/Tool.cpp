#include"Tool.h"

Mat3 getlookatmatrix( Vec3d & eye,  Vec3d& center,  Vec3d& up)
{
    Vec3d f = center - eye;
    f.normalize();

    Vec3d s = up.cross(f);
    s.normalize();

    Vec3d u = f.cross(s);

    Mat3 mat;


    mat(0, 0) = s.x();
    mat(1, 0) = s.y();
    mat(2, 0) = s.z();
    mat(0, 1) = u.x();
    mat(1, 1) = u.y();
    mat(2, 1) = u.z();
    mat(0, 2) = f.x();
    mat(1, 2) = f.y();
    mat(2, 2) = f.z();

    return mat;

    // Vec3d z = (eye - center);
    // z.normalize();
    // Vec3d x = (up.cross(z) );
    // x.normalize();
    // Vec3d y = (z.cross(x));
    // y.normalize();
    // Mat3 Minv = Mat3::Identity();
    // Mat3 Tr   = Mat3::Identity();
    // for (int i=0; i<3; i++) {
    //     Minv(0, i) = x(i, 0);
    //     Minv(1, i) = y(i, 0);
    //     Minv(2, i) = z(i, 0);
    //     // Tr(i, 3) = -center(i);
    // }
    // Mat3 ModelView = Minv;
    
    // return ModelView;
}

void lookatafter(std::vector<TriMesh>& alltrimesh, Mat3& view)
{
    for(int i = 0; i < alltrimesh.size(); i++) // 所有的物体
    {
        for(int j = 0; j < alltrimesh[i].vecCoords.size(); j++) // 一个物体上所有的点的坐标
        {
            Vec3d& curCoord = alltrimesh[i].vecCoords[j];
            Vec3d tmp(curCoord(0), curCoord(1), curCoord(2));

            curCoord = view * tmp; // 旋转

            // for(int k = 0; k < 3; k++)
            // {
            //     curCoord(k) = tmp(k);
            // }
        }
    }

    // for(int trisId = 0; trisId < alltrimesh.size(); trisId++) // 遍历每一个物体
    // {
    //     for(int i = 0; i < alltrimesh[trisId].vecTopos.size(); i++) // 遍历一个物体上所有的三角形
    //     {
    //         Vec3i curTopo = alltrimesh[trisId].vecTopos[i];
    //         for(int j = 0; j < 3; j++)  // 对于一个三角形遍历所有的点
    //         {
    //             Vec4d tmpPoint(alltrimesh[trisId].vecCoords[curTopo(j)](0), alltrimesh[trisId].vecCoords[curTopo(j)](1), alltrimesh[trisId].vecCoords[curTopo(j)](2), 1.0);
    //             tmpPoint = view * tmpPoint;
    //         }
    //     }

    // }

   return ;
}