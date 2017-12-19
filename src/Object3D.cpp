#include "Object3D.h"

Object3D::Object3D() {}
Object3D::~Object3D() {}

Object3D::Object3D(Eigen::Vector3d origin) {
	o = origin;
}

Object3D::Object3D(Eigen::Vector3d origin, Eigen::Vector3d direction) {
	o = origin;
	d = direction;
}

Eigen::Vector3d& Object3D::getOrigin() {
	return o;
}

Eigen::Vector3d Object3D::getDirection() {
	return d;
}

Eigen::Vector3d Object3D::ScreenToWorld(Eigen::Vector3d coords) {
	//Norm - a.k.a NDC space
	Eigen::Vector2d norm((coords[0] + 0.5) / 800, (coords[1] + 0.5) / 800);

	//PixelScreen(x)=2∗PixelNDC(x)−1
	//PixelScreen(y)=1−2∗PixelNDC(y)
	Eigen::Vector2d ScreenSpace(2 * (norm[0] - 1),
	                            1 - 2 * norm[1]); //Reverse the Y so we get a the right orientetion

	Eigen::Vector3d sum(ScreenSpace[0], ScreenSpace[1], 0);

	return sum;
}

//To be implemented
void Object3D::translate(Eigen::Vector3d pos) {}
void Object3D::scale(Eigen::Vector3d scale) {}
void Object3D::rotate(Eigen::Vector3d rotation) {}

void Object3D::setX(double displace) {
	o[0] += displace;
}
void Object3D::setY(double displace) {
	o[1] += displace;
}
void Object3D::setZ(double displace) {
	o[2] += displace;
}