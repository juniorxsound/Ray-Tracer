# Ray-Tracer
A simple ray tracing program for rendering 3D scene written in C++.
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Dependencies](#dependencies)

![Final Image](https://github.com/juniorxsound/Ray-Tracer/blob/master/resources/2.png)


### Features:
- Camera (Projection, Perspective)
- Shading (Ambient, Diffuse, Specular)
- Shadows
- Reflections
- Plane, sphere and custom geometry
- Simple scene graph
- Simple animations

### Installation:

Clone/fork repository (make sure to use the recursive flat to fetch submodules too, e.g ```git clone http://repo.link --recursive```)

```mkdir build``` and ```cd build```

run ```cmake ../ -DCMAKE_BUILD_TYPE=Release```

run ```make```

run ```./Raytracer```

### Usage:

The ray tracer uses a simple scene graph structure which makes it easier to add/remove objects from the rendered scene. For example this is how you could create a scene with a sphere:
```cpp
    //Instance a scene
    Scene scene;
  
    //Create a perspective camera (1st parameter is position)
    Camera::PerspectiveCamera camera(Vector3d(0, 0, -2));
  
    //Create a diffused material
    Material diffusedMat(0.2, //Ambient intensity
                         Vector3d(1, 1, 1.5), // Ambient coefficiencies
                         0.4, // Diffuse intensity
                         Vector3d(1, 1, 1.5), // Diffuse coefficiencies
                         0,//Specular intensity
                         Vector3d(1, 1, 1), //Specular coefficiencies
                         1000); // Phong exponent

    
    //Instance a sphere
    Sphere sphere(Vector3d(-1, .5, 3), 0.5);
    sphere.setMaterial(specularMat);
    scene.add(sphere);
```
To add a light to the same scene we would do this
```cpp
  // Instance a light (vec3 position, float intensity)
  Light light(Vector3d(-3, .5, -1.8), 1);
  scene.add(light);
```
And finally to load an .off model we would do this
```cpp
  //First argument is a position vector
  Model model(Vector3d(-4, 10, 50));
  model.loadOFF("bumpy_cube.off");
  model.setMaterial(specularMat);
```

### Dependencies:
- [Eigen](https://github.com/libigl/eigen)

> Tested on macOS (10.13.2) using CMake (3.10.0)

