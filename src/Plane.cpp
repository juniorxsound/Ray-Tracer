#include "Plane.h"

Plane::Plane(Eigen::Vector3d _norm, Eigen::Vector3d orig) {
    normal = _norm;
    o = orig;
}

Plane::~Plane() {}

bool Plane::hasIntersected(Ray ray, double &t)
{
    Eigen::Vector3d flippedNorm;
    // assuming vectors are all normalized
    if (o[1] < 0) flippedNorm = Eigen::Vector3d(normal[0], normal[1] * -1, normal[2]);
    else flippedNorm = Eigen::Vector3d(normal[0], normal[1], normal[2]);

    double denom = flippedNorm.dot(ray.getDirection());
    if (denom > 1e-6) {
        Eigen::Vector3d p0l0 = (o - ray.getOrigin()).normalized();
        t = p0l0.dot(flippedNorm) / denom;
        return (t >= 0);
    }

    return false;
}

void Plane::setMaterial(Material &mat) {
    material = mat;
}