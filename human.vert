#version 410 core
layout (location = 0) in vec3 vert_pos; //xyz = pos, w = face siding

uniform mat4 PV;
uniform mat4 M;

flat out vec3 color;

void main() {
  gl_Position = PV * vec4(vert_pos.xyz, 1.0);
  //gl_Position = vec4(vert_pos.xyz, 1.0);
  color = vec3(1.0, 0.0, 0.0);
}
