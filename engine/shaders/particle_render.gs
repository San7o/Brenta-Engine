#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float drawOrDie[];

uniform int atlas_width;
uniform int atlas_height;
uniform int atlas_index;
uniform float scale = 1.0;
uniform float aspect_ratio = 1.0;

out vec2 TexCoordFrag;
out float drawOrDieFrag;

void main() {

    drawOrDieFrag = drawOrDie[0];
    float square_x = 1.0 / atlas_width;
    float square_y = 1.0 / atlas_height;
    int row_index = atlas_index / atlas_width;
    int col_index = atlas_index % atlas_width;

    float adjusted_scale_x = scale / (aspect_ratio < 1.0 ? aspect_ratio : 1.0);
    float adjusted_scale_y = scale / (aspect_ratio > 1.0 ? 1.0 / aspect_ratio : 1.0);

    gl_Position = gl_in[0].gl_Position + vec4(-1.0 * adjusted_scale_x,
                                              -1.0 * adjusted_scale_y, 0.0, 0.0);
    TexCoordFrag = vec2(square_x * col_index,
                        square_y * (atlas_height - row_index - 1));
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(1.0 * adjusted_scale_x,
                                             -1.0 * adjusted_scale_y, 0.0, 0.0);
    TexCoordFrag = vec2(square_x * (col_index + 1),
                        square_y * (atlas_height - row_index - 1));
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-1.0 * adjusted_scale_x,
                                               1.0 * adjusted_scale_y, 0.0, 0.0);
    TexCoordFrag = vec2(square_x * col_index,
                        square_y * (atlas_height - row_index));
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(1.0 * adjusted_scale_x,
                                              1.0 * adjusted_scale_y, 0.0, 0.0);
    TexCoordFrag = vec2(square_x * (col_index + 1),
                        square_y * (atlas_height - row_index));
    EmitVertex();
    EndPrimitive();
}
