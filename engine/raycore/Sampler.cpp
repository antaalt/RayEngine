#include "Sampler.h"


namespace raycore {
	namespace rand {

		Sampler::Sampler()
		{
		}


		Sampler::~Sampler()
		{
		}


		float Sampler::r1() const
		{
			return Random::get(0.f, 1.f);
		}
		float Sampler::r2() const
		{
			return Random::get(0.f, 1.f);
		}

		float Sampler::nextFloat() const
		{
			return Random::get(0.f, 1.f);
		}

	}
}