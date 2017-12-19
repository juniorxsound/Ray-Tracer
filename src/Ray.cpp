#include "Ray.h"

Ray::Ray() {}
Ray::Ray(Eigen::Vector3d orig, Eigen::Vector3d dir) {
	o = orig;
	d = dir;
}
Ray::~Ray() {}

//Slim methods
Eigen::Vector3d Ray::getOrigin() { return o; }
Eigen::Vector3d Ray::getDirection() { return d; }

void Ray::createCameraRay(Eigen::Vector3d orig, Eigen::Vector3d dir) {
	o = orig;
	d = dir;
}

void Ray::createShadowRay(Eigen::Vector3d orig, Eigen::Vector3d dir) {
	o = orig;
	d = dir;
}

void Ray::createReflectionRay(Eigen::Vector3d orig, Eigen::Vector3d dir) {
	o = orig;
	d = dir;
}