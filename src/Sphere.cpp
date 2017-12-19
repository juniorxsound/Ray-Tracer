#include "Sphere.h"

Sphere::Sphere(Eigen::Vector3d orig, double rad) {
    o = orig;
    r = rad;
}

Sphere::~Sphere() {}

bool Sphere::hasIntersected(Ray ray, double &t) {
    double t0, t1;
    //Get the values of the ray
    Eigen::Vector3d e = ray.getOrigin();
    Eigen::Vector3d rd = ray.getDirection();

    Eigen::Vector3d L = e - o;
    double a = rd.dot(rd);
    double b = 2 * rd.dot(L);
    double c = L.dot(L) - r * r;

    //See if there is an intersection
    double disc = b * b - 4 * a * c;

    // T optional intersections
    double smallT;
    double bigT;

    //If the disc is smaller than zero we have no intersections
    if (disc < 0) return false;
    //Second option is that it's zero and we have one intersection
    else if (disc == 0) {
        smallT = - 0.5 * b / a;
        bigT = - 0.5 * b / a;
    } else {
        //Get rid of the square root
        disc = std::sqrt(disc);

        //Solve the quadratic formula for both smallT (option 1) and bigT (option 2)
        double optionalSolutions;
        if (b > 0) {
            optionalSolutions = -0.5 * (b + disc);
        } else {
            optionalSolutions = -0.5 * (b - disc);
        }
        smallT = optionalSolutions / a;
        bigT = c / optionalSolutions;
    }

    if (smallT > bigT) {
        t = bigT;
    } else {
        t = smallT;
    }


    // t = t0; //Set the reference to the lowest t value
    return true;
}

//Returns the sphere radius
double Sphere::getRadius() { return r; }

// Return a simple sphere normal calculation
Eigen::Vector3d Sphere::getNormal(Eigen::Vector3d &point) {
    return (point - o).normalized();
}

//Save a reference to a material instance
void Sphere::setMaterial(Material &mat) {
    material = mat;
}