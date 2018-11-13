#version 460 core

out vec4 colour;

uniform sampler2D theTexture;

in vec2 tex;
in float a;
in vec3 n;
in vec3 pos;


void main(){

		
		colour = texture(theTexture,vec2(tex.x+(0.1f*tex.y),tex.y));
		
		

}