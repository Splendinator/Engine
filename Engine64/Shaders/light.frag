#version 460 core


uniform sampler2D texDepth;
uniform sampler2D texNorms;

uniform vec2 pixelSize;
uniform vec3 cameraPos;

uniform float lightRadius;
uniform vec3 lightPos;
uniform vec4 lightColour;

in mat4 inverseProjView;


out vec4 colour[2];


void main(){


vec3 pos = vec3 ((gl_FragCoord.x * pixelSize.x) ,
(gl_FragCoord.y * pixelSize.y) , 0.0f);

pos.z = texture(texDepth,pos.xy).r;

vec3 normal = normalize(texture(texNorms, pos.xy).xyz * 2.0f - 1.0f);

vec4 clip = transpose(inverseProjView) * vec4(pos*2.0f - 1.0f, 1.0f);
pos = clip.xyz / clip.w ;

float dist = length(lightPos - pos);
float atten = 1.0f - clamp(dist/lightRadius, 0.0f, 1.0f);

if( atten == 0.0) {
discard;
}

vec3 incident = normalize (lightPos - pos);
vec3 viewDir = normalize (cameraPos - pos);
vec3 halfDir = normalize (incident + viewDir);

float lambert = clamp(dot(incident , normal)/3.f ,0.0 ,1.0);
float rFactor = clamp(dot(halfDir , normal) ,0.0 ,1.0);
float sFactor = pow(rFactor,33.0);

colour[0] = vec4(lightColour.xyz * lambert * atten * lightColour.a , 1.0f);
colour[1] = vec4(lightColour.xyz * sFactor * atten * 0.33f * lightColour.a ,1.0f);


}