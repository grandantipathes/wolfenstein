#version 440

//MATRICES
uniform mat4 mvp;
uniform mat4 camera;
uniform mat4 world;
uniform mat3 nmat;

//BUFFERS
in vec3 vertices;
in vec3 normals;
in vec2 uvs;

//OUTPUTS
out vec3 out_diffuse;
out vec3 out_reflect;
out vec2 out_uvs;

void main()
{
    vec4 p_cam = (camera * world *vec4(vertices,1.0));
    vec3 N = normalize(nmat*normals);
    vec4 lightpos = camera *vec4(4.0,4.0,3.0,1.0);
    vec3 L = (normalize((lightpos-p_cam))).xyz;
    vec3 light = vec3(1.0,1.0,1.0);
    vec3 I = normalize(p_cam).xyz;
    vec3 R = normalize(L+I);
    float Kr = max(dot(N,R),0.0);
    vec3 refl_col = vec3(1.0);
    
    out_reflect = vec3(pow(Kr,0.1) * refl_col);
    out_diffuse = light* max(dot(L,N),0.0);
    out_uvs = uvs;
    
    gl_Position = mvp * vec4(vertices, 1.0);
    
}
