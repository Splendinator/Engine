#version 460 core


in vec3 position;
in vec2 texCoords;

layout (location = 10) uniform mat4 mvp;

out vec2 tex;


void main(){
	
	tex = texCoords;

	gl_Position = vec4(position,1) * mvp;
	
	
}