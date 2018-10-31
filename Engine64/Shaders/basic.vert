#version 330 core

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 pos;

void main(){
	mat4 mvp = model * view * proj; 
	
	pos = vec4(position,1) * mvp;
	
	gl_Position = pos;
	
}