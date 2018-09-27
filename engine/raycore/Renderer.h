#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "PixelBuffer.h"

#include <string>

#define TILE_WIDTH_NUMBER 20
#define TILE_HEIGHT_NUMBER 20
#define PARALLEL_RENDERING

namespace raycore {


	namespace tracer {

		struct Tile {
			Tile(const ivec2 &min, const ivec2 &max): min(min), max(max) {}

			int width() { return max.x - min.x; }
			int height() { return max.y - min.y; }
			ivec2 center() { return (max - min) * 0.5f; }

			ivec2 min;
			ivec2 max;
		};

		class Renderer
		{
		public:
			Renderer(unsigned int width, unsigned int height, unsigned int tileSize);
			~Renderer();
			Renderer(const Renderer& other) = delete;
			Renderer& operator=(const Renderer &other) = delete;

			bool buildScene(Scene &&scene, Acceleration acceleration);

			bool updateRays();

			bool renderPreview();

			bool render();

			void buildTiles(unsigned int tileSize);

			void setTracer(tracer::Tracer* tracer);
			void setCamera(tracer::Camera* camera);

			const PixelBuffer &image() const;

		private:
			//Scene scene;
			Camera* camera;
			Tracer* tracer;
			Accelerator::Ptr accelerator;

			std::vector<Ray> rays;
			std::vector<Tile> tiles;
			unsigned int tileSize;
			unsigned int width, height;
			PixelBuffer output;
			unsigned int samples;
		};
	}
}