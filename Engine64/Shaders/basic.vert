#version 460 core

in vec3 position;

uniform mat4 mvp;

out vec4 pos;

void main(){
	
	pos = vec4(position,1) * mvp;
	
	gl_Position = pos;
	
}