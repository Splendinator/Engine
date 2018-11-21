#version 460 core


in vec3 position;
in vec2 texCoords;
in float alpha;
in vec3 norms;

uniform vec2 pixelSize;

out vec2 tex;
out float a;
out vec3 n;
out vec3 pos;


void main(){

	n = norms;
	a  = alpha;

	tex = texCoords;
	pos = position;




	gl_Position =  vec4(position,1);
	
	
}