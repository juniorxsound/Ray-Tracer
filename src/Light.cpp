#include "Light.h"

Light::Light(Eigen::Vector3d pos, double intense) {
	o = pos;
	intensity = intense;
}
Light::~Light() {}

Eigen::Vector3d Light::getOrigin() {
	return o;
}

void Light::setX(double displace) {
	o[0] += displace;

}
void Light::setY(double displace) {
	o[1] += displace;

}
void Light::setZ(double displace) {
	o[2] += displace;
}

void Light::setNewPos(Eigen::Vector3d newPos) {
	o = newPos;
}

