#type vertex
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in int aCoreID;
layout(location = 4) in int aUIID;
layout(location = 5) in int aAlphaCoreID;

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
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat int coreID;
layout (location = 3) out flat int alphaCoreID;


void main()
{
	Output.Color = aColor;
	Output.TexCoord = aTexCoord;
	coreID = aCoreID;
    alphaCoreID = aAlphaCoreID;
    

	gl_Position = uProjection * uView * vec4(aPos, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 display;
layout(location = 1) out int oCoreID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 2) in flat int coreID;
layout (location = 3) in flat int alphaCoreID;


layout (binding = 0) uniform sampler2D u_FontAtlas;

float screenPxRange() {
	const float pxRange = 2.0; // set to distance field's pixel range
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_FontAtlas, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(Input.TexCoord);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

//uniform sampler2D uIDAttachment;

void main()
{
	vec4 texColor = Input.Color * texture(u_FontAtlas, Input.TexCoord);

	vec3 msd = texture(u_FontAtlas, Input.TexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
    display = mix(bgColor, Input.Color, opacity);
	if (display.a == 0.0 && alphaCoreID == 0)
		discard;

	oCoreID = coreID;
}