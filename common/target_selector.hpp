#pragma once
#include "object_manager.hpp"


class target_selector_t
{
public:

	enum target_mode_t
	{
		target_auto,
		target_forced,
		target_low_hp,
		target_most_hp,
		target_most_ad,
		target_most_ap,
		target_closest,
		target_near_mouse,
		target_less_attack,
		target_less_cast,
		target_stack,
		target_max
	};

	object_manager_t::obj_ai_hero_t forced_target;
	bool forced_active;

	auto get_hero_target(target_mode_t mode, std::vector<object_manager_t::obj_ai_hero_t> hero_list) const->object_manager_t::obj_ai_hero_t;
	auto force_target() const -> bool;

};

extern std::unique_ptr<target_selector_t> target_selector;