#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D texture_wall;
uniform int colorMode;

void main() {
	if (colorMode == 1) {
		FragColor = texture(texture_wall, texCoord);
	} else if (colorMode == 2) {
		FragColor = vec4(1.0, 0.2, 0.5, 1.0);
	} else {
		FragColor = vec4(0.2, 1.0, 0.3, 1.0);
	}
}
