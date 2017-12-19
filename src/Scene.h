#ifndef _SCENE_H_
#define _SCENE_H_

#include <Eigen/Core>

#include <iostream>
#include <vector>

#include "Object3D.h"
#include "Light.h"
#include "Sphere.h"
#include "Model.h"

class Scene {
public:
	int numSpheres;
	int numLights;
	int numModels;

	Scene();
	~Scene();

	/**************
	* Class methods
	***************/

	//Add a light to the scene graph
	void add(Light);

	//Add a sphere to the scene graph
	void add(Sphere);

	//Add a model to the scene graph
	void add(Model);

	void clearLights();

	//Getters
	std::vector<Light> getLights();

	Light& getLight(int);

	std::vector<Sphere> getSpheres();

	Sphere& getSphere(int);

	std::vector<Model> getModels();

	Model& getModel(int);

	static void sortDepth();

private:
	std::vector<Sphere> spheres;
	std::vector<Light> lights;
	std::vector<Model> models;
};

#endif