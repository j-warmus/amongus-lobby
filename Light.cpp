#include "Light.h"

Light::Light(glm::vec3 col, glm::vec3 pos, float atten)
{
	color = col;
	position = pos;
	attenuation = atten;
}
