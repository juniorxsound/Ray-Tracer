#include "Camera.h"

//Projection Camera
Camera::ProjectionCamera::ProjectionCamera(Eigen::Vector3d origin, Eigen::Vector3d direction) {
	o = origin;
	d = direction;
}
Camera::ProjectionCamera::ProjectionCamera() {}
Camera::ProjectionCamera::~ProjectionCamera() {}

void Camera::ProjectionCamera::castRay(Eigen::Vector3d coords) {
	Eigen::Vector3d pos = Camera::ProjectionCamera::ScreenToWorld(coords);

	//Prepare the camera ray - o is used to displace the camera
	ray.createCameraRay(Eigen::Vector3d(pos[0] + o[0], pos[1] + o[1], pos[2] + o[2]), d);
}

//Perspective Camera
Camera::PerspectiveCamera::PerspectiveCamera(Eigen::Vector3d pos) {
	o = pos;
}
Camera::PerspectiveCamera::~PerspectiveCamera() {}

void Camera::PerspectiveCamera::castRay(Eigen::Vector3d coords) {
	Eigen::Vector3d WorldPos = Camera::PerspectiveCamera::ScreenToWorld(coords);	//Calculate new directions for the rays
	Eigen::Vector3d perspectiveRays(WorldPos[0] - o[0], WorldPos[1] - o[1], WorldPos[2] - o[2]);
	ray.createCameraRay(o, perspectiveRays.normalized());
}
