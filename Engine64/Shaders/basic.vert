#version 330

in vec3 position;

uniform mat4 transform;

void main(){
	gl_Position = vec4(position.x,position.y*transform[1][1],position.z,1.0f);
	
}