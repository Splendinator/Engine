#version 460 core

out vec4 colour;

uniform sampler2D theTexture;
uniform sampler2D texEmissive;

uniform vec2 pixelSize;


uniform int effect;

in vec2 tex;
in float a;
in vec3 n; 
in vec3 pos;




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


vec4 bloom(){
	vec4 t1;
	vec4 t2;
	vec4 t3;

	vec4 f1;
	vec4 f2;
	vec4 f3;

	t1 = texture(texEmissive,vec2(tex.x,tex.y + pixelSize.y));
	t2 = texture(texEmissive,vec2(tex.x,tex.y));
	t3 = texture(texEmissive,vec2(tex.x,tex.y - pixelSize.y));
	
	f1 = (t1 + t2 + t3) / 3;
	
	t1 = texture(texEmissive,vec2(tex.x + pixelSize.x,tex.y + pixelSize.y));
	t2 = texture(texEmissive,vec2(tex.x + pixelSize.x,tex.y));
	t3 = texture(texEmissive,vec2(tex.x + pixelSize.x,tex.y - pixelSize.y));
	
	f2 = (t1 + t2 + t3) / 3;

	t1 = texture(texEmissive,vec2(tex.x - pixelSize.x,tex.y + pixelSize.y));
	t2 = texture(texEmissive,vec2(tex.x - pixelSize.x,tex.y));
	t3 = texture(texEmissive,vec2(tex.x - pixelSize.x,tex.y - pixelSize.y));
	
	f3= (t1 + t2 + t3) / 3;
	
	vec4 bloomValue = (f1 + f2 + f3) / 3;
	
	//bloomValue = (bloomValue.x > 0.0f ? ():(texture(theTexture,tex));

	if(bloomValue.x > 0.f)
	return texture(theTexture,tex) +  bloomValue * (bloomValue.x*1.8f);

	return texture(theTexture,tex);
}


bool similar(vec4 a, vec4 b, float dif){
	float tot = 0;
	tot += abs(a.x - b.x);
	tot += abs(b.y - b.y);
	tot += abs(a.z - b.z);

	return (tot < dif);
}


vec4 detectEdge(){
	vec4 t1,t2,t3,t4;
	
	t1 = texture(theTexture,vec2(tex.x,tex.y));
	t2 = texture(theTexture,vec2(tex.x + pixelSize.x,tex.y));
	t3 = texture(theTexture,vec2(tex.x,tex.y + pixelSize.y));
	t4 = texture(theTexture,vec2(tex.x + pixelSize.x,tex.y + pixelSize.y));

	bool b = true;

	b = b && similar(t1,t2,0.13f);
	b = b && similar(t1,t3,0.13f);
	b = b && similar(t1,t4,0.13f);
	b = b && similar(t2,t3,0.13f);
	b = b && similar(t2,t4,0.13f);
	b = b && similar(t3,t4,0.13f);
	
	
	return (b?texture(theTexture,tex):vec4(0,0,0,1));
}









void main(){

	
	switch(effect){
		case(0):
			colour = texture(theTexture,tex);
			break;
	
		case(1):
			colour = gaucianBlurr();
			break;
	
		case(2):
			colour = bloom();
			break;

		case(3):
			colour = detectEdge();
			break;
	
	}

}


