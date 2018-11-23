#version 460 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;    


layout (location = 10) uniform mat4 model;
layout (location = 11) uniform mat4 view;
layout (location = 12) uniform mat4 proj;


uniform float size; 


in Vertex {
	vec3 pos;
} IN [];

out vec2 tex;

void main(){

for(int i = 0; i < gl_in.length(); ++i) {

mat4 modl = model;
//modl[3][0] = IN[i].pos.x;
//modl[3][1] = IN[i].pos.y;
//modl[3][2] = IN[i].pos.z;

mat4 MV = modl * view;
mat4 VP = view * proj;

MV = transpose(MV);


vec3 right = vec3(MV[0][0], 
                    MV[1][0], 
                    MV[2][0]);

vec3 up = vec3(MV[0][1], 
                 MV[1][1], 
                 MV[2][1]);

				//right = vec3(0,1,0);
				 //up = vec3(1,0,0);
		


  vec3 va = IN[i].pos - (right + up) * size;
  gl_Position = vec4(va, 1.0) * VP;
  tex = vec2(0.0, 0.0);
  EmitVertex();  
  
  vec3 vb = IN[i].pos - (right - up) * size;
  gl_Position = vec4(vb, 1.0) * VP;
  tex = vec2(0.0, 1.0);
  EmitVertex();  

  vec3 vd = IN[i].pos + (right - up) * size;
  gl_Position = vec4(vd, 1.0) * VP;
  tex = vec2(1.0, 0.0);
  EmitVertex();  

  vec3 vc = IN[i].pos + (right + up) * size;
  gl_Position = vec4(vc, 1.0) * VP;
  tex = vec2(1.0, 1.0);
  EmitVertex();  

  
  EndPrimitive(); 

}

}