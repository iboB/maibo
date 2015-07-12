#version 100
precision mediump float;
// simple colorize shader

uniform vec4 color;

void main()
{
    gl_FragColor = color;
}
