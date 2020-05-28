#include "Matte.h"

namespace raycore {
namespace prim{

Matte::Matte(Texture<float> * color) :
	Material(color, BSDFType(BSDF_DIFFUSE | BSDF_REFLECTION))
{
}

color4f Matte::sample(const tracer::Ray & in, const prim::HitInfo & info, vec3f & wo, float & pdf, BSDFType &type) const
{
	LambertianReflection brdf(colorTexture->evaluate(info.texcoord), info);
	type = this->type;
	return brdf.sample(wo, pdf);
}

}
}
