#include "Material.h"

Material::Material(double ambientIn,
                   Eigen::Vector3d ambientCE,
                   double diffuseIn,
                   Eigen::Vector3d diffuseCE,
                   double specularIn,
                   Eigen::Vector3d specularCE,
                   int phongEx) {

  //Assign the intensities
  diffuse.intensity = diffuseIn;
  ambient.intensity = ambientIn;
  specular.intensity = specularIn;

  //Assign the coefficiences
  diffuse.ce = diffuseCE;
  ambient.ce = ambientCE;
  specular.ce = specularCE;

  //Remember the phong exponent
  phong_exponent = phongEx;
}

Material::Material() {}
Material::~Material() {}

//Calculate shading - Diffused model
Eigen::Vector3d Material::Diffuse(Eigen::Vector3d lightPos, Eigen::Vector3d normal, Eigen::Vector3d intersectionPt) {
  Eigen::Vector3d l = (lightPos - intersectionPt).normalized();
  double calculateDiff = std::max(l.dot(normal), 0.);


  Eigen::Vector3d dif((calculateDiff * diffuse.ce[0]) * diffuse.intensity,
                      (calculateDiff * diffuse.ce[1]) * diffuse.intensity,
                      (calculateDiff * diffuse.ce[2]) * diffuse.intensity);
  return dif;
}

//Calculate shading - Ambient model
Eigen::Vector3d Material::Ambient() {
  Eigen::Vector3d amb(std::max(ambient.intensity * ambient.ce[0], 0.),
                      std::max(ambient.intensity * ambient.ce[1], 0.),
                      std::max(ambient.intensity * ambient.ce[2], 0.));
  return amb;
}

//Calculate shading - Specular model
Eigen::Vector3d Material::Specular(Eigen::Vector3d lightPos, Eigen::Vector3d rayOrigin, Eigen::Vector3d normal, Eigen::Vector3d intersectionPt) {
  //r = 2(v.n)n-v
  //specular = coefficiencies[](v.l)^phongexponent
  Eigen::Vector3d V = (rayOrigin - intersectionPt).normalized();
  Eigen::Vector3d l = (lightPos - intersectionPt).normalized();
  Eigen::Vector3d reflectionVector = 2 * (l.dot(normal)) * normal - l;
  double reflect = std::max(std::pow(V.dot(reflectionVector), phong_exponent), 0.);

  Eigen::Vector3d spec(reflect * specular.intensity * specular.ce[0],
                       reflect * specular.intensity * specular.ce[1],
                       reflect * specular.intensity * specular.ce[2]);

  return spec;
}