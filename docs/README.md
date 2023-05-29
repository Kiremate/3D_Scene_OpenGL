3D_Scene_Renderer
This project is a simple 3D rendering engine, designed for educational purposes. It demonstrates the use of basic 3D graphics concepts, such as camera control, mesh loading, and rasterization.

Classes
Camera
The Camera class is responsible for representing the camera in the 3D scene. It stores information about the camera's position, orientation, and projection properties. The class provides methods to control the camera and to retrieve its view and projection matrices.

Mesh
The Mesh class represents a 3D mesh in the scene. It inherits from the Node class, which allows it to be transformed and organized hierarchically. The Mesh class loads mesh data from a file using the Assimp library, and it stores the original vertices, indices, and colors, as well as transformed vertices and display vertices.

MeshData
The MeshData class is a helper class that stores vertex, index, and color data for a mesh. It provides methods to manipulate and access the mesh data.

Node
The Node class is a base class for objects in the scene. It provides basic functionality for hierarchical transformations and updates. Classes that inherit from Node can implement their own update and transformation logic, making it easy to create complex scenes with multiple objects.

Rasterizer
The Rasterizer class is responsible for rasterizing the 3D scene onto a 2D buffer. It takes the transformed vertices from the Mesh class and projects them onto the screen. The class provides methods to draw polygons, clear the buffer, and blit the buffer to the window.

View
The View class manages the main rendering loop and maintains references to the camera, rasterizer, and scene objects. It is responsible for updating the camera and objects, as well as rendering the scene using the rasterizer.

Main
The Main.cpp file is the entry point of the application. It initializes the window, creates an instance of the View class, and enters the main event loop.

How to use
Compile and run the project using your preferred C++ compiler and development environment.
Interact with the 3D scene using the provided camera controls.
Dependencies
This project requires the following libraries:

Assimp (Asset Importer Library): for loading 3D mesh data from various file formats.
SDL2 (Simple DirectMedia Layer): for creating a window, handling user input, and displaying the rendered image.