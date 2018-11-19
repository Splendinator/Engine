#version 460 core

out vec4 colour;

uniform sampler2D theTexture;
uniform sampler2D theTexture2;

in vec2 tex;
in float a;
in vec3 n;
in vec3 pos;

vec3 LIGHTPOS = vec3(40.f,50.f,40.f);

void main(){

	
		float grad = 500 * dot(normalize(n),vec3(0,1,0));

		float pct = max(0.f,min(grad/4,1.0f));

	
		colour = mix(texture(theTexture2,tex), texture(theTexture,tex), pct);


		colour.a = a;
		
		float light = max(dot(normalize(LIGHTPOS - pos),n),0.2f);
		float dist = length(LIGHTPOS - pos);


		light *= max(1-(dist/8000.f),0);

		light = max(min(light,0.8f),0.2f);

		colour.x *= light;  
		colour.y *= light;
		colour.z *= light;
		
		

}