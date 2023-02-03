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
    vec4 color;// = Input.Color;
    switch (int(Input.TexID)) {
    case 0 : color = texture(uTexture[0 ], Input.TexCoord); break;
    case 1 : color = texture(uTexture[1 ], Input.TexCoord); break;
    case 2 : color = texture(uTexture[2 ], Input.TexCoord); break;
    case 3 : color = texture(uTexture[3 ], Input.TexCoord); break;
    case 4 : color = texture(uTexture[4 ], Input.TexCoord); break;
    case 5 : color = texture(uTexture[5 ], Input.TexCoord); break;
    case 6 : color = texture(uTexture[6 ], Input.TexCoord); break;
    case 7 : color = texture(uTexture[7 ], Input.TexCoord); break;
    case 8 : color = texture(uTexture[8 ], Input.TexCoord); break;
    case 9 : color = texture(uTexture[9 ], Input.TexCoord); break;
    case 10: color = texture(uTexture[10], Input.TexCoord); break;
    case 11: color = texture(uTexture[11], Input.TexCoord); break;
    case 12: color = texture(uTexture[12], Input.TexCoord); break;
    case 13: color = texture(uTexture[13], Input.TexCoord); break;
    case 14: color = texture(uTexture[14], Input.TexCoord); break;
    case 15: color = texture(uTexture[15], Input.TexCoord); break;
    case 16: color = texture(uTexture[16], Input.TexCoord); break;
    case 17: color = texture(uTexture[17], Input.TexCoord); break;
    case 18: color = texture(uTexture[18], Input.TexCoord); break;
    case 19: color = texture(uTexture[19], Input.TexCoord); break;
    case 20: color = texture(uTexture[20], Input.TexCoord); break;
    case 21: color = texture(uTexture[21], Input.TexCoord); break;
    case 22: color = texture(uTexture[22], Input.TexCoord); break;
    case 23: color = texture(uTexture[23], Input.TexCoord); break;
    case 24: color = texture(uTexture[24], Input.TexCoord); break;
    case 25: color = texture(uTexture[25], Input.TexCoord); break;
    case 26: color = texture(uTexture[26], Input.TexCoord); break;
    case 27: color = texture(uTexture[27], Input.TexCoord); break;
    case 28: color = texture(uTexture[28], Input.TexCoord); break;
    case 29: color = texture(uTexture[29], Input.TexCoord); break;
    case 30: color = texture(uTexture[30], Input.TexCoord); break;
    case 31: color = texture(uTexture[31], Input.TexCoord); break;

    }
    display = color;
}