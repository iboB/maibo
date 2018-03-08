#version 100
// position only

uniform mat4 projView; // projection view
uniform mat4 model; // model

attribute vec4 v_pos;

void main()
{
    gl_Position = projView * model * v_pos;
}
