#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

// Structure to store vertex data, including position, normal, color, and texture coordinates.
struct VertexData {
    glm::vec3 position;  // 3D Position (x, y, z)
    glm::vec3 normal = glm::vec3(0.0f);  // Normal vector (default to 0)
    glm::vec3 color = glm::vec3(1.0f);  // Color (default white)
    glm::vec2 texCoords = glm::vec2(0.0f);  // Texture coordinates (default 0)
};

// Structure to store triangle face data using indices of three vertices.
struct TriData {
    int v1, v2, v3;
};

// Function to read a PLY file and extract vertex and face data.
void readPLYFile(const std::string& fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces) {
    std::ifstream file(fname);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open PLY file " << fname << std::endl;
        return;
    }

    std::string line;
    bool headerEnded = false;
    int vertexCount = 0, faceCount = 0;
    bool hasNormals = false, hasColor = false, hasUV = false;
    
    // Parse the PLY file header to determine what attributes are available
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;
        
        if (token == "element") {
            std::string type;
            ss >> type;
            if (type == "vertex") {
                ss >> vertexCount;  // Read number of vertices
            } else if (type == "face") {
                ss >> faceCount;  // Read number of faces
            }
        } else if (token == "property") {
            std::string dataType, propName;
            ss >> dataType >> propName;
            if (propName == "nx" || propName == "ny" || propName == "nz") hasNormals = true;
            if (propName == "red" || propName == "green" || propName == "blue") hasColor = true;
            if (propName == "u" || propName == "v") hasUV = true;
        } else if (token == "end_header") {
            headerEnded = true;
            break;
        }
    }
    
    if (!headerEnded) {
        std::cerr << "Error: PLY file header is incomplete." << std::endl;
        return;
    }
    
    // Reserve space for vertices and faces in vectors
    vertices.reserve(vertexCount);
    faces.reserve(faceCount);
    
    // Read vertex data
    for (int i = 0; i < vertexCount; i++) {
        VertexData vertex;
        file >> vertex.position.x >> vertex.position.y >> vertex.position.z;  // Read required position values
        if (hasNormals) file >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;  // Read normal if available
        if (hasColor) file >> vertex.color.r >> vertex.color.g >> vertex.color.b;  // Read color if available
        if (hasUV) file >> vertex.texCoords.x >> vertex.texCoords.y;  // Read texture coordinates if available
        vertices.push_back(vertex);
    }
    
    // Read face data (triangle indices)
    for (int i = 0; i < faceCount; i++) {
        int count;
        TriData face;
        file >> count; // First value should always be 3 for triangles
        if (count != 3) {
            std::cerr << "Error: Non-triangle face found in " << fname << std::endl;
            return;
        }
        file >> face.v1 >> face.v2 >> face.v3;  // Read indices of the three vertices forming the triangle
        faces.push_back(face);
    }
    
    std::cout << "Successfully loaded " << vertexCount << " vertices and " << faceCount << " faces from " << fname << std::endl;
}
