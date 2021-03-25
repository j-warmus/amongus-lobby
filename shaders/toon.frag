#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalOutput;
in vec3 fragPos;

// light information

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightAtten;

// material information
uniform vec3 color;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

// viewposition
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{   
// If phong mode, calc diffuse component
    vec3 lightVec = normalize(lightPos-fragPos);
    vec3 diff_comp = lightColor * diffuse * max(dot(normalize(normalOutput),lightVec), 0.0);
// calculate specular component
    vec3 viewVec = normalize(viewPos - fragPos);
    vec3 reflectVec = reflect(-lightVec, normalize(normalOutput));
    vec3 spec_comp = specular * pow(max(dot(reflectVec, viewVec), 0.0), shininess);

// attenuate.  Only linear right now
    float light_dist = length(lightPos - fragPos);
    float attenuation = 1/(lightAtten * light_dist);

// toon it up
    float intensity = length(spec_comp + diff_comp)/2;
    vec3 tcolor = color;
    if (intensity > 0.95){
        tcolor = 1.0 * color;
    } else if (intensity > 0.5){
        tcolor = 0.7 * color;
    } else if (intensity > 0.05){
        tcolor = 0.35 * color;
    } else {
        tcolor = 0.1 * color;
    }
		
    float edge = max(0, dot(normalize(normalOutput),viewVec));
    if (edge < 0.5){
        tcolor = vec3(0.0,0.0,0.0);
    }

// add it up
    // vec3 phong = attenuation * color * (ambient + diff_comp + spec_comp);

    fragColor = vec4(tcolor, 1.0f);

}