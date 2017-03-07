#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;
// layout (location = 3) in vec3 v_tangent;

out vec4 f_position;  // Fragment position in camera coordinates.
out vec4 f_normal;    // Fragment normal in camera coordinates.
out vec2 f_uv;        // Fragment uv coordinates.

// out vec4 f_tangent;   // Fragment tangent vector in camera coordinates.

uniform mat4 M;  // Model matrix.
uniform mat4 V;  // View  matrix.
uniform mat4 P;  // Projection matrix.
uniform mat4 N;  // Normal matrix N = (VM)^-t.

// const float C = 1;
// const float far = 1000;

void main()
{
  // Compute vertex position in world coordinates.
  f_position = V * (M * vec4(v_position, 1.0f));
  f_position = f_position/f_position.w;

  // Then project f_position onto screen and store into gl_Position.
  gl_Position = P * f_position;
  // gl_Position.z = 2.0*log(gl_Position.w*C + 1)/log(far*C + 1) - 1;
  // gl_Position.z *= gl_Position.w;

  // Transform the vertex normal vector.
  f_normal = normalize(V * N * vec4(v_normal, 0.0));

  // Pass uv coordinates to be interpolated.
  f_uv = v_uv;
}