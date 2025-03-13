#include <iostream>
#include <vector>
#include "LoadBitmap.cpp" // Include the BMP loader
#include <GL/glew.h>

class TextureLoader {
public:
    static GLuint loadTexture(const char* filename) {
        unsigned char* data;
        unsigned int width, height;
        
        // Load the BMP file
        loadARGB_BMP(filename, &data, &width, &height);
        if (!data) {
            std::cerr << "Error: Could not load texture file " << filename << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload texture data to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free memory
        delete[] data;
        
        std::cout << "Successfully loaded texture " << filename << " (" << width << "x" << height << ")" << std::endl;
        return textureID;
    }
};
