#version 430

in vec3 fragNormal;
in vec3 fragWorldPos;
in vec4 fragColor;
in float fragLife;

out vec4 finalColor;

uniform vec3 viewPos;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(vec3(0.3, 1.0, 0.5));
    float diff = max(dot(N, L), 0.3);

    vec3 V = normalize(viewPos - fragWorldPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0) * 0.3;

    vec3 color = fragColor.rgb * diff + vec3(spec);

    float alpha = fragLife;
    if (alpha < 0.01) discard;

    finalColor = vec4(color, alpha);
}
