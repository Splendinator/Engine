#version 460 core

out vec4 colour;

uniform sampler2D texDiffuse;
uniform sampler2D texEmissive;
uniform sampler2D texSpecular;

in vec2 tex;
in float a;
in vec3 n; 
in vec3 pos;


void main(){

	vec3 diffuse = texture(texDiffuse, tex).xyz ;
	vec3 light = texture(texEmissive, tex).xyz ;
	vec3 specular = texture(texSpecular, tex).xyz ;

	colour.xyz = diffuse * 0.2f;
	colour.xyw += diffuse * light;
	colour.xyz += specular;
	colour.a = 1.0f;

}