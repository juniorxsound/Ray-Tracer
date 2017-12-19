#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <Eigen/Core>

//Classes
#include "Object3D.h"
#include "Material.h"
#include "Light.h"
#include "Ray.h"

class Sphere: public Object3D {
public:
	//Instance of a material
	Material material;

	Sphere(Eigen::Vector3d, double);
	~Sphere();


	//Main intersection methid
	bool hasIntersected(Ray ray, double &t);

	//Returns the sphere radius
	double getRadius();

	// Return a simple sphere normal calculation
	Eigen::Vector3d getNormal(Eigen::Vector3d &point);

	//Save a reference to a material instance
	void setMaterial(Material &mat);


private:
	//Radius
	double r;
};

#endif