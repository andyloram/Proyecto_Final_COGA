#version 330 core
out vec4 FragColor;

struct LuzDireccional {
    vec3 direccion;
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
};
struct LuzSpot
{
    vec3 posicion;
    vec3 direccion;
    float cutOff;
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
    float constant;
    float linear;
    float quadratic;
    float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

uniform sampler2D textura_difusa1;
uniform sampler2D textura_especular1;
uniform sampler2D textura_normal1;

uniform float shininess;
uniform LuzDireccional sol;
uniform LuzSpot linterna;

uniform vec3 viewPos;

uniform bool mapaNormal;
uniform bool noche;

vec3 calcularSpot(LuzSpot linterna, vec3 normal, vec3 viewDir);
vec3 calcularLuzDireccional(LuzDireccional sol, vec3 normal, vec3 viewDir);
void main()
{
    vec3 resultado=vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = Normal;
    if(mapaNormal)
    {
    norm = texture(textura_normal1, TexCoords).rgb;
    norm = norm * 2.0 - 1.0; 
    norm=normalize(TBN * norm);
    }
   if(noche)
        resultado+=calcularSpot(linterna, norm, viewDir);
    else
       resultado+= calcularLuzDireccional(sol, norm, viewDir);

    FragColor = vec4(resultado,1.0);
} 
vec3 calcularLuzDireccional(LuzDireccional sol, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-sol.direccion);
    vec3 colorBase = texture(textura_difusa1, TexCoords).rgb;
    //Parte Ambiente
    vec3 ambient = sol.ambiente * colorBase;

    //Parte Difusa
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = sol.difusa *diff * colorBase;
    
    //Parte Especular
     vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = sol.especular * spec * texture(textura_especular1, TexCoords).rgb;  
    
    return (ambient+diffuse+specular);
}

vec3 calcularSpot(LuzSpot linterna, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(linterna.posicion - FragPos);
    float theta= dot(lightDir, normalize(-linterna.direccion));
    vec3 colorBase = texture(textura_difusa1, TexCoords).rgb;
   
   //Parte Ambiente
    vec3 ambient = linterna.ambiente * colorBase;

    //Parte Difusa
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = linterna.difusa *diff * colorBase;
    
    //Parte Especular
     vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = sol.especular * spec * texture(textura_especular1, TexCoords).rgb;  

    float distancia=length(linterna.posicion- FragPos);
    float epsilon=linterna.cutOff - linterna.outerCutOff;
    float intensity= clamp((theta-linterna.outerCutOff)/epsilon, 0.0, 1.0);
    float atenuacion= 1.0/(linterna.constant+linterna.linear*distancia+linterna.quadratic* distancia*distancia);
    diffuse*=intensity;
    specular*=intensity;
    diffuse*=atenuacion;
    specular*=atenuacion;
    return (ambient+diffuse+specular);


}