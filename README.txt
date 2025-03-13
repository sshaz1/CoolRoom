Files Submitted: LoadPly.cpp, LoadTexture.cpp, LoadBitmap.cpp, TexturedMesh.cpp, 1.png, 2.png, 3.png.

What Was Implemented & Why:

1. Textured Mesh Loading
PLY File Parsing (LoadPly.cpp): Reads vertex positions, normals, texture coordinates, and faces from PLY files.
Texture Loading (LoadTexture.cpp & LoadBitmap.cpp): Loads BMP images and converts them into OpenGL-compatible textures.
VAO & VBO Setup (TexturedMesh.cpp): Stores vertex data efficiently using OpenGL buffers.

2. Rendering Pipeline
Shaders (shader.vert, shader.frag): Handles transformation and texture mapping.
MVP Matrix (MainRendering.cpp): Computes and passes the Model-View-Projection matrix to shaders.
Blending & Transparency: Uses glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) to ensure proper rendering of transparent objects like curtains and windows.
Depth Testing: Uses glEnable(GL_DEPTH_TEST) to handle occlusion and prevent objects from overlapping incorrectly.

3. First-Person Camera (MainRendering.cpp)
Movement (Arrow Keys):
↑ / ↓ : Move forward/backward.
← / → : Rotate left/right in place.
Yaw Rotation: The camera’s front direction updates using trigonometry (cos / sin).
Free Movement: No collision detection, allowing unrestricted navigation.

How I setup the camera for screenshots:
1.png: Set it up very similarly to the first example in assignment 4 pdf.
2.png: From the window peeking into the room.
3.png: Very similar to the second example in the pdf.

Compiling:
Compile with glfw, glm, and glew. Make sure to have all the bmp & ply files in the same directory as my cpp files. Run the exe file.
