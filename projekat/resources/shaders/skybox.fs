#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 TexCoords;
} fs_in;

uniform samplerCube skybox;

void main(){
    FragColor = texture(skybox, fs_in.TexCoords);
}