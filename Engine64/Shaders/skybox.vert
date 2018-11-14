#version 460 core


in vec3 position;


out vec3 normal;


layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;



void main(){

	vec3 multiply = vec3 (0 ,0 ,0);

	multiply . x = 1.0f / proj[0][0];	
	multiply . y = 1.0f / proj[1][1];

	vec3 tempPos = (position * multiply)- vec3(0, 0, 1);
	normal = mat3(view) * normalize(tempPos);
	gl_Position = vec4 (position, 1.0);



	
	
}