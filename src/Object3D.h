#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <tuple>

//Base class for everything that lives in the 3D space
class Object3D {
public:
    Object3D();
    ~Object3D();
    //Origin only construction
    Object3D(Eigen::Vector3d);
    //Origin and direction
    Object3D(Eigen::Vector3d, Eigen::Vector3d);


    /**************
    * Class methods
    **************/
    Eigen::Vector3d& getOrigin();
    Eigen::Vector3d getDirection();

    void translate(Eigen::Vector3d);
    void scale(Eigen::Vector3d);
    void rotate(Eigen::Vector3d rotation);

    void setX(double displace);
    void setY(double displace);
    void setZ(double displace);

    static Eigen::Vector3d ScreenToWorld(Eigen::Vector3d);

protected:
    Eigen::Vector3d o;
    Eigen::Vector3d d;

};

#endif