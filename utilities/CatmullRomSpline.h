#pragma once
#include "ViewProjection.h"
#include "CalcMath.h"
#include <PrimitiveDrawer.h>
#include <vector>

class CatmullRomSpline {
public:
	void Initialize(const ViewProjection& view);

	void Update();

	void Draw();

private:
	std::vector<Vector3> controlPoints_;
	PrimitiveDrawer* primitiveDrawer_;
	std::vector<Vector3> pointsDrawing_;
	size_t segmentCount_;
	Vector3 Catmull_Rom(std::vector<Vector3> points, float t);
};
