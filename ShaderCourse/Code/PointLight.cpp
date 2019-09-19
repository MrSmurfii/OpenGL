#include "PointLight.h"
#include "Material.h"
#include <stdio.h>

void PointLight::UploadToMaterial(int index, const Material& targetMaterial){

	static char stringBuffer[100];

	sprintf_s(stringBuffer, 100, "u_PointLights[%d].position", index);
	targetMaterial.Set(stringBuffer, position);

	sprintf_s(stringBuffer, 100, "u_PointLights[%d].color", index);
	targetMaterial.Set(stringBuffer, color);

	sprintf_s(stringBuffer, 100, "u_PointLights[%d].radius", index);
	targetMaterial.Set(stringBuffer, radius);
}