#pragma once
#include <vector>
#include <memory>
#include "../libary/global.hpp"
#include "../libary/math/vector.hpp"

class object_manager_t
{
public:
	struct obj_ai_base_t
	{
		void* object;
		unsigned net_id;
		std::string object_name;
		Vector object_position;
		SDK_HEALTH object_health;
		float bounding_radius;
	};

	struct obj_ai_hero_t
	{
		void* object;
		unsigned net_id;
		int ping;
		float move_speed;
		std::string object_name;
		Vector object_position;
		SDK_HEALTH object_health;
		float object_distance;
		bool is_casting;
		float attack_damage;
		float ability_power;
		float bounding_radius;
		float projectile_speed;
		float attack_range;
		float attack_cast_delay;
		float attack_delay;
	};

	struct obj_local_player
	{
		void* object;
		unsigned net_id;
		unsigned ping;
		int team_id;
		std::string object_name;
		float attack_speed;
		Vector object_position;
		SDK_HEALTH object_health;
		bool is_casting;
		float attack_damage;
		float bounding_radius;
		float projectile_speed;
		float attack_range;
		float attack_cast_delay;
		float attack_delay;
		float last_attack_tick = 0;
		bool attack_completed;
		bool missile_launched;
	};

	struct obj_ai_minion_t
	{
		void* object;
		unsigned net_id;
		std::string object_name;
		Vector object_position;
		SDK_HEALTH object_health;
		float bounding_radius;
		bool is_valid;
		float unit_bounds;
		float attack_damage;
		float distance;
	};

	struct obj_ai_turret_t
	{
		void* object;
	};

	obj_local_player local_player; // -> Returns information about the local player
	std::vector<obj_ai_base_t> vector_object_base;
	std::vector<obj_ai_minion_t> vector_object_minion;
	std::vector<obj_ai_hero_t> vector_object_hero;
	std::vector<obj_ai_turret_t> vector_object_turret;
	//std::vector<obj_ai_missile_t> vector_object_missile;

	float temp_range = 0.f;
	bool temp_team = false;
	void* temp_unit;

	static auto get_local_player() -> void; // -> Initializes the local player
	static auto update_local_player(void* object, unsigned net_id, void*) -> bool; // -> Updates information about the local player
	static auto update_forced_target() -> void;
	static auto get_ai_base(float range)->std::vector<obj_ai_base_t>; // -> Returns every attackable object in range X
	static auto get_ai_minions(float range)->std::vector<obj_ai_minion_t>; // -> Returns every minion in range X
	static auto get_ai_heroes(float range, bool all_teams)->std::vector<obj_ai_hero_t>; // -> Returns every hero in range X
	static auto get_ai_turrets(float range)->std::vector<obj_ai_turret_t>; // -> Returns every turret in range X
};

extern std::unique_ptr<object_manager_t> object_manager;