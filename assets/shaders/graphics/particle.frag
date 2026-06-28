#version 430

in vec2 vLocalPos;
in vec4 vColor;

out vec4 fragColor;

void main()
{
    // The quad goes from -1.0 to 1.0. A circle fits perfectly with radius 1.0.
    if (length(vLocalPos) > 1.0)
    {
        discard;
    }

    fragColor = vColor;
}
