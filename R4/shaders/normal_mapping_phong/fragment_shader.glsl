#version 330

// === Uniform Structures ===  //

struct LightSource
{
  vec3 pos;  // Center coordinates.
  vec3 dir;  // Direction vector.

  vec3 Ld;  // Diffuse component  (in [0, 1]).
  vec3 Ls;  // Specular component (in [0, 1]).

  float alpha;  // Shininess of specular component.
  int is_spotlight;
  float cone_angle;
};

struct Material
{
  vec3 Ka;  // Ambient component (in [0, 1]).
  vec3 Kd;  // Diffuse component (in [0, 1]).
  vec3 Ks;  // Specular component (in [0, 1]).
};

// === I/O === //

// Per-fragment data:
in vec4 f_position;
in vec4 f_normal;
in vec2 f_uv;
in vec4 f_tangent;

out vec4 pixel_color;

// === Light Sources === //
const int max_num_lights = 8;
uniform int lighting = 0;  // Boolean.

uniform int num_lights = 1;                 // Number of light sources.
uniform int light_switch[max_num_lights];   // Array of light source states (on/off).

uniform vec3 La = vec3(0.1);                // Ambient light component.
uniform LightSource light[max_num_lights];  // Array of light sources.

// === Texture === //
uniform int use_color_map  = 1;
uniform int use_normal_map = 1;
uniform sampler2D color_map;
uniform sampler2D normal_map;

// === Material === //
uniform Material material;

// === Code === //

void NormalFromMap(in vec3 normal, out vec3 transformed_n)
{
	vec3 n = normal;
    vec3 t = f_tangent.xyz;

    // Normal map provides coordinates in the fragment coordinate system.
    // Since we have both normal and tangent vectors, we can calculate the bitangent,
    // build a basis and transform normal map coordinates to world coordinates to
    // compute the lighting.
    vec3 b = cross(n, t);   // b = n x t.
    mat3 M = mat3(t, b, n);
	
    // rgb to normal.
    n = texture(normal_map, f_uv).xyz;
    n = 2*n - vec3(1.0);

    transformed_n = M * n; 
}

// Attenuation decreases linearly with the angle.
float CalculateSpotlightAttenuation(in vec3 fragToLight, int i)
{
	float cone_angle_inv = 1.0f/light[i].cone_angle;
	float similarity = max(-dot(light[i].dir, fragToLight), 0.0);
	float angle = degrees(acos(similarity));

	return max((-cone_angle_inv*angle + 1.0f), 0.0f);
}

void main()
{
  vec4 diffuse_color = texture(color_map, f_uv);

  if (use_color_map == 0)
  {
    diffuse_color = vec4(material.Kd, 1.0);
  }

  if (lighting == 0)  // off.
  {
    pixel_color = vec4(material.Kd, 1.0);
  }
  else  // on.
  {
    vec3 Ka = material.Ka;
    vec3 Kd = diffuse_color.xyz;
    vec3 Ks = material.Ks;

    // Fragment data and light sources are in camera coordinates.
    vec3 I = Ka*La;
	vec3 n = f_normal.xyz;
	
	if (use_normal_map == 1) 
	{
		NormalFromMap(n, n);
	}

    for (int i = 0; i < num_lights; i++)
    {
      if (light_switch[i] == 0)  // Off!
        continue;

      vec3 l  = normalize(light[i].pos - f_position.xyz);  // Unit vector from fragment to light source.
      float attenuation = 1.0f;
	  
	  if (light[i].is_spotlight == 1)
	  {
		attenuation = CalculateSpotlightAttenuation(l, i);
		
		if (attenuation == 0.0f)
			continue;
	  }
 
	  vec3 r  = -reflect(l, n);                            // Reflection of light ray on fragment.
      vec3 f = normalize(-f_position.xyz);                 // Unit vector from fragment to camera (origin).
      float d =    length(light[i].pos - f_position.xyz);  // Distance from fragment to light source.
      float alpha = light[i].alpha;

      vec3 Id = light[i].Ld * max(dot(n, l), 0);              // Diffuse component.
      vec3 Is = light[i].Ls * pow(max(dot(r, f), 0), alpha);  // Specular component.
	  
	  I += attenuation*(Kd*Id + Ks*Is);
    }
    
    pixel_color = vec4(I, 1.0);
  }
}