#version 460 core

out vec4 colour;


uniform samplerCube cubeTex;


in vec3 normal; 


void main(){

	
	colour = texture(cubeTex,normal);

}





