#version 460 core


in vec3 position;
in vec2 texCoords;


out vec2 tex;



void main(){

	tex = texCoords;
	vec3 pos = position;
	gl_Position =  vec4(pos,1);
	
	
	
}