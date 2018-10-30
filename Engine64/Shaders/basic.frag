#version 330 core


out vec4 colour;

void main(){
	colour = ((int(gl_FragCoord.x) % 16 < 8) != ((int(gl_FragCoord.y)) % 16 < 8) ? vec4(gl_FragCoord.x/600,gl_FragCoord.y/600,0,1) : vec4(0.1,0.1,0.1,1));
}