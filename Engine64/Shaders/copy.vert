#version 460 core


in vec3 position;
in vec2 texCoords;


uniform vec2 pixelSize;

out vec2 tex;



void main(){

	tex = texCoords;
	gl_Position =  vec4(position,1);
	
	
}