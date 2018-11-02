#version 460 core

uniform mat4 proj;

out vec4 colour;

in vec4 pos;

void main(){
	//colour = ((int(gl_FragCoord.x) % 16 < 8) != ((int(gl_FragCoord.y)) % 16 < 8) ? vec4(gl_FragCoord.x/600,gl_FragCoord.y/600,0,1) : vec4(0.1,0.1,0.1,1));
	colour = vec4(0,1,0,1);
	colour *= 1 - ((pos.z - 10.f)/15);
}