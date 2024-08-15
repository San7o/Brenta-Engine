#version 330 core

layout (location = 0) in vec3 inPosition; // current particle position
layout (location = 1) in vec3 inVelocity; // current particle velocity
layout (location = 2) in float inTTL;       // current particle life

out vec3 outPosition;
out vec3 outVelocity;
out float outTTL;

uniform float deltaTime;
uniform vec3 gravity = vec3(0.0, -2.0, 0.0);

// Random number generator between [-1, 1]
float rand(vec2 co);

void main() {

    if (inTTL <= 0.0) {
        outPosition = inPosition;
        outVelocity = vec3(0.0);
        outTTL = 0.0;
        return;
    }

    outTTL = inTTL - deltaTime;

    vec3 processed_velocity = inVelocity;
    float delta = 0.0;

    if (inVelocity.x == 0.0) {
        delta = rand(vec2(float(gl_VertexID)));
        processed_velocity += vec3(delta, 0.0, 0.0);
    }
    if (inVelocity.z == 0.0) {
        float delta = rand(vec2(float(gl_VertexID) + delta));
        processed_velocity += vec3(0.0, 0.0, delta);
    }

    vec3 newVelocity = processed_velocity + gravity * deltaTime;
    vec3 newPosition = inPosition + newVelocity * deltaTime;
    
    outVelocity = newVelocity;
    outPosition = newPosition;
}
    
float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
