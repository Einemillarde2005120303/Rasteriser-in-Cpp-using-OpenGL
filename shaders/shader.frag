#version 460 core

in vec2 v_TexCoord;

uniform sampler2D s_Texture;

void main()
{
  gl_FragColor = texture(s_Texture, v_TexCoord);
}