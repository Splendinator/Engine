#version 460 core


in vec3 position;
in vec2 texCoords;
in float alpha;
in vec3 norms;

layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;

out vec2 tex;
out float a;
out vec3 n;
out vec3 pos;


void main(){

	n = norms;
	a  = alpha;

	tex = texCoords;

	vec4 p = vec4(position,1);
	p *= model;
	pos = vec3(p.x,p.y,p.z);
	p *= view;
	p *= proj;
	
	tex.y = 1 - tex.y;

	gl_Position = p;// * model * view * proj;
	
	
}