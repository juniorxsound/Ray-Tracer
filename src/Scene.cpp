#include "Scene.h"

Scene::Scene() {
	numSpheres = 0;
	numLights = 0;
	numModels = 0;
}
Scene::~Scene() {}

/**************
* Class methods
***************/

//Add a light to the scene graph
void Scene::add(Light light) {
	lights.push_back(light);
	numLights++;
}

//Add a sphere to the scene graph
void Scene::add(Sphere sphere) {
	spheres.push_back(sphere);
	numSpheres++;
}

void Scene::add(Model model) {
	models.push_back(model);
	numModels++;
}

//Getters
std::vector<Light> Scene::getLights() {
	return lights;
}
Light& Scene::getLight(int index) {
	return lights[index];
}
std::vector<Sphere> Scene::getSpheres() {
	return spheres;
}
Sphere& Scene::getSphere(int index) {
	return spheres[index];
}
std::vector<Model> Scene::getModels(){
	return models;
}
Model& Scene::getModel(int index) {
	return models[index];
}
void Scene::clearLights() {
	lights.clear();
	numLights = 0;
}

void Scene::sortDepth() {}