#version 400

in vec4 Frag_Color;

layout(location=0) out vec4 Out_Color;

void main(void)
{
	Out_Color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}