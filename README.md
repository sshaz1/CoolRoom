# CoolRoom
Loads triangle mesh data and textures from files, renders them, and allows the user to explore that rendered world by moving the camera around via arrow keys.

## What Was Implemented & Why:

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

<img width="642" alt="1" src="https://github.com/user-attachments/assets/0df206c5-7b38-4013-9ad6-970c2899ae83" />
<img width="642" alt="2" src="https://github.com/user-attachments/assets/55da3508-8106-454e-b653-cfe24ebdd197" />
<img width="642" alt="3" src="https://github.com/user-attachments/assets/59233262-3106-4cb8-91b4-563d980f1ac4" />
