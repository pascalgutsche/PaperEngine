#type vertex
#version 460 core
layout(location = 0) in vec2 aPos; // the position variable has attribute position 0
layout(location = 1) in vec4 aColor; //the color of the vector
layout(location = 2) in vec2 aTexCoord; //the coords of the texture
layout(location = 3) in float aTilingFactor;
layout(location = 4) in int aTexID; //The slot of the texture
layout(location = 5) in int aCoreID;

// camera variables
uniform mat4 uProjection;
uniform mat4 uView;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};
 
layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat int TexID;
layout(location = 4) out flat int CoreID;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    Output.TilingFactor = aTilingFactor;
    TexID = aTexID;
    CoreID = aCoreID;

    gl_Position = uProjection * uView * vec4(aPos, 0.0f, 1.0f); // adjust gl_Position with the help of 'u' Factors
}


#type fragment
#version 460 core

layout(location = 0) out vec4 display;
//layout(location = 1) out int objectID;



struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat int TexID;
layout(location = 4) in flat int CoreID;

layout(binding = 0) uniform sampler2D uTexture[32];

void main()
{
    if (TexID >= 0) {
        display = texture(uTexture[TexID], Input.TexCoord * Input.TilingFactor);  
    }
    else {
        display = Input.Color;
    }
    //objectID = CoreID;
}