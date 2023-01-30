#type vertex
#version 460 core
layout (location = 0) in vec2 aPos; // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; //the color of the vector
layout (location = 2) in vec2 aTexCoord; //the coords of the texture
layout (location = 3) in float aTexID; //The slot of the texture

// declare vertex variables that are being piped to fragment
out vec4 fColor;
out vec2 fTexCoord;
out float fTexID;

// camera variables
uniform mat4 uProjection;
uniform mat4 uView;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexID;
};

layout(location = 0) out VertexOutput Output;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    Output.TexID = aTexID;

    // pipe variables from the vbo
    gl_Position = uProjection * uView * vec4(aPos, 0.0f, 1.0f); // adjust gl_Position with the help of 'u' Factors
}


#type fragment
#version 460 core

layout(location = 0) out vec4 display;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexID;
};

layout(location = 0) in VertexOutput Input;

uniform sampler2D uTexture[32];

void main()
{
    vec4 color = Input.Color;
    if (int(Input.TexID) > -1) {
        color *= texture(uTexture[int(Input.TexID)], Input.TexCoord);
    }
    display = color;
}