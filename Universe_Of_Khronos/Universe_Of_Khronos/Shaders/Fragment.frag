#version 330 core

struct Material {
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

struct Light {
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform Light light;
uniform Material material;

out vec4 color;

in vec3 Normal;
in vec3 FragPos; 

uniform vec3 LightPos;
uniform vec3 ViewPos;

void main()
{
	// Ambient
	vec3 Ambient = light.Ambient * material.Ambient;
	
	// Diffuse
	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	float Diff = max(dot(Norm, LightDir), 0.0);
	vec3 Diffuse = light.Diffuse * (Diff * material.Diffuse);

	// Specular
	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec3 ReflectionDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectionDir), 0.0), material.Shininess);
	vec3 Specular = light.Specular * (Spec * material.Specular);

	vec3 Result = Ambient + Diffuse + Specular;
	color = vec4(Result, 1.0f);
}