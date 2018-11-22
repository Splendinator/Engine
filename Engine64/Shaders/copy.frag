#version 460 core

out vec4 colour;

uniform sampler2D theTexture;

in vec2 tex;


void main(){

	colour = texture(theTexture,tex);
		

}
