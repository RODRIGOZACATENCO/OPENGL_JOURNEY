#version 460 core


out vec4 FragColor;
void main()
{
    uint faceID=uint(gl_PrimitiveID)+1u;
    FragColor = uvec4(faceID,0,0,0);
}