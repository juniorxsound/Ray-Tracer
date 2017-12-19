// C++ include
#include <iostream>
#include <string>
#include <vector>

// Eigen Linear Algerbra (dense is used for cross products)
#include <Eigen/Core>
#include <Eigen/Dense>

// Image writing library
#define STB_IMAGE_WRITE_IMPLEMENTATION // Do not include this line twice in your project!
#include "stb_image_write.h"
#include "utils.h"

#define EPSILLON 0.0000001

using namespace Eigen;
using namespace std;

//Ray tracer classes
#include "Scene.h"
#include "Ray.h"
#include "Light.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"
#include "Model.h"
#include "Plane.h"

const int width = 800;
const int height = 800;

/********************
* Ray Tracer function
********************/
int frameCounter = 0;
int numAnimationFrames = 1;

void RayTracer() {

    //R, G, B and A pixel buffers
    MatrixXd colorR = MatrixXd::Zero(width, height);
    MatrixXd colorG = MatrixXd::Zero(width, height);
    MatrixXd colorB = MatrixXd::Zero(width, height);
    MatrixXd colorA = MatrixXd::Zero(width, height);

    //A diffused blue'ish material
    Material specularMat(0.3, //Ambient intensity
                         Vector3d(1, 1, 1), // Ambient coefficiencies
                         0.7, // Diffuse intensity
                         Vector3d(1.5, 1, 1), // Diffuse coefficiencies
                         10.0,//Specular intensity
                         Vector3d(1, 1, 1), //Specular coefficiencies
                         10000); // Phong exponent

    //Material instance
    Material diffusedMat(0.2, //Ambient intensity
                         Vector3d(1, 1, 1.5), // Ambient coefficiencies
                         0.4, // Diffuse intensity
                         Vector3d(1, 1, 1.5), // Diffuse coefficiencies
                         0,//Specular intensity
                         Vector3d(1, 1, 1), //Specular coefficiencies
                         1000); // Phong exponent


    //Material instance
    Material floorMat(0.01, //Ambient intensity
                      Vector3d(1, 1, 1.5), // Ambient coefficiencies
                      1.5, // Diffuse intensity
                      Vector3d(1, 1, 1.5), // Diffuse coefficiencies
                      1,//Specular intensity
                      Vector3d(1, 1, 1), //Specular coefficiencies
                      100000); // Phong exponentew

    //Instance a scene (i.e graph manager)
    Scene scene;

    Camera::PerspectiveCamera camera(Vector3d(0, 0, -2));

    Vector3d bunnyPos(-0.3, -0.1, -1);
    Vector3d simplePos(-1, 0, 2);

    //First argument is a position vector
    Model model2(Vector3d(-4, 10, 50));
    model2.loadOFF("bumpy_cube.off");
    model2.setMaterial(specularMat);
    // scene.add(model2);

    //First argument is a position vector
    Model model(Vector3d(-6, 1, 6));
    model.loadOFF("bunny_scaled.off");
    model.setMaterial(specularMat);
    // scene.add(model);


    //Instance a sphere
    Sphere sphere(Vector3d(-1, .5, 3), 0.5);
    sphere.setMaterial(specularMat);
    scene.add(sphere);

    //Instance a second sphere
    Sphere sphere2(Vector3d(-2.5, .5, 6), 0.5);
    sphere2.setMaterial(diffusedMat);
    scene.add(sphere2);

    // Instance a light
    Light light(Vector3d(-3, .5, -1.8), 1);
    scene.add(light);

    Light light2(Vector3d(-2, .5, 10), 1);
    scene.add(light2);

    Plane plane(Vector3d(0, 1, 0), Vector3d(0, -0.2, 0));
    plane.setMaterial(floorMat);

    int counter = 0;
    double cameraPlaneMove = 0.0;

    //Used for rendering sequences
    for (int frame = 0; frame < numAnimationFrames; frame++) {

        //Clear the "canvas"
        //R, G, B and A pixel buffers
        colorR = MatrixXd::Zero(width, height);
        colorG = MatrixXd::Zero(width, height);
        colorB = MatrixXd::Zero(width, height);
        colorA = MatrixXd::Zero(width, height);

        /***********************
        * Simple sine animations
        ***********************/
        cameraPlaneMove += 0.01;

        //Light animations
        for (int i = 0; i < scene.numLights; i ++) {
            scene.getLight(i).setX(sin(double(frame * 0.05)));
            scene.getLight(i).setY(cos(double(frame * 0.05)));
            scene.getLight(i).setZ(sin(double(frame * 0.05)));
        }

        camera.setZ(0.01);

        for (unsigned int x = 0; x < width; ++x) {

            for (unsigned int y = 0; y < height; ++y) {

                //Keep count of the current round
                counter++;
                int renderStatus = round((float(counter) / (float(width) * float(height)) * 100) / numAnimationFrames);
                if (counter % 100000 == 0) cout << "[Renderer] Rendered " << renderStatus << " percent" << endl;

                //Prepare and shoot the ray
                camera.castRay(Vector3d(x, y, cameraPlaneMove));

                double t = INFINITY;

                /*****************************************
                * Plane - reflection and diffuse/specular
                *****************************************/
                if (plane.hasIntersected(camera.ray, t)) {

                    Vector3d pt = camera.ray.getOrigin() + t * camera.ray.getDirection();

                    //Calculate the ambient outside the light loop
                    Vector3d ambient = plane.material.Ambient();

                    for (int light_i = 0; light_i < scene.numLights; light_i++) {
                        //r = d - 2(d · n)n
                        Vector3d r = camera.ray.getDirection() - 2 * (camera.ray.getDirection().dot(plane.normal)) * plane.normal;
                        Ray reflectionRay;
                        reflectionRay.createReflectionRay(pt, r);
                        double reflectionT = 100;

                        //A vector to collect diffuse contribution over the lights array (i.e additive lighting)
                        Vector3d diffuse_contribution(0, 0, 0);

                        //A vector to collect specular contibution over the lights array (i.e additive lighting)
                        Vector3d specular_contribution(0, 0, 0);

                        //Calculate the diffuse
                        Vector3d diffuseMat = plane.material.Diffuse(scene.getLight(light_i).getOrigin(),
                                              plane.normal,
                                              pt);

                        //Specular calculation - (Vector3d lightPos, Vector3d rayOrigin, Vector3d normal, Vector3d intersectionPt) {
                        Vector3d specularMat = plane.material.Specular(scene.getLight(light_i).getOrigin(),
                                               camera.ray.getOrigin(),
                                               plane.normal,
                                               pt);

                        specular_contribution += specularMat;

                        diffuse_contribution += diffuseMat;

                        //Calculate the reflection off of spheres in the scene
                        for (int reflectedSphere = 0; reflectedSphere < scene.numSpheres; reflectedSphere++) {
                            if (scene.getSphere(reflectedSphere).hasIntersected(reflectionRay, reflectionT)) {

                                Vector3d spherePt = reflectionRay.getOrigin() + reflectionT * reflectionRay.getDirection();

                                //Calculate the diffuse
                                Vector3d diffuse = scene.getSphere(reflectedSphere).material.Diffuse(scene.getLight(light_i).getOrigin(),
                                                   scene.getSphere(reflectedSphere).getNormal(spherePt),
                                                   spherePt);

                                //Specular calculation - (Vector3d lightPos, Vector3d rayOrigin, Vector3d normal, Vector3d intersectionPt) {
                                Vector3d specular = scene.getSphere(reflectedSphere).material.Specular(scene.getLight(light_i).getOrigin(),
                                                    reflectionRay.getOrigin(),
                                                    scene.getSphere(reflectedSphere).getNormal(spherePt),
                                                    spherePt);


                                //Add the light contribution of the current light to the current object
                                diffuse_contribution += diffuse;
                                specular_contribution += specular;

                            }
                        }

                        //Calculate the reflection off of the model's face
                        for (int reflectedModels = 0; reflectedModels < scene.numModels; reflectedModels++) {
                            for (int reflectedFace = 0; reflectedFace < scene.getModel(reflectedModels).numFaces; reflectedFace++) {
                                if (scene.getModel(reflectedModels).faces[reflectedFace].hasIntersected(reflectionRay, reflectionT)) {

                                    Vector3d facePt = reflectionRay.getOrigin() + reflectionT * reflectionRay.getDirection();

                                    //Compute diffuse model
                                    Vector3d diffuse = scene.getModel(reflectedModels).material.Diffuse(scene.getLight(light_i).getOrigin(),
                                                       scene.getModel(reflectedModels).faces[reflectedFace].getNormal(),
                                                       facePt);

                                    Vector3d specular = scene.getModel(reflectedModels).material.Specular(scene.getLight(light_i).getOrigin(),
                                                        reflectionRay.getOrigin(),
                                                        scene.getModel(reflectedModels).faces[reflectedFace].getNormal(),
                                                        facePt);

                                    diffuse_contribution += diffuse;
                                    specular_contribution += specular;

                                }
                            }
                        }

                        colorR(x, y) = max(diffuse_contribution[0] + specular_contribution[0] + ambient[0], 0.);
                        colorG(x, y) = max(diffuse_contribution[1] + specular_contribution[1] + ambient[1], 0.);
                        colorB(x, y) = max(diffuse_contribution[2] + specular_contribution[2] + ambient[2], 0.);
                        colorA(x, y) = 1.0;
                    }
                }

                /***********************************
                *Sphere - shadows, diffuse, specular
                ***********************************/

                for (int object_i = 0; object_i < scene.numSpheres; object_i++) {

                    if (scene.getSphere(object_i).hasIntersected(camera.ray, t)) {

                        //Used for shadow rays
                        bool isShaded = true;

                        //A vector to collect diffuse contribution over the lights array (i.e additive lighting)
                        Vector3d diffuse_contribution(0, 0, 0);

                        //A vector to collect specular contibution over the lights array (i.e additive lighting)
                        Vector3d specular_contribution(0, 0, 0);

                        //Ambient shade calculation
                        Vector3d ambient = scene.getSphere(object_i).material.Ambient();

                        //Point of intersection - f(x, y, z) = o + td;
                        Vector3d pt = camera.ray.getOrigin() + t * camera.ray.getDirection();

                        //For every object iterate over the lights in the scene
                        for (int light_i = 0; light_i < scene.numLights; light_i++) {


                            //Calculate the diffuse
                            Vector3d diffuse = scene.getSphere(object_i).material.Diffuse(scene.getLight(light_i).getOrigin(),
                                               scene.getSphere(object_i).getNormal(pt),
                                               pt);

                            //Specular calculation - (Vector3d lightPos, Vector3d rayOrigin, Vector3d normal, Vector3d intersectionPt) {
                            Vector3d specular = scene.getSphere(object_i).material.Specular(scene.getLight(light_i).getOrigin(),
                                                camera.ray.getOrigin(),
                                                scene.getSphere(object_i).getNormal(pt),
                                                pt);

                            //Add the light contribution of the current light to the current object
                            diffuse_contribution += diffuse;
                            specular_contribution += specular;

                            //Shoot shadow rays to all spheres and models
                            Vector3d intersectionScaled(pt[0] + 0.01, pt[1] + 0.01, pt[2] + 0.01);
                            double shadowT = 10;
                            Ray shadowRay;
                            shadowRay.createShadowRay(intersectionScaled, (scene.getLight(light_i).getOrigin() - pt).normalized());

                            //Make sure we are running this because the pixel isn't in shadow already!
                            if (isShaded) {
                                //Iterate over all spheres to try and find intersections for shadow rays
                                for (int shadowingObject = 0; shadowingObject < scene.numSpheres; shadowingObject++) {
                                    if (scene.getSphere(shadowingObject).hasIntersected(shadowRay, shadowT)) {
                                        // cout << scene.getSphere(shadowingObject) == scene.getSphere(shadowingObject) << endl;
                                        if (shadowingObject != object_i) {
                                            isShaded = false;
                                            break;
                                        }
                                    }
                                }


                                //Iterate over all models to try and find intersections for shadow rays with faces
                                for (int shadowingModel = 0; shadowingModel < scene.numModels; shadowingModel++) {
                                    for (int shadowingFace = 0; shadowingFace < scene.getModel(shadowingModel).numFaces; shadowingFace++) {
                                        if (scene.getModel(shadowingModel).faces[shadowingFace].hasIntersected(shadowRay, shadowT)) {
                                            isShaded = false;
                                            break;
                                        }
                                    }
                                }

                            }


                        }

                        //If it's a shadow point don't render
                        if (!isShaded) {
                            //Save the pixel color at the end
                            colorR(x, y) = max(ambient[0], 0.);
                            colorG(x, y) = max(ambient[1], 0.);
                            colorB(x, y) = max(ambient[2], 0.);

                            //Cancel transperency on these pixels
                            colorA(x, y) = 1.0;
                        } else {
                            //Save the pixel color at the end
                            colorR(x, y) = max(diffuse_contribution[0] + ambient[0] + specular_contribution[0], 0.);
                            colorG(x, y) = max(diffuse_contribution[1] + ambient[1] + specular_contribution[1], 0.);
                            colorB(x, y) = max(diffuse_contribution[2] + ambient[2] + specular_contribution[2], 0.);

                            //Cancel transperency on these pixels
                            colorA(x, y) = 1.0;
                        }
                    }

                }


                /************************************
                * Models - shadows, specular, diffuse
                ************************************/

                for (int model_index = 0; model_index < scene.numModels; model_index++) {

                    //Ambient shade calculation
                    Vector3d ambient = scene.getModel(model_index).material.Ambient();

                    for (int model_face = 0; model_face < scene.getModel(model_index).numFaces; model_face++) {

                        bool isShaded = true;

                        if (scene.getModel(model_index).faces[model_face].hasIntersected(camera.ray, t)) {

                            //Compute the point of intersection
                            Vector3d P = camera.ray.getOrigin() + (camera.ray.getDirection() * (t * camera.ray.getDirection().norm()));

                            //Compute the diffuse lighting of the material
                            Vector3d diffuse_contribution(0, 0, 0);

                            //Specular lighting of the material
                            Vector3d specular_contribution(0, 0, 0);

                            for (int light_i = 0; light_i < scene.numLights; light_i++) {

                                //Compute diffuse model
                                Vector3d diffuse = scene.getModel(model_index).material.Diffuse(scene.getLight(light_i).getOrigin(),
                                                   scene.getModel(model_index).faces[model_face].getNormal(),
                                                   P);

                                Vector3d specular = scene.getModel(model_index).material.Specular(scene.getLight(light_i).getOrigin(),
                                                    camera.ray.getOrigin(),
                                                    scene.getModel(model_index).faces[model_face].getNormal(),
                                                    P);

                                diffuse_contribution += diffuse;
                                specular_contribution += specular;

                                //Shoot shadow rays to all spheres and models
                                Vector3d intersectionScaled(P[0] + 0.01, P[1] + 0.01, P[2] + 0.01);
                                double shadowT = 10;
                                Ray shadowRay;
                                shadowRay.createShadowRay(intersectionScaled, (scene.getLight(light_i).getOrigin() - P).normalized());

                                //Make sure we are running this because the pixel isn't in shadow already!
                                if (isShaded) {
                                    //Iterate over all spheres to try and find intersections for shadow rays
                                    for (int shadowingObject = 0; shadowingObject < scene.numSpheres; shadowingObject++) {
                                        if (scene.getSphere(shadowingObject).hasIntersected(shadowRay, shadowT)) {
                                            // cout << scene.getSphere(shadowingObject) == scene.getSphere(shadowingObject) << endl;
                                            isShaded = false;
                                            break;
                                        }
                                    }


                                    //Iterate over all models to try and find intersections for shadow rays with faces
                                    for (int shadowingModel = 0; shadowingModel < scene.numModels; shadowingModel++) {
                                        //Get rid of trying to intersect with the model we are currently on before computing face shadows
                                        if (shadowingModel != model_index) {
                                            for (int shadowingFace = 0; shadowingFace < scene.getModel(shadowingModel).numFaces; shadowingFace++) {
                                                if (scene.getModel(shadowingModel).faces[shadowingFace].hasIntersected(shadowRay, shadowT)) {
                                                    isShaded = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                            //If it's shadow'd
                            if (!isShaded) {
                                colorR(x, y) = ambient[0];
                                colorG(x, y) = ambient[1];
                                colorB(x, y) = ambient[2];

                                //Cancel transperency on these pixels
                                colorA(x, y) = 1.0;
                            } else {
                                colorR(x, y) = max(diffuse_contribution[0] + specular_contribution[0] + ambient[0], 0.);
                                colorG(x, y) = max(diffuse_contribution[1] + specular_contribution[1] + ambient[1], 0.);
                                colorB(x, y) = max(diffuse_contribution[2] + specular_contribution[2] + ambient[2], 0.);

                                //Cancel transperency on these pixels
                                colorA(x, y) = 1.0;
                            }
                        }
                    }
                }


            }
        }

        /***********************
        * File saving opreations
        ***********************/
        if (numAnimationFrames > 1) {
            string folderName = "4";
            folderName += "/";
            string fileName = to_string(frameCounter);
            fileName += ".png";
            write_matrix_to_png(colorR, colorG, colorB, colorA, "../renders/animations/" + folderName + fileName);
            cout << "[Renderer] Image saved" << endl;
            frameCounter++;
        } else {
            string fileName = getDateAsString();
            fileName += ".png";
            write_matrix_to_png(colorR, colorG, colorB, colorA, "../renders/frames/" + fileName);
            cout << "[Renderer] Image " << fileName << " saved" << endl;
        }

    }
}

int main(int argc, char *argv[])
{
    RayTracer();
    return 0;
}
