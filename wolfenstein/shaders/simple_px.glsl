#version 440
in vec3 out_diffuse;
in vec3 out_reflect;
in vec2 out_uvs;

uniform sampler2D texture1;
uniform sampler2D texture2;
out vec4 frag_color;

void main()
{
    vec3 ambient = vec3(0.01);
    vec3 one = (texture(texture1,out_uvs)).rgb; 
    vec3 two = (texture(texture2,out_uvs)).rgb;
    vec3 three = mix(one,two,0.5);
    vec3 lighting = (out_diffuse + ambient + out_reflect) * three;
    frag_color = vec4(lighting, 1.0);
}
