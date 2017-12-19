#include "Model.h"

Model::Model(Eigen::Vector3d _pos) {
	//Relative model path
	DATA_PATH = "../data/";
	o = _pos;
}

Model::~Model() {}

void Model::loadOFF(std::string fileName) {
	std::ifstream fileIn((DATA_PATH + fileName).c_str());
	//Log state
	if (!fileIn) std::cout << "[Setup] Unable to read " << fileName << std::endl;
	else std::cout << "[Setup] " << fileName << " opened successfully" << std::endl;

	//Read the first line and make sure it's an .off file
	//Should start with "OFF" - http://www.geomview.org/docs/html/OFF.html
	std::string tempLine;
	std::getline(fileIn, tempLine);

	//Second line defines how many vertecies and how many faces the shape has
	std::getline(fileIn, tempLine);

	//Set a delimiter which we will use to split the string lines back to ints
	int firstDeliPos = tempLine.find(" ", 0);
	numVertecies = std::atoi(tempLine.substr(0, firstDeliPos + 1).c_str());
	std::cout << "[Setup] The model has " << numVertecies << " verticies" << std::endl;

	int secondDeliPos = tempLine.find(" ", firstDeliPos + 1);
	numFaces = std::atoi(tempLine.substr(firstDeliPos, secondDeliPos + 1).c_str());
	std::cout << "[Setup] The model has " << numFaces << " faces" << std::endl;

	//Will be used for vertices and faces
	int thirdDeliPos;
	int fourthDeliPos;

	//Create the vertices array (on the heap!)
	vertices = new Vertex[numVertecies];

	//Iterate and store all vertices in the model
	for (int i = 0; i < numVertecies; i++) {
		//Read line
		std::getline(fileIn, tempLine);
		//Split and save X
		firstDeliPos = tempLine.find(" ", 0);
		vertices[i].o[0] = std::atof(tempLine.substr(0, firstDeliPos).c_str());
		//Split and save Y
		secondDeliPos = tempLine.find(" ", firstDeliPos + 1);
		vertices[i].o[1] = std::atof(tempLine.substr(firstDeliPos, secondDeliPos ).c_str());
		//Split and save Z
		thirdDeliPos = tempLine.find(" ", secondDeliPos + 1);
		vertices[i].o[2] = std::atof(tempLine.substr(secondDeliPos, thirdDeliPos ).c_str());
	}

	//Crate the faces array (on the heap!)
	faces = new Face[numFaces];

	//Iterate over all faces and sort the relevant vertecies into them
	for (int i = 0; i < numFaces; i++) {
		//Read line
		std::getline(fileIn, tempLine);
		//Split the number of vertices - currently only 3 are allowed (i.e triangular meshes)
		firstDeliPos = tempLine.find(" ", 0);
		const int verteciesPerFace = std::atoi(tempLine.substr(0, firstDeliPos).c_str());
		//If the model uses more than 3 verts per face break the function
		if (verteciesPerFace != 3) {
			std::cout << "[Warning] The model uses more than 3 vertices per face, model will not be loaded" << std::endl;
			return;
		}
		//Get the number of the first vertex in the face and save it
		secondDeliPos = tempLine.find(" ", firstDeliPos + 1);
		int v0num = std::atoi(tempLine.substr(firstDeliPos, secondDeliPos).c_str());
		faces[i].v0 = Eigen::Vector3d((vertices[v0num].o[0] + o[0]),
		                              (vertices[v0num].o[1] + o[1]),
		                              (vertices[v0num].o[2] + o[2]));

		//Get the number of the second vertex in the face and save it
		thirdDeliPos = tempLine.find(" ", secondDeliPos + 1);
		int v1num = std::atoi(tempLine.substr(secondDeliPos, thirdDeliPos).c_str());
		faces[i].v1 = Eigen::Vector3d((vertices[v1num].o[0] + o[0]),
		                              (vertices[v1num].o[1] + o[1]),
		                              (vertices[v1num].o[2] + o[2]));

		//Get the number of the third vertex in the face and save it
		fourthDeliPos = tempLine.find(" ", thirdDeliPos + 1);
		int v2num = std::atoi(tempLine.substr(thirdDeliPos, fourthDeliPos).c_str());
		faces[i].v2 = Eigen::Vector3d((vertices[v2num].o[0] + o[0]),
		                              (vertices[v2num].o[1] + o[1]),
		                              (vertices[v2num].o[2] + o[2]));

		// std::cout << "FACE NUMBER" << i << std::endl;
		// std::cout << "----------------" << std::endl;
		// std::cout <<"v0 is: " << v0num << " v1 is: " << v1num << " v2 is: " << v2num << std::endl;
		// std::cout << "v0 vector3 is: \n" << faces[i].v0 << std::endl;
		// std::cout << "v1 vector3 is: \n" << faces[i].v1 << std::endl;
		// std::cout << "v2 vector3 is: \n" << faces[i].v2 << std::endl;
		// std::cout << std::endl;

	}

	std::cout << "[Setup] Finished loading model" << std::endl;

}

void Model::setMaterial(Material &mat) {
	material = mat;
}

bool Face::hasIntersected(Ray ray, double &t) {

	double a, f, u, v;
	Eigen::Vector3d v0v1 = v1 - v0;
	Eigen::Vector3d v0v2 = v2 - v0;

	Eigen::Vector3d rawNormal = ray.getDirection().cross(v0v2);
	normal = (v0v1.cross(v0v2)).normalized();
	a = v0v1.dot(rawNormal);

	if (a < EPSILLON) return false;

	f = 1 / a;
	Eigen::Vector3d s = ray.getOrigin() - v0;
	u = f * (s.dot(rawNormal));
	if (u < 0.0 || u > 1.0) return false;

	Eigen::Vector3d q = s.cross(v0v1);
	v = f * ray.getDirection().dot(q);

	if (v < 0.0 || u + v > 1.0) return false;

	t = f * v0v2.dot(q);
	if (t > EPSILLON) return true;
	else return false;

}

Eigen::Vector3d Face::getNormal() {
	return normal;
}