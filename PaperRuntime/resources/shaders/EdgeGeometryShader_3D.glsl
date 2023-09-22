#type vertex
#version 460 core
layout(location = 0) in vec3 aPos; // the position variable has attribute position 0
layout(location = 1) in vec4 aColor; //the color of the vector
layout(location = 2) in int aIsLightSource;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec2 aTexCoord; //the coords of the texture
layout(location = 5) in float aTilingFactor;
layout(location = 6) in int aTexID; //The slot of the texture
layout(location = 7) in int aCoreID;

// camera variables
layout(std140, binding = 0) uniform Camera
{
    mat4 uProjection;
    mat4 uView;
};

struct VertexOutput
{
    vec4 Color;
    int IsLightSource;
    vec3 Normal;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 5) out flat int TexID;
layout(location = 6) out flat int CoreID;

void main()
{
    Output.Color = aColor;
    Output.IsLightSource = aIsLightSource;
    Output.Normal = aNormal;
    Output.TexCoord = aTexCoord;
    Output.TilingFactor = aTilingFactor;
    TexID = aTexID;
    CoreID = aCoreID;

    gl_Position = uProjection * uView * vec4(aPos, 1.0f);
}


#type fragment
#version 460 core

layout(location = 0) out vec4 display;
layout(location = 1) out int objectID;



struct VertexOutput
{
    vec4 Color;
    int IsLightSource;
    vec3 Normal;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) in flat VertexOutput Input;
layout(location = 5) in flat int TexID;
layout(location = 6) in flat int CoreID;

uniform sampler2D uTexture[32];
uniform vec4 uLightColor;

struct Light 
{
    vec3 position;
};

layout(std430, binding = 1) buffer LightBuffer
{
    uint lightsLength;
    Light lights[];
};

void main()
{
    for (int i = 0; i < lightsLength; ++i)
    {
        Light currentLight = lights[i];
    }

    vec4 color = Input.Color;
    if (TexID >= 0)
        color *= texture(uTexture[TexID], Input.TexCoord * Input.TilingFactor);

    if (color.a == 0.0)
        discard;


    vec4 ambientStrength = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    vec4 ambient = ambientStrength * uLightColor;

    if (Input.IsLightSource == 1)
        ambient = vec4(1.0f);

    display = ambient * color;
    objectID = CoreID;


}