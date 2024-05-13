#version 460

uniform float offset_x;
uniform float offset_y;

void main() {
    if (gl_VertexID == 0)
        gl_Position = vec4(-0.25 + offset_x, -0.25 + offset_y, 0.0, 1.0);

    else if (gl_VertexID == 1)
        gl_Position = vec4(-0.25 + offset_x, 0.25 + offset_y, 0.0, 1.0);

    else
        gl_Position = vec4(0.25 + offset_x, 0.0 + offset_y, 0.0, 1.0);
}
