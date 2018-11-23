#version 460 core

out vec4 colour[2];

uniform sampler2D texDiffuse;
uniform sampler2D texNorms;
uniform sampler2DShadow texShadow;
uniform samplerCube texReflect;
uniform vec3 cameraPos;

uniform float reflectivity;

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

	vec4 diffuse = texture(texDiffuse, tex);

	colour[1] = vec4(norm.xyz * 0.5f + 0.5f ,1.0f);


	float shadow = 1.0f;

	//if(shadowPos.w < 0.0f){
		shadow = textureProj(texShadow,shadowPos);
	//}

		diffuse.x *= shadow;  
		diffuse.y *= shadow;
		diffuse.z *= shadow;


	vec3 incident = normalize (pos + cameraPos);

	vec4 reflection = texture(texReflect, reflect(incident, normalize(norm)));

	colour[0] = mix(reflection,diffuse,reflectivity);


}
