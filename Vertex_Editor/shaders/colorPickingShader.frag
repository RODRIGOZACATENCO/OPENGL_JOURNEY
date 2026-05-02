#version 460 core


out uvec4 FragColor;
void main()
{
    uint faceID=uint(gl_PrimitiveID)+1u;// Add 1 to avoid using 0 as a valid ID
    FragColor = uvec4(faceID,0,0,0);
}