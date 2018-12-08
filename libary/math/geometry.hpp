#pragma once
#include "vector.hpp"
#include <memory>

class geometry_t
{
public:
	struct object_t
	{
		float t1;
		Vector vec;
	};

	static auto vector_movement_collision(Vector start_point_1, Vector end_point_1, float v_1, Vector start_point_2, float v_2, float delay = 0.f) -> object_t;
	auto close(float a, float b, float eps) -> bool;
};

extern std::unique_ptr<geometry_t> geometry;
