#version 460 core

out vec4 colour;

layout (location = 11) uniform sampler2D theTexture;

in vec2 tex;
in float a;

void main(){

		colour = texture(theTexture,tex);
		colour.a = a;

}