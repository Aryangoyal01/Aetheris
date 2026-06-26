#version 330

// Inputs from C++ (Uniforms)
uniform vec2 handPos;      // Live hand coordinates
uniform int isPinching;    // 1 if pinching, 0 if open
uniform vec2 screenRes;    // 1280x720 window dimensions
uniform float time;        // Continuous running time

// Output fragment color
out vec4 finalColor;

void main() {
    // Normalize current pixel coordinates from 0.0 to 1.0
    vec2 uv = gl_FragCoord.xy / screenRes.xy;
    
    // Set up a repeating grid pattern (creating thousands of visual "particles")
    vec2 gridCount = vec2(60.0, 35.0); // Adjust grid density
    vec2 st = fract(gl_FragCoord.xy / (screenRes.xy / gridCount));
    
    // Vector pointing from this pixel to the hand position
    vec2 vectorToHand = handPos - gl_FragCoord.xy;
    float distanceToHand = length(vectorToHand);
    
    // Base radius of our grid dots
    float radius = 0.15;
    vec3 dotColor = vec3(0.2, 0.4, 0.8); // Default calm blue particle
    
    if (isPinching == 1) {
        // --- GRAVITATIONAL WARPING ---
        // If pinching, pull the grid space mathematically toward the hand
        float gravityField = smoothstep(400.0, 0.0, distanceToHand);
        st += (vectorToHand / screenRes.xy) * gravityField * 0.5;
        
        // Make particles near the hand turn energetic orange/red
        dotColor = mix(dotColor, vec3(1.0, 0.3, 0.1), gravityField);
        radius = mix(0.15, 0.35, gravityField);
    } else {
        // --- DISTANT PULSE ---
        // If hand is open, gently expand the dots close to the hand like a radar wave
        float wave = sin(distanceToHand * 0.05 - time * 5.0) * 0.5 + 0.5;
        float influence = smoothstep(200.0, 0.0, distanceToHand);
        radius += wave * influence * 0.15;
    }

    // Render sharp circular mathematical shapes inside each grid tile
    float distToCellCenter = distance(st, vec2(0.5));
    float alpha = smoothstep(radius, radius - 0.02, distToCellCenter);

    // Output final glowing composite color
    finalColor = vec4(dotColor * alpha, alpha * 0.8);
}