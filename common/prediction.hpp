#pragma once
#include "object_manager.hpp"

class prediction_t
{
public:
	enum hitchance_t
	{
		hitchance_low,
		hitchance_medium,
		hitchance_high,
		hitchance_veryhigh,
		hitchance_dashing,
		hitchance_immobile
	};

	enum skillshot_type_t
	{
		skillshot_line,
		skillshot_cone,
		skillshot_circle
	};

	enum collisionable_objects_t
	{
		collision_minions,
		collision_yasuowall
	};

	struct prediction_input_t
	{
		bool is_aoe = false;
		bool collision = false;
		std::vector<collisionable_objects_t> collision_objects = { collision_minions, collision_yasuowall };
		float spell_delay;
		float spell_radius;
		float spell_range;
		float spell_speed;
		skillshot_type_t skillshot_type = skillshot_line;
		bool use_bounding_radius = true;
		Vector caster_pos;
	};

	struct prediction_output_t
	{
		std::vector<object_manager_t::obj_ai_hero_t> aoe_heroes_hit;
		std::vector<object_manager_t::obj_ai_base_t> collision_objects;
		hitchance_t hitchance;
		prediction_input_t prediction_input;
		Vector cast_position;
		Vector unit_position;
	};

	size_t target_path_count;

	auto get_position_on_path(prediction_input_t pred_input) -> prediction_output_t;
	auto get_path_length(PSDKVECTOR waypoints) -> float;
	auto cut_path(PSDKVECTOR waypoints, float distance) -> std::vector<Vector>;
	static auto get_standart_prediction(prediction_input_t pred_input) -> prediction_output_t;
};

extern std::unique_ptr<prediction_t> prediction;