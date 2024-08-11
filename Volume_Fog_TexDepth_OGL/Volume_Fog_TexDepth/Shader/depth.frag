#version 330

out float FragColor;

in float TexDepth;

void main()
{
    gl_FragDepth = TexDepth;
}