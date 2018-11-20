#version 460 core


in vec3 position;
in vec2 texCoords;
in float alpha;
in vec3 norms;

layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;
uniform mat4 shadowMat;

out vec2 tex;
out float a;
out vec3 n;
out vec3 pos;
out vec4 shadowPos;


void main(){

	n = norms;
	a  = alpha;

	tex = texCoords;

	mat3 normalMatrix = transpose ( inverse ( mat3 ( model )));
	n = normalize ( normalMatrix * normalize ( norms ));


	vec4 p = vec4(position ,1);
	p *= model;
	pos = vec3(p.x,p.y,p.z);
	p *= view * proj;





	vec4 shadowP = vec4(position + n * 0.1f,1);

	shadowP *= model;
	shadowP *= shadowMat;


	shadowPos = shadowP;
	

	gl_Position = p;// * model * view * proj;
	
	
}