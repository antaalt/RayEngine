#include "Material.h"

#include "Mathematic.h"
#include "Sampler.h"


namespace raycore {
	namespace prim {
		
		vec3 reflect(const vec3 & incident, const norm3 & normal)
		{
			return incident - 2.f * vec3::dot(incident, normal) * normal;
		}
		bool refract(vec3 &wt, const vec3 & wi, const norm3 & n, float eta)
		{
			/*float cosThetaI = vec3::dot(wi, n);
			float sin2thetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
			float sin2thetaT = eta * eta * sin2thetaI;
			if (sin2thetaT >= 1) return false;
			float cosThetaT = std::sqrt(1.f - sin2thetaT);
			wt = eta * -wi + (eta * cosThetaI - cosThetaT) * vec3(n);
			return true;*/
			float IdotN = vec3::dot(wi, n);
			float k = 1.f - eta * eta * (1.f - IdotN * IdotN);
			if (k < 0.f)
				return true;
			wt = (eta * (wi + IdotN * n) - n * sqrt(k));
			return false;
		}
		vec3 sampleUnitSphere(const rand::Sampler &sampler)
		{
			float r1 = sampler.r1();
			float r2 = sampler.r2();
			
			float sinTheta = sqrtf(1 - r1 * r1);
			float phi = 2.f * M_PIf * r2;
			float x = sinTheta * cosf(phi);
			float z = sinTheta * sinf(phi);
			return vec3(x, r1, z);
		}
		vec3 sampleMicroFacet(const rand::Sampler &sampler, float roughness)
		{
			float r1 = sampler.r1();
			float r2 = sampler.r2();
			float phi = std::atanf(roughness * std::sqrtf(r1) / std::sqrtf(1 - r1));
			float theta = 2 * M_PIf * r2;
			vec3 p;
			p.x = std::sinf(phi) * std::cosf(theta);
			p.y = std::sinf(phi) * std::sinf(theta);
			p.z = std::cosf(phi);
			return p;
		}
	}
}