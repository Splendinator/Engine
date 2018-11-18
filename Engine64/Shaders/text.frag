#version 460 core

out vec4 colour;

uniform sampler2D theTexture;

in vec2 tex;
in float a;
in vec3 n;
in vec3 pos;



void main(){

		colour = texture(theTexture,tex);
		colour.a = colour.r;
		//if (colour.r > 0) colour = vec4(1,1,1,1);
		
}