#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense> // Needed for cross products

#include "Object3D.h"
#include "Ray.h"
#include "Material.h"

#define EPSILLON 0.0000001

struct Vertex {
	Eigen::Vector3d o;
};

struct Face {
	Eigen::Vector3d v0;
	Eigen::Vector3d v1;
	Eigen::Vector3d v2;

	Eigen::Vector3d normal;

	bool hasIntersected(Ray ray, double &t);
	Eigen::Vector3d getNormal();
};

class Model: public Object3D {
public:
	Material material;
	Model(Eigen::Vector3d);
	~Model();

	void loadOFF(std::string);
	void setMaterial(Material &mat);

	std::string DATA_PATH;

	//Arrays created once model has been loaded
	Vertex *vertices;
	Face *faces;

	//Utility stuff
	int numVertecies;
	int numFaces;

private:

};

#endif