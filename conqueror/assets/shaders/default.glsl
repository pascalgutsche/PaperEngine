#type vertex
#version 460 core
layout (location = 0) in vec2 aPos; // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; //the color of the vector
layout (location = 2) in vec2 aTexCoord; //the coords of the texture
layout (location = 3) in float aTexID; //The slot of the texture

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
    gl_Position = uProjection * uView * vec4(aPos, 0.0f, 1.0f); // adjust gl_Position with the help of 'u' Factors
    Output.Color = aColor;
    Output.TexCoord = aTexCoord;
    Output.TexID = aTexID;

    // pipe variables from the vbo
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

layout(binding = 0) uniform sampler2D uTexture[32];

void main()
{
    vec2 uvDx = dFdx(Input.TexCoord);
    vec2 uvDy = dFdy(Input.TexCoord);
    vec4 color = Input.Color;
    switch (int(Input.TexID)) {
        case -1:; break;
        case 0 : color *= textureGrad(uTexture[0 ], Input.TexCoord, uvDx, uvDy); break;
        case 1 : color *= textureGrad(uTexture[1 ], Input.TexCoord, uvDx, uvDy); break;
        case 2 : color *= textureGrad(uTexture[2 ], Input.TexCoord, uvDx, uvDy); break;
        case 3 : color *= textureGrad(uTexture[3 ], Input.TexCoord, uvDx, uvDy); break;
        case 4 : color *= textureGrad(uTexture[4 ], Input.TexCoord, uvDx, uvDy); break;
        case 5 : color *= textureGrad(uTexture[5 ], Input.TexCoord, uvDx, uvDy); break;
        case 6 : color *= textureGrad(uTexture[6 ], Input.TexCoord, uvDx, uvDy); break;
        case 7 : color *= textureGrad(uTexture[7 ], Input.TexCoord, uvDx, uvDy); break;
        case 8 : color *= textureGrad(uTexture[8 ], Input.TexCoord, uvDx, uvDy); break;
        case 9 : color *= textureGrad(uTexture[9 ], Input.TexCoord, uvDx, uvDy); break;
        case 10: color *= textureGrad(uTexture[10], Input.TexCoord, uvDx, uvDy); break;
        case 11: color *= textureGrad(uTexture[11], Input.TexCoord, uvDx, uvDy); break;
        case 12: color *= textureGrad(uTexture[12], Input.TexCoord, uvDx, uvDy); break;
        case 13: color *= textureGrad(uTexture[13], Input.TexCoord, uvDx, uvDy); break;
        case 14: color *= textureGrad(uTexture[14], Input.TexCoord, uvDx, uvDy); break;
        case 15: color *= textureGrad(uTexture[15], Input.TexCoord, uvDx, uvDy); break;
        case 16: color *= textureGrad(uTexture[16], Input.TexCoord, uvDx, uvDy); break;
        case 17: color *= textureGrad(uTexture[17], Input.TexCoord, uvDx, uvDy); break;
        case 18: color *= textureGrad(uTexture[18], Input.TexCoord, uvDx, uvDy); break;
        case 19: color *= textureGrad(uTexture[19], Input.TexCoord, uvDx, uvDy); break;
        case 20: color *= textureGrad(uTexture[20], Input.TexCoord, uvDx, uvDy); break;
        case 21: color *= textureGrad(uTexture[21], Input.TexCoord, uvDx, uvDy); break;
        case 22: color *= textureGrad(uTexture[22], Input.TexCoord, uvDx, uvDy); break;
        case 23: color *= textureGrad(uTexture[23], Input.TexCoord, uvDx, uvDy); break;
        case 24: color *= textureGrad(uTexture[24], Input.TexCoord, uvDx, uvDy); break;
        case 25: color *= textureGrad(uTexture[25], Input.TexCoord, uvDx, uvDy); break;
        case 26: color *= textureGrad(uTexture[26], Input.TexCoord, uvDx, uvDy); break;
        case 27: color *= textureGrad(uTexture[27], Input.TexCoord, uvDx, uvDy); break;
        case 28: color *= textureGrad(uTexture[28], Input.TexCoord, uvDx, uvDy); break;
        case 29: color *= textureGrad(uTexture[29], Input.TexCoord, uvDx, uvDy); break;
        case 30: color *= textureGrad(uTexture[30], Input.TexCoord, uvDx, uvDy); break;
        case 31: color *= textureGrad(uTexture[31], Input.TexCoord, uvDx, uvDy); break;
    }
    display = color;
}