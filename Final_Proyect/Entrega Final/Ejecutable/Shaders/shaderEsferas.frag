#version 330 core
out vec4 FragColor;
uniform float color;

void main()
{    
    if(color==0.0)
        FragColor = vec4(1.0,0.0,0.0,1.0);
    else if(color==1.0)
        FragColor = vec4(0.0,0.0,1.0,1.0);
    else
        FragColor = vec4(1.0,1.0,1.0,1.0);
    

}