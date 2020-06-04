#pragma once

#include <vector>

#include "Hitable.h"

namespace prim {

norm3f interpolate(const norm3f &nA, const norm3f &nB, const norm3f &nC, vec2f barycentric);
uv2f interpolate(const uv2f &tA, const uv2f &tB, const uv2f &tC, vec2f barycentric);
color4f interpolate(const color4f &cA, const color4f &cB, const color4f &cC, vec2f barycentric);

class Mesh : public Hitable {
public:
	union Triangle {
		uint32_t data[3];
		struct {
			uint32_t A, B, C;
		};
	};
public:
	Mesh(const mat4f &transform, Material *material);

	void addTriangle(const Triangle &tri) { m_triangles.push_back(tri); }
	void addPosition(const point3f &point) { m_positions.push_back(point); }
	void addNormal(const norm3f &normal) { m_normals.push_back(normal); }
	void addUV(const uv2f &uv) { m_uvs.push_back(uv); }
	void addColor(const color4f &color) { m_colors.push_back(color); }

	void addTriangles(const Triangle *tri, size_t count) { m_triangles.insert(m_triangles.begin(), tri, tri + count); }
	void addPositions(const point3f *point, size_t count) { m_positions.insert(m_positions.begin(), point, point + count); }
	void addNormals(const norm3f *normal, size_t count) { m_normals.insert(m_normals.begin(), normal, normal + count); }
	void addUVs(const uv2f *uv, size_t count) { m_uvs.insert(m_uvs.begin(), uv, uv + count); }
	void addColors(const color4f *color, size_t count) { m_colors.insert(m_colors.begin(), color, color + count); }

	void build() override;

	bool intersect(const Ray &ray, Intersection &intersection) const override;

	void include(BoundingBox &boundingBox) override;
protected:
	void compute(const point3f &hitPoint, const vec2f &barycentric, Intersection::Indice indice, norm3f *normal, uv2f *texCoord, color4f *color) const override;
	point3f center(const Mesh::Triangle &tri) const;
	float area(const Mesh::Triangle &tri) const;
	bool intersectTri(const Mesh::Triangle &tri, const Ray &ray, Intersection &intersection) const;
protected:
	BoundingBox m_bbox;
	std::vector<Triangle> m_triangles;
	std::vector<point3f> m_positions;
	std::vector<norm3f> m_normals;
	std::vector<uv2f> m_uvs;
	std::vector<color4f> m_colors;
};

}
