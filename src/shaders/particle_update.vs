#version 330 core

layout (location = 0) in vec3 inPosition; // current particle position
layout (location = 1) in vec3 inVelocity; // current particle velocity
layout (location = 2) in float inTTL;       // current particle life

out vec3 outPosition;
out vec3 outVelocity;
out float outTTL;

uniform float deltaTime;
uniform vec3 gravity = vec3(0.0, -2.0, 0.0);
uniform vec3 emitterVel = vec3(0.0, 0.0, 0.0);
uniform vec3 emitterPos = vec3(0.0, 0.0, 0.0);
uniform vec3 emitterSpread = vec3(1.0, 0.0, 1.0);
uniform float emitterTTL = 3.0;
uniform float spawnProbability = 0.01;

// Random number generator between [-1, 1]
float rand(vec2 co);

void main() {

    float random = rand(vec2(deltaTime * float(gl_VertexID)));
    if (random < spawnProbability) {
        outPosition = emitterPos;
        outVelocity = emitterVel + emitterSpread *
                      vec3(rand(vec2(float(gl_VertexID + 1.0))) -0.5,
                           rand(vec2(float(gl_VertexID + 2.0))) -0.5,
                           rand(vec2(float(gl_VertexID + 3.0))) -0.5);
        outTTL = emitterTTL;
        return;
    }

    if (inTTL <= 0.0) {
        outPosition = inPosition;
        outVelocity = vec3(0.0);
        outTTL = -1.0;
        return;
    }

    outTTL = inTTL - deltaTime;

    vec3 processed_velocity = inVelocity;
    float delta = 0.0;

    vec3 newVelocity = processed_velocity + gravity * deltaTime;
    vec3 newPosition = inPosition + newVelocity * deltaTime;
    
    outVelocity = newVelocity;
    outPosition = newPosition;
}
    
float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
