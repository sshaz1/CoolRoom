#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TexturedMesh.cpp"

// Shader program ID
GLuint shaderProgram;

// Camera attributes
glm::vec3 cameraPos   = glm::vec3(0.5f, 0.4f, 0.5f); // Initial camera position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Direction the camera is facing
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f); // Defines the "up" direction
float cameraYaw = -90.0f; // Initial yaw angle to align with -Z axis
float cameraSpeed = 0.05f;  // Camera movement speed
float rotationSpeed = 3.0f; // Camera rotation speed

void processInput(GLFWwindow *window) {
    // Move forward/backward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    // Rotate left/right
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraYaw -= rotationSpeed;  // Rotate counter-clockwise
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraYaw += rotationSpeed;  // Rotate clockwise

    // Update cameraFront direction based on yaw rotation
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw));
    front.z = sin(glm::radians(cameraYaw));
    cameraFront = glm::normalize(front);
}


// Function to read a shader file and return its content as a string
std::string readShaderFile(const char* filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile) {
        std::cerr << "Error: Could not open shader file " << filePath << std::endl;
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

// Compiles shaders and links them into a shader program
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);
    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Error: Shader file is empty" << std::endl;
        return 0;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Compilation Error: " << infoLog << std::endl;
        return 0;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Compilation Error: " << infoLog << std::endl;
        return 0;
    }

    // Link shaders into a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program Linking Error: " << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    std::cout << "Shader Program Compiled Successfully!\n";
    return shaderProgram;
}

// Projection and View Matrices
glm::mat4 projection;
glm::mat4 view;

// List of textured meshes
std::vector<TexturedMesh> meshes;

// Function to initialize projection and view matrices
void setupCamera() {
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    view = glm::lookAt(
        glm::vec3(0.5f, 0.4f, 0.5f),  // Camera position
        glm::vec3(0.5f, 0.4f, 0.5f) + glm::vec3(0.0f, 0.0f, -1.0f), // Look-at point should be forward from the camera
        glm::vec3(0.0f, 1.0f, 0.0f)   // Up direction
    );
}


// Function to load all 10 PLY objects
void loadMeshes() {
    std::vector<std::pair<std::string, std::string>> files = {
        {"Bottles.ply", "bottles.bmp"},
        {"Floor.ply", "floor.bmp"},
        {"Patio.ply", "patio.bmp"},
        {"Table.ply", "table.bmp"},
        {"Walls.ply", "walls.bmp"},
        {"WindowBG.ply", "windowbg.bmp"},
        {"WoodObjects.ply", "woodobjects.bmp"},
        {"DoorBG.ply", "doorbg.bmp"},
        {"MetalObjects.ply", "metalobjects.bmp"},
        {"Curtains.ply", "curtains.bmp"}
    };

    for (const auto& file : files) {
        meshes.emplace_back(file.first, file.second, shaderProgram);
    }
}

// Main rendering loop
void runRenderingLoop() {
    // Initialize OpenGL and GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Assignment4", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    shaderProgram = createShaderProgram("shader.vert", "shader.frag");
    if (!shaderProgram) {
        std::cerr << "Failed to create shader program" << std::endl;
        return;
    }

    // Set up projection and view matrices
    setupCamera();

    // Load all objects
    loadMeshes();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);  // Handle movement input

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update View Matrix
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // Compute Model-View-Projection Matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 MVP = projection * view * model;
        
        // Render all objects
        for (TexturedMesh &mesh : meshes) {
            mesh.draw(MVP);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main() {
    runRenderingLoop();
    return 0;
}
