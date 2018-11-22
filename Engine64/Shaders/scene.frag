#version 460 core

out vec4 colour[2];

uniform sampler2D texDiffuse;
uniform sampler2D texNorms;
uniform sampler2DShadow texShadow;

in vec2 tex;
in float a;
in vec3 normal;
in vec3 pos;
in vec3 tangent;
in vec3 binormal;
in vec4 shadowPos;


void main(){

	mat3 TBN  = mat3(tangent, binormal, normal);
	vec3 norm = normalize(TBN * texture(texNorms,tex).rgb);

	colour[0] = texture(texDiffuse, tex);
	colour[1] = vec4(norm.xyz * 0.5f + 0.5f ,1.0f);


	float shadow = 1.0f;

	//if(shadowPos.w < 0.0f){
		shadow = textureProj(texShadow,shadowPos);
	//}

		colour[0].x *= shadow;  
		colour[0].y *= shadow;
		colour[0].z *= shadow;


}

