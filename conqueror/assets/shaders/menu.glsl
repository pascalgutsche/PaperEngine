#type vertex
#version 330 core

layout (location = 0) in vec2 aPos; // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; //the color of the vector
layout (location = 2) in vec2 aTexCoord; //the coords of the texture
layout (location = 3) in float aTexID; //The slot of the texture

// declare vertex variables that are being piped to fragment
out vec4 fColor;
out vec2 fTexCoord;
out float fTexID;

void main()
{
    // the least 1 needs an explanation
    gl_Position = vec4(aPos, 0, 1); // adjust the gl_Position with the help of 'u' Factors
    fColor = aColor; // set the output variable to a dark-red color
    fTexCoord = aTexCoord; // set texCoord from the array within the code
    fTexID = aTexID; // pipe texID to fragment
}

#type fragment
#version 330 core

in vec4 fColor; // get color from vertex
in vec2 fTexCoord; // get texCoord from vertex
in float fTexID; // get texID from vertex

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;


out vec4 display;

void main()
{
    // if there is a desired texture, load it
    if (fTexID >= 0) {
        switch(int(fTexID)) {
            case 0:
                display = texture(uTexture0, fTexCoord);
                break;
            case 1:
                display = texture(uTexture1, fTexCoord);
                break;
            case 2:
                display = texture(uTexture2, fTexCoord);
                break;
            case 3:
                display = texture(uTexture3, fTexCoord);
                break;
            case 4:
                display = texture(uTexture4, fTexCoord);
                break;
            case 5:
                display = texture(uTexture5, fTexCoord);
                break;
            case 6:
                display = texture(uTexture6, fTexCoord);
                break;
            case 7:
                display = texture(uTexture7, fTexCoord);
                break;
            default:
            break;
        }

    }
    else {
        // if there is no texture, display the colors
        display = fColor;
    }
}