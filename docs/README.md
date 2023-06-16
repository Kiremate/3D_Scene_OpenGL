# 3D_Scene_OpenGL
This project is a 3D scene rendered using OpenGL. It consists of multiple classes that handle different aspects of the scene, such as the camera, mesh rendering, skybox, and texture management.

Classes
Camera.h: Defines the camera class, which represents the viewpoint and handles camera transformations.
Mesh.h: Handles loading and rendering of a mesh object.
MeshData.h: Contains the data structure for storing mesh information.
Node.h: Represents a node in a scene graph, used for hierarchical transformations.
Skybox.h: Manages the rendering of a skybox.
TextureManager.h: Handles loading and managing textures.
View.hpp: Implements the main view class, which sets up the OpenGL context and handles rendering the scene.
Main Components
View
The View class is the main class responsible for rendering the 3D scene. It handles the initialization of the OpenGL context, manages the camera, skybox, shaders, and rendering of the mesh. The View class also handles user input and updates the scene accordingly.

Rendering
void update(): This method is called to update the scene. It handles any necessary transformations, such as camera rotations.
void render(): This method is called to render the scene. It starts by clearing the color and depth buffers, renders the skybox, sets up the shader program for the mesh, sends transformation matrices to the shader, and draws the mesh. It also includes post-processing effects by binding the framebuffer and rendering a quad with a texture.
Initialization and Setup

void resize(int new_width, int new_height): This method is called when the window is resized. It updates the viewport and aspect ratio of the camera.
GLuint compile_shaders(): This method compiles the vertex and fragment shaders for the main rendering.
GLuint compile_postprocessing_shaders(): This method compiles the vertex and fragment shaders for post-processing effects.
void load_mesh(const std::string& mesh_file_path): This method loads a mesh from a file using the Assimp library. It generates vertex and index buffers for rendering the mesh.

User Input
void on_key(int key_code): This method is called when a key is pressed. It can be used to handle key-based interactions.
void on_drag(int pointer_x, int pointer_y): This method is called when the mouse is dragged. It updates the rotation angles based on the mouse movement.
void on_click(int pointer_x, int pointer_y, bool down): This method is called when a mouse button is clicked or released. It handles the mouse button state and updates the rotation angles accordingly.

Shaders
The class includes the vertex and fragment shader code as static string constants.

These are the key functionalities provided by the View class to handle the rendering and interaction with the 3D scene using OpenGL.
TextureManager
The TextureManager class handles the loading and management of textures. It provides methods to load textures from files and retrieve the OpenGL texture ID associated with a specific texture.

Usage
To use the 3D scene, follow these steps:

Include the necessary header files in your project: Camera.h, Mesh.h, MeshData.h, Node.h, Skybox.h, Texture_Cube.h, TextureManager.h, and View.hpp.

Create an instance of the View class and provide the width and height of the window.

Inside your main loop, call the update() and render() methods of the View instance to update and render the scene, respectively.

Handle user input events and pass them to the View instance by calling the appropriate methods, such as on_key, on_drag, and on_click to move the camera.