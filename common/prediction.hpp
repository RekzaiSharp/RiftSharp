#pragma once
#include "object_manager.hpp"

class prediction_t
{
public:
	enum collisionable_objects_t
	{
		collision_minions,
		collision_yasuowall
	};

	size_t target_path_count;
	Vector pred_pos;
	float temp_radius;
	bool is_colliding;

	auto get_line_prediction(float spell_range, float missile_speed, float cast_time) -> Vector;
	auto will_collide(collisionable_objects_t collision_type, Vector pred_pos, float spell_radius) -> bool;
	auto point_on_line(Vector pt1, Vector pt2, Vector pt, double epsilon) -> bool;
};

extern std::unique_ptr<prediction_t> prediction;