#pragma once
#include "Hitable.h"

namespace raycore {
	namespace prim {
		class Sphere : public Hitable
		{
		public:
			Sphere();
			Sphere(const point3 &center, float radius);
			~Sphere();

			virtual bool intersect(const tracer::Ray &ray, Intersection &intersection) const;
			virtual HitInfo computeIntersection(const tracer::Ray &ray, const Intersection &intersection) const;
			virtual BoundingBox computeBoundingBox() const;

		protected:
			float m_radius;
			point3 m_center;
		};
	}
}

