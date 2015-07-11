// position only

uniform mat4 pvm; // projection view model

attribute vec4 v_pos;

void main()
{
    gl_Position = pvm * v_pos;
}
