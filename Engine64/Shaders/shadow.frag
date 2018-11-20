#version 460 core

out vec4 colour;

uniform sampler2D theTexture;
uniform sampler2DShadow shadowTex;

in vec2 tex;
in float a;
in vec3 n;
in vec3 pos;
in vec4 shadowPos;

vec3 LIGHTPOS = vec3(20.f,10.f,20.f);

void main(){

		colour = texture(theTexture,tex);
		colour.a = a;
		//
		//float light = max(dot(normalize(LIGHTPOS - pos),n),0.2f);
		//float dist = length(LIGHTPOS - pos);
		//
		//
		//light *= max(1-(dist/8000.f),0);
		//
		//light = max(min(light,0.8f),0.2f);
		//
		//colour.x *= light;  
		//colour.y *= light;
		//colour.z *= light;


		float shadow = 1.0f;

		if(shadowPos.w > 0.0f){
			shadow = textureProj ( shadowTex , shadowPos );
		}
		//shadow = (1.f-shadow) * 0.6f + 0.2f;

		colour.x *= shadow;  
		colour.y *= shadow;
		colour.z *= shadow;
		

}