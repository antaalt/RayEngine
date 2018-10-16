#include "Accelerator.h"

#include <map>
#include "Material.h"
#include "Triangle.h"
#include "Sphere.h"
#include "PointLight.h"
#include "ConstantTexture.h"

namespace raycore {

	namespace tracer {

		Accelerator::Accelerator()
		{
		}


		Accelerator::~Accelerator()
		{

			for (size_t i = 0; i < this->hitables.size(); i++)
				delete this->hitables[i];
			for (size_t i = 0; i < this->materials.size(); i++)
				delete this->materials[i];
			for (size_t i = 0; i < this->lights.size(); i++)
				delete this->lights[i];
		}
		bool Accelerator::build(const Scene & scene)
		{ 
			// TODO move default build to accelerator constructor and call it from here
			std::map<const Texture*, Texture*> mapTexture;
			//std::map<unsigned int, const prim::Material*> mapMaterials; // TODO implement map

			// --- Textures
			this->textures.reserve(scene.textures.size());
			for (size_t iTex = 0; iTex < scene.textures.size(); iTex++)
			{
				const Texture *texture = scene.textures[iTex];
				this->textures.push_back(texture->clone()); // TODO clone to prevent empty node deleted by scene
				Texture *tex = this->textures.back();
				mapTexture.insert(std::make_pair(texture, tex));
			}
			// --- Materials
			this->materials.reserve(scene.materials.size());
			for (size_t iMat = 0; iMat < scene.materials.size(); iMat++)
			{
				const Material &material = scene.materials[iMat];
				prim::Material* newMaterial;
				auto it = mapTexture.find(material.texture);
				Texture *tex = (it == mapTexture.end() ? new ConstantTexture(material.color) : it->second);
				switch (material.type)
				{
				case MaterialType::DIFFUSE:
					newMaterial = new prim::Matte(tex);
					break;
				case MaterialType::SPECULAR:
					newMaterial = new prim::Matte(tex); //new prim::Specular();
					break;
				case MaterialType::DIELECTRIC:
					newMaterial = new prim::Glass(tex, 1.1f);
					break;
				case MaterialType::METAL:
					newMaterial = new prim::Metal(tex, 0.1f);
					break;
				default:
					return false;
				}
				this->materials.push_back(newMaterial);
			}
			// --- Nodes
			for (size_t iNode = 0; iNode < scene.nodes.size(); iNode++)
			{
				const Node &node = scene.nodes[iNode];
				const Shape *shape = node.shape;
				if (shape == nullptr)
					continue;
				// TODO add array of allowed shapetype for a given accelerator
				switch (shape->type())
				{
				case ShapeType::MESH:
				{
					const Mesh *mesh = dynamic_cast<const Mesh*>(shape);
					ASSERT(mesh != nullptr, "should not happend");
					const mat4 transform = node.getModel();
					const mat3 normalTransform = convert::toMat3(transform);
					float det = transform.det();
					unsigned int triVert1 = 1;
					unsigned int triVert2 = 2;
					if (det < 0.f) // mirror triangle
					{
						triVert1 = 2;
						triVert2 = 1;
					}
					for (size_t iPrim = 0; iPrim < mesh->primitives.size(); iPrim++)
					{
						const Primitive *prim = mesh->primitives[iPrim];
						ASSERT(prim != nullptr, "primitive should not be null");
						ASSERT(prim->material != nullptr, "material should not be null");
						for (size_t iTri = 0; iTri < prim->triangles.size(); iTri++)
						{
							const Triangle &tri = prim->triangles[iTri];
							prim::Vertex data[3];
							for (unsigned int iVert = 0; iVert < 3; iVert++)
							{
								const Vertex &v = prim->vertices[tri.vertices[iVert]];
								vec3 p = v.position;
								norm3 n = v.normal;
								p = transform * p;
								n = normalTransform * n;
								data[iVert] = prim::Vertex(p, n, v.texcoord, v.color);
								bbox.include(p);
							}
							prim::Triangle *newTri = new prim::Triangle(
								data[0],
								data[triVert1],
								data[triVert2]
							);
							newTri->material = this->materials[prim->material->index];
							this->hitables.push_back(newTri);
						}
					}
				}
				break;
				case ShapeType::SPHERE:
				{
					const Sphere *sphere = reinterpret_cast<const Sphere*>(shape);
					const mat4 transform = node.getModel();
					prim::Sphere* newSphere = new prim::Sphere(sphere->center, sphere->radius, sphere->up);
					newSphere->material = this->materials[sphere->material->index];
					this->hitables.push_back(newSphere);
					bbox.include(newSphere->computeBoundingBox());
				}
				break;
				case ShapeType::PARALLELOGRAM:
				{
					/*const Parallelogram *plane = reinterpret_cast<const Parallelogram*>(shape);
					const Matrix4 transform = node.getModel();
					prim::Parallelogram::Ptr newSphere = new prim::Parallelogram(plane->point[0], plane->point[1], plane->point[2], plane->normal);
					newSphere->material = this->materials[plane->material->index];
					this->hitables.push_back(newSphere);
					bbox.include(plane->point[0]);
					bbox.include(plane->point[1]);
					bbox.include(plane->point[2]);*/
					// TODO add 4th point
				}
				default:
					std::runtime_error("Type not supported");
				}
			}
			// --- Lights
			for (const raycore::Light& l : scene.lights)
			{
				PointLight *light = new PointLight(colorHDR(0.9f, 0.8f, 1.f, 1.f), 1.f, l.position);
				this->lights.push_back(light);
			}
			this->hitableCount = this->hitables.size();
			return true;
		}
		const Light * Accelerator::getLight(size_t index) const
		{
			return lights[index];
		}
		size_t Accelerator::getLightsCount() const
		{
			return lights.size();
		}
	}
}