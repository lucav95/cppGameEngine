#version 120
uniform sampler2D currentTexture;

void main() {
	vec2 coord = gl_TexCoord[0].xy;
	vec4 pixel_color = texture2D(currentTexture, coord);
	gl_FragColor = vec4(pixel_color[0], 0.5, pixel_color[2], pixel_color[3]);
}