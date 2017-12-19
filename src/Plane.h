#ifndef _PLANE_H_
#define _PLANE_H_

#include <Eigen/Core>
#include <Eigen/Dense>

#include "Object3D.h"
#include "Material.h"
#include "Ray.h"

class Plane: public Object3D {
public:
	Material material;
	Eigen::Vector3d normal;
	Plane(Eigen::Vector3d _norm, Eigen::Vector3d orig);
	~Plane();
	bool hasIntersected(Ray ray, double &t);
	void setMaterial(Material &mat);
};

#endif