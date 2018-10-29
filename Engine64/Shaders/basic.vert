#version 330 core

in vec3 position;

uniform mat4 transform;

void main(){
	gl_Position = vec4(position,1) * transform; 
	
}