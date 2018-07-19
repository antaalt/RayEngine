#pragma once

#include "Config.h"
#include <climits>


namespace Application {

	namespace RayTracer {

		enum RayType {
			GEOMETRY_RAY,
			SHADOW_RAY
		};

		struct Ray {
			Point3 origin;
			Vector3 direction;
			float tmin, tmax;
			RayType type;
			Ray(Point3 origin, Vector3 direction, RayType ray_type = GEOMETRY_RAY, float tmin = EPSILON, float tmax = std::numeric_limits<float>::max()) :
				origin(origin),
				direction(direction),
				type(ray_type),
				tmin(tmin),
				tmax(tmax)
				{}
		};
	}
}