#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float drawOrDie[];
out float drawOrDieFrag;

void main() {

    drawOrDieFrag = drawOrDie[0];

    gl_Position = gl_in[0].gl_Position + vec4(-1.0, -1.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(1.0, -1.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-1.0, 1.0, 0.0, 0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}
