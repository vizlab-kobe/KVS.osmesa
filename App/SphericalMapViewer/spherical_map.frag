#version 120
#include "texture.h"

uniform sampler2D spherical_map;

void main()
{
    vec2 index = gl_TexCoord[0].st;
    gl_FragColor = LookupTexture2D( spherical_map, index );
}
