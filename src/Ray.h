#ifndef _RAY_H_
#define _RAY_H_

#include <Eigen/Core>

class Ray {
public:
	//Constructor
	Ray();
	Ray(Eigen::Vector3d, Eigen::Vector3d);
	~Ray();

	//Slim methods
	Eigen::Vector3d getOrigin();
	Eigen::Vector3d getDirection();

	void createCameraRay(Eigen::Vector3d, Eigen::Vector3d);
	void createShadowRay(Eigen::Vector3d, Eigen::Vector3d);
	void createReflectionRay(Eigen::Vector3d, Eigen::Vector3d);
private:
	Eigen::Vector3d o;
	Eigen::Vector3d d;
};

#endif