#version 460 core


in vec3 position;


layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;





out mat4 inverseProjView;

void main(){




	inverseProjView = inverse(view * proj);

	vec4 p = vec4(position,1);

	p *= model;
	p *= view;
	p *= proj;
	


	gl_Position = p;
	
}