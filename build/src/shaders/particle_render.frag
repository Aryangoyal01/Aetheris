#version 430

in vec3 fragNormal;
in vec3 fragWorldPos;
in vec4 fragColor;

out vec4 finalColor;

uniform vec3 viewPos;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(vec3(0.3, 1.0, 0.5));
    float diff = max(dot(N, L), 0.3);

    finalColor = vec4(fragColor.rgb * diff, 1.0);
}
