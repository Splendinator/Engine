#version 460 core

out vec4 colour;

layout (location = 11) uniform sampler2D theTexture;

in vec2 tex;
in float a;
in vec3 n;

vec3 LIGHTPOS = vec3(10,5,20);

void main(){

		colour = texture(theTexture,tex);
		colour.a = a;

		float light = max(dot(vec3(0,1,0),n),0.2f);
					  
		colour.x *= light;  
		colour.y *= light;
		colour.z *= light;
		
		

}