#type vertex
#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 fTexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f); 
    fTexCoord = aTexCoord;
}


#type fragment
#version 460 core

out vec4 display;

in vec2 fTexCoord;

uniform sampler2D frameBufferTexture;

void main()
{
    //display = texture(frameBufferTexture, fTexCoord);
    display = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}