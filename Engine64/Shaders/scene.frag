#version 460 core

out vec4 colour[2];

uniform sampler2D texDiffuse;
uniform sampler2D texNorms;

in vec2 tex;
in float a;
in vec3 normal;
in vec3 pos;
in vec3 tangent;
in vec3 binormal;

vec3 LIGHTPOS = vec3(20.f,10.f,20.f);

void main(){

	mat3 TBN  = mat3(tangent, binormal, normal);
	vec3 norm = normalize(TBN * texture(texNorms,tex).rgb);

	colour[0] = texture(texDiffuse, tex);
	colour[1] = vec4(norm.xyz * 0.5f + 0.5f ,1.0f);

}