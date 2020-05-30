#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Config.h"
#include "Random.h"
#include "Texture.h"
#include "Random.h"
#include "BSDF.h"

namespace prim {

enum TextureType {
	COLOR_TEXTURE,
	NORMAL_TEXTURE,
	BUMP_TEXTURE,
	ROUGHNESS_TEXTURE,
	EMISSION_TEXTURE,
	NB_TEXTURES_TYPE
};

enum BSDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_GLOSSY = 1 << 2,
	BSDF_SPECULAR = 1 << 3,
	BSDF_DIFFUSE = 1 << 4,
	BSDF_ALL = BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR | BSDF_DIFFUSE | BSDF_GLOSSY
};

class Material
{
public:
	Material(Texture<float> *color, BSDFType type) : type(type), colorTexture(color) {}
	virtual geometry::color4f sample(const Ray &in, const ComputedIntersection &info, geometry::vec3f &wo, float &pdf, BSDFType &type) const = 0;
protected:
	Texture<float> *colorTexture;
	BSDFType type;
};

}