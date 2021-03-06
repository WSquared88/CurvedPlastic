#version 430
uniform vec4 color = vec4(1,1,1,1);
uniform sampler2D uniformTex;
in vec2 fragTexUV;
in vec3 fragPos;
in vec3 fragNorm;

void main()
{
	vec4 color = texture(uniformTex, fragTexUV);
	vec3 lightDir = vec3(0,1,0);
	float lamb = max(dot(lightDir, fragNorm),0);
	vec3 viewDir = normalize(-fragPos);
	vec3 halfDir = normalize(lightDir+viewDir);
	float specAngle = max(dot(halfDir, fragNorm), 0);
	float specular = pow(specAngle, 16.0);
	float light = .1 + .6 * lamb + .3 * specular;
	gl_FragColor = vec4(color.rgb * light, 1);
}