#version 330 core

layout (location = 0) in vec3 inPosition; // current particle position
layout (location = 1) in vec3 inVelocity; // current particle velocity

out vec3 outPosition;
out vec3 outVelocity;

uniform float deltaTime;
uniform vec3 gravity = vec3(0.0, 9.8, 0.0);

void main() {

    vec3 newVelocity = inVelocity + gravity * deltaTime;
    vec3 newPosition = inPosition + newVelocity * deltaTime;
    
    outVelocity = newVelocity;
    outPosition = newPosition;
}
