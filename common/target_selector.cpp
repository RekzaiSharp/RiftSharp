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

auto target_selector_t::force_target() -> void
{
	target_selector->forced_active = false;

	global->context()._SdkEnumHeroes([](void* object, unsigned net_id, void*) -> bool
	{
		auto is_target(false);
		global->context()._SdkGetUnitIsMouseOver(object, &is_target);
		if (!is_target)
			return true;

		auto team_id(0);
		global->context()._SdkGetObjectTeamID(object, &team_id);
		if (team_id == object_manager->local_player.team_id)
			return true;

		object_manager_t::obj_ai_hero_t target;
		target.object = object;
		target.net_id = net_id;
		auto name = "";
		global->context()._SdkGetAIName(object, &name);
		target.object_name = name;
		target_selector->forced_active = true;
		target_selector->forced_target = target;
		return true;
	}, nullptr);
}

std::unique_ptr<target_selector_t> target_selector = std::make_unique<target_selector_t>();