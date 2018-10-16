#pragma once
#include "Types.h"

namespace raycore {

		class Fresnel
		{
		public:
			virtual float evaluate(const vec3 &direction, const vec3 &normal) const = 0;
		};

		// @see http://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/reflection_refraction.pdf
		class Schlick : public Fresnel
		{
		public:
			Schlick(float eta1, float eta2) : eta1(eta1), eta2(eta2)
			{
			}

			virtual float evaluate(const vec3 &direction, const vec3 &normal) const
			{
				bool inside = vec3::dot(direction, normal) >= 0.f;
				float n1 = eta2;
				float n2 = eta1;
				vec3 nn = normal;
				if (!inside)
				{
					std::swap(n1, n2);
					nn = -normal;
				}
				auto pow5 = [](float v) { return (v*v)*(v*v)*v; };
				float R0 = (n1 - n2) / (n1 + n2);
				float R02 = R0 * R0;
				float NdotV = vec3::dot(nn, direction);
				return R02 + (1.f - R02) * pow5(1.f - NdotV);
			}
		protected:
			float eta1, eta2;
		};
}