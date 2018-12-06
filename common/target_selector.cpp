#include "target_selector.hpp"
#include <algorithm>
#include <functional>
#include "../libary/global.hpp"

#define ADD_MODE(id, check) case id: std::sort(targets.begin(), targets.end(), [](const object_manager_t::obj_ai_hero_t& a, const object_manager_t::obj_ai_hero_t& b) { return check; }); break


auto target_selector_t::get_hero_target(target_mode_t mode, std::vector<object_manager_t::obj_ai_hero_t> hero_list) const -> object_manager_t::obj_ai_hero_t
{
	auto targets = hero_list;
	switch (mode)
	{
		ADD_MODE(target_low_hp, a.object_health.Current < b.object_health.Current);
		ADD_MODE(target_most_hp, a.object_health.Current > b.object_health.Current);
		ADD_MODE(target_closest, a.object_distance < b.object_distance);
		ADD_MODE(target_most_ad, a.attack_damage > b.attack_damage);
		ADD_MODE(target_most_ap, a.ability_power > b.ability_power);
	default: break;
	}

	return targets.front();
}

auto target_selector_t::force_target() const -> bool
{
	POINT p;
	if (GetCursorPos(&p))
	{
		Vector coords;
		global->context()._SdkScreenToWorld(PSDKPOINT(&p), PSDKVECTOR(&coords));
		object_manager->local_player.object_position = coords;
		auto targets = object_manager->get_ai_heroes(500.f, false);
		if (targets.empty())
		{
			target_selector->forced_active = false;
			return false;
		}
		std::sort(targets.begin(), targets.end(), [](const object_manager_t::obj_ai_hero_t& a, const object_manager_t::obj_ai_hero_t& b) { return a.object_distance < b.object_distance; });
		target_selector->forced_target = targets.front();
		target_selector->forced_active = true;
		return true;
	}
	return false;
}

std::unique_ptr<target_selector_t> target_selector = std::make_unique<target_selector_t>();