#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <Eigen/Core>
#include <iostream>

class Light {
public:
	double intensity;

	//Constructor
	Light(Eigen::Vector3d, double);
	~Light();
	/**************
	* Class methods
	***************/
	Eigen::Vector3d getOrigin();

	void setX(double displace);
	void setY(double displace);
	void setZ(double displace);

	void setNewPos(Eigen::Vector3d newPos);

private:
	Eigen::Vector3d o;
};
#endif