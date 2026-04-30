#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1 ) in uint inFaceID;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

flat out uint FaceID;
void main()
{
    FaceID=inFaceID;
    gl_Position = projection*view*model*vec4(aPos, 1.0);

}