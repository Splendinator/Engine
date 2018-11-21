#version 460 core


in vec3 position;
in vec2 texCoords;
in float alpha;
in vec3 norms;
in vec3 tangents;

layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;

out vec2 tex;
out float a;
out vec3 normal;
out vec3 pos;
out vec3 tangent;
out vec3 binormal;


void main(){

	a  = alpha;


	mat3 normalMatrix = transpose ( inverse ( mat3 ( model )));
	normal = normalize ( normalMatrix * normalize ( norms ));


	tex = texCoords;
	tangent = normalize(normalMatrix * normalize(tangents));
	binormal = normalize(normalMatrix * normalize(cross(norms,tangent)));



	vec4 p = vec4(position,1);
	p *= model;
	pos = vec3(p.x,p.y,p.z);
	p *= view * proj;
	
	gl_Position = p;

}