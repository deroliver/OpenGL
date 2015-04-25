#version 150

uniform mat4 ProjMat;

uniform vec4 MoveVec;

in vec4 vPosition;


void main()
{
	gl_Position = ProjMat * vPosition + MoveVec;

}