#version 330

uniform vec2 handPos;
uniform int isPinching;
uniform vec2 screenRes;
uniform float time;
uniform float opacity;

out vec4 finalColor;

void main() {
    vec2 uv = gl_FragCoord.xy / screenRes.xy;

    vec2 gridCount = vec2(60.0, 35.0);
    vec2 st = fract(gl_FragCoord.xy / (screenRes.xy / gridCount));

    vec2 vectorToHand = handPos - gl_FragCoord.xy;
    float distanceToHand = length(vectorToHand);

    float radius = 0.15;
    vec3 dotColor = vec3(0.05, 0.15, 0.35);

    if (isPinching == 1) {
        float gravityField = smoothstep(400.0, 0.0, distanceToHand);
        st += (vectorToHand / screenRes.xy) * gravityField * 0.5;
        dotColor = mix(dotColor, vec3(0.8, 0.2, 0.05), gravityField);
        radius = mix(0.15, 0.35, gravityField);
    } else {
        float wave = sin(distanceToHand * 0.05 - time * 5.0) * 0.5 + 0.5;
        float influence = smoothstep(200.0, 0.0, distanceToHand);
        radius += wave * influence * 0.15;
    }

    float distToCellCenter = distance(st, vec2(0.5));
    float alpha = smoothstep(radius, radius - 0.02, distToCellCenter) * opacity;

    finalColor = vec4(dotColor * alpha, alpha);
}
