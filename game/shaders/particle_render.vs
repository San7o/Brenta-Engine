#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 2) in float inTTL;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float drawOrDie;

void main() {
   
    if (inTTL < 0.0) {
        drawOrDie = -1.0;
    }
    else {
        drawOrDie = 1.0;
    }

    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    gl_PointSize = 10.0;
}
