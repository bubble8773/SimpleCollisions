#version 330

layout (location = 0) in vec4 vposition;
layout (location = 1) in vec4 vcolor;

out vec4 color;
void main()
{   
	gl_Position = vposition;
	color = vcolor;
}
