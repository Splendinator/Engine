#version 460 core

out vec4 colour;

uniform sampler2D theTexture;
uniform vec2 pixelSize;

in vec2 tex;
in float a;
in vec3 n; 
in vec3 pos;


void main(){

	colour = texture(theTexture,tex);
		

}


vec4 gaucianBlurr(){
	vec4 t1;
	vec4 t2;
	vec4 t3;

	vec4 f1;
	vec4 f2;
	vec4 f3;

	t1 = texture(theTexture,vec2(tex.x,tex.y + pixelSize.y));
	t2 = texture(theTexture,vec2(tex.x,tex.y));
	t3 = texture(theTexture,vec2(tex.x,tex.y - pixelSize.y));
	
	f1 = (t1 + t2 + t3) / 3;
	
	t1 = texture(theTexture,vec2(tex.x + pixelSize.x,tex.y + pixelSize.y));
	t2 = texture(theTexture,vec2(tex.x + pixelSize.x,tex.y));
	t3 = texture(theTexture,vec2(tex.x + pixelSize.x,tex.y - pixelSize.y));
	
	f2 = (t1 + t2 + t3) / 3;

	t1 = texture(theTexture,vec2(tex.x - pixelSize.x,tex.y + pixelSize.y));
	t2 = texture(theTexture,vec2(tex.x - pixelSize.x,tex.y));
	t3 = texture(theTexture,vec2(tex.x - pixelSize.x,tex.y - pixelSize.y));
	
	f3= (t1 + t2 + t3) / 3;
	
	
	
	
	return (f1 + f2 + f3) / 3;
}