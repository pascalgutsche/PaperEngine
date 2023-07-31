#type vertex
#version 460 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec4 aColor; //the color of the vector
layout(location = 2) in vec2 aTexCoord; //the coords of the texture
layout(location = 3) in float aTilingFactor;
layout(location = 4) in int aTexID; //The slot of the texture
layout(location = 5) in int aCoreID;
layout(location = 6) in int aUIID;
layout(location = 7) in int aAlphaCoreID;

// camera variables
layout(std140, binding = 0) uniform Camera
{
    mat4 uProjection;
    mat4 uView;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat int TexID;
layout(location = 4) out flat int CoreID;
layout(location = 5) out flat int alphaCoreID;

void main()
{
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    Output.TilingFactor = aTilingFactor;
    TexID = aTexID;
    CoreID = aCoreID;
    alphaCoreID = aAlphaCoreID;

    gl_Position = uProjection * uView * vec4(aPos, 1.0f);
}


#type fragment
#version 460 core

layout(location = 0) out vec4 display;
layout(location = 1) out int objectID;



struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat int TexID;
layout(location = 4) in flat int CoreID;
layout(location = 5) in flat int alphaCoreID;


uniform sampler2D uTexture[32];

void main()
{
    vec4 color = Input.Color;
    if (TexID >= 0)
        color *= texture(uTexture[TexID], Input.TexCoord * Input.TilingFactor);

    if (color.a == 0.0 && alphaCoreID == 0)
        discard;

    display = color;
    objectID = CoreID;
}