#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out mat3 TBN;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat2 textureTrans;
uniform bool dibujarSiempre;


void main()
{
    //Posicion de los fragmentos
    FragPos = vec3(model * vec4(aPos, 1.0));   

    //Coordenadas de la textura
    TexCoords = textureTrans * aTexCoords;
    
    //Si utilizamos algun scale no uniforme
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    normalMatrix = mat3(model);

    //Calculamos los vectores de la base tangente para el mapa normal
    vec3 N = normalize(normalMatrix* aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);

    Normal = N;

    //Arreglo para coordenadas simetricas
    if(dot(cross(N,T),B)<0.0)
        T=T*-1.0;
    T= normalize(T-dot(T,N)*N);
    B= cross(N,T);
    TBN= mat3(T, B, N);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}