#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Eigen/Core>

#include "Scene.h"
#include "Object3D.h"
#include "Ray.h"

namespace Camera {

//Projection camera
class ProjectionCamera: public Object3D {
public:
	Ray ray;

	ProjectionCamera(Eigen::Vector3d, Eigen::Vector3d);
	ProjectionCamera();
	~ProjectionCamera();

	void castRay(Eigen::Vector3d);
};

//Perspective camera
class PerspectiveCamera: public Object3D {
public:
	Ray ray;

	PerspectiveCamera(Eigen::Vector3d);
	~PerspectiveCamera();

	void castRay(Eigen::Vector3d);
};

}

#endif
