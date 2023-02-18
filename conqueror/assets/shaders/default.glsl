#type vertex
#version 460 core
layout (location = 0) in vec2 aPos; // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; //the color of the vector
layout (location = 2) in vec2 aTexCoord; //the coords of the texture
layout (location = 3) in float aTexID; //The slot of the texture
layout (location = 4) in float aCoreID;

// camera variables
uniform mat4 uProjection;
uniform mat4 uView;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexID;
    float CoreID;
};

layout(location = 0) out VertexOutput Output;

void main()
{
    gl_Position = uProjection * uView * vec4(aPos, 0.0f, 1.0f); // adjust gl_Position with the help of 'u' Factors
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    Output.TexID = aTexID;
    Output.CoreID = aCoreID;
    // pipe variables from the vbo
}


#type fragment
#version 460 core

layout(location = 0) out vec4 display;
layout(location = 1) out int objectID;



struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexID;
    float CoreID
};

layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D uTexture[32];

void main()
{
    // if there is a desired texture, load it
    if (int(Input.TexID) >= 0) {
        display = texture(uTexture[int(Input.TexID)], Input.TexCoord);
    }
    else {
        // if there is no texture, display the colors
        display = Input.Color;
    }
    objectID = int(fCoreID);
}