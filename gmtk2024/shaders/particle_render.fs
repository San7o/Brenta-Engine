#version 330 core

in float drawOrDieFrag;
in vec2 TexCoordFrag;

out vec4 FragColor;
uniform sampler2D sprite;

void main() {

    if (drawOrDieFrag < 0.0) {
        discard;
    }
    else {
        FragColor = texture(sprite, TexCoordFrag);
        if (FragColor.a < 0.1) {
            discard;
        }
        //FragColor = vec4(0.0, 1.0, 0.0, 1.0); // White
    }
}
