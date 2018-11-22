#version 460 core

out vec4 colour[2];

uniform sampler2D texDiffuse;
uniform sampler2D texNorms;
uniform sampler2D texDiffuse2;

in vec2 tex;
in float a;
in vec3 normal;
in vec3 pos;
in vec3 tangent;
in vec3 binormal;


void main(){

	float grad = dot(normalize(normal),vec3(0,1,0));
	float pct = grad;

	

	mat3 TBN  = mat3(tangent, binormal, normal);
	vec3 norm = normalize(TBN * texture(texNorms,tex).rgb);





	colour[0] = mix(texture(texDiffuse2, tex),texture(texDiffuse,tex),pct);
	colour[1] = vec4(norm.xyz * 0.5f + 0.5f ,1.0f);

}

