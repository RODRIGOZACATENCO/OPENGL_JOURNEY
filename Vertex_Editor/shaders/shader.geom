#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Barycentric;
void main()
{
    uint incomingFaceID = uint(gl_PrimitiveIDIn);
    gl_Position = gl_in[0].gl_Position;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    Barycentric = vec3(1.0, 0.0, 0.0); // 100% pull from Corner A
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    Barycentric = vec3(0.0, 1.0, 0.0); // 100% pull from Corner B
    EmitVertex();


    gl_Position = gl_in[2].gl_Position;
    gl_PrimitiveID = gl_PrimitiveIDIn;
    Barycentric = vec3(0.0, 0.0, 1.0); // 100% pull from Corner C
    EmitVertex();

    // Close the triangle
    EndPrimitive();
}