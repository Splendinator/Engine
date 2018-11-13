#version 460 core

out vec4 colour;

uniform sampler2D theTexture;

in vec2 tex;
in float a;
in vec3 n;
in vec3 pos;

vec3 LIGHTPOS = vec3(20.f,10.f,20.f);

void main(){

		colour = texture(theTexture,tex);
		colour.a = a;
		
		float light = max(dot(normalize(LIGHTPOS - pos),n),0.2f);
		float dist = length(LIGHTPOS - pos);


		light *= max(1-(dist/70.f),0);

		light = max(min(light,0.8f),0.2f);

		colour.x *= light;  
		colour.y *= light;
		colour.z *= light;
		
		

}