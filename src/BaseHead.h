
#ifndef _BASE_HEAD_ 
#define _BASE_HEAD_ 

// #include<cmath>

#include<math.h>
#include<cmath>
#include<vector>
#include<list>
#include "Eigen/Dense"

typedef Eigen::Matrix<unsigned char, 4, 1> Vector4unchar;
typedef Eigen::Matrix<double, 3, 3> Mat3;
typedef Eigen::Vector3d Vec3d;
typedef Eigen::Vector4d Vec4d;
typedef Eigen::Vector3i Vec3i;
typedef Eigen::Vector2i Vec2i;

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
#define min4(a, b, c, d) min(min(a, b), min(c, d))

extern double initdeep;

struct EdgeTable
{
    double x; // 起点
    double z;

    double dx;  // x方向的位移
    double dz;

    int len_y; // 经过扫描线的次数   

};

struct TriTable
{
    EdgeTable e_01, e_02, e_12;
    Vector4unchar color;
};

struct ActiveEdgeTable
{
    double lx, rx;  // 扫描线的左右起点
    double dlx, drx;  // X方向上的位移
    double lz, rz;  // 扫描线左右深度起点
    double dlz, drz;  // z的变化
    int dy;
    int len;
    bool used;
    TriTable tri;  // 找到该边属于哪个面表
};

struct ftree
{
    ftree* t1;
    ftree* t2;
    ftree* t3;
    ftree* t4;
    double deep;

    ftree()
    {
        t1 = nullptr;
        t2 = nullptr;
        t3 = nullptr;
        t4 = nullptr;
        deep = initdeep;        
    }
};




#endif