#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <Eigen/Core>

//This is used for each material type to store the indvidual paramaters under a type's name
struct MaterialType {
	double intensity;
	Eigen::Vector3d ce;
};

//The main material class
class Material {
public:
	//Instance the three types of shading techniques
	MaterialType diffuse;
	MaterialType specular;
	MaterialType ambient;

	//Keep phong exponent at the root of the Material instance
	int phong_exponent;

	Material(double,
	         Eigen::Vector3d,
	         double,
	         Eigen::Vector3d,
	         double,
	         Eigen::Vector3d,
	         int);

	Material();
	~Material();

	/**************
	* Class methods
	**************/

	//Calculate shading - Diffused model
	Eigen::Vector3d Diffuse(Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d);

	//Calculate shading - Ambient model
	Eigen::Vector3d Ambient();

	//Calculate shading - Specular model
	Eigen::Vector3d Specular(Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d);

};
#endif