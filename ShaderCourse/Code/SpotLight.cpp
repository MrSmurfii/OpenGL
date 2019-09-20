#include "SpotLight.h"
#include "Material.h"

void SpotLight::UploadToMaterial(const Material& targetMaterial)
{
	targetMaterial.Set("u_SpotLight.position", position);
	targetMaterial.Set("u_SpotLight.direction", direction);
	targetMaterial.Set("u_SpotLight.color", color);
	targetMaterial.Set("u_SpotLight.radius", radius);
	targetMaterial.Set("u_SpotLight.angle", angle);
}
