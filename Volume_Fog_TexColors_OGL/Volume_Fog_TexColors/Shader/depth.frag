#version 330

out float FragColor;

in float TexDepth;

void main()
{
    FragColor = TexDepth;
}