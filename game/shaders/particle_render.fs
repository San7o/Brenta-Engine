#version 330 core

in float drawOrDieFrag;
out vec4 FragColor;

void main() {

    if (drawOrDieFrag < 0.0) {
        //FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for dead
        discard;
    }
    else {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // White
    }
}
