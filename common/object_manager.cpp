#include "object_manager.hpp"
#include "../libary/global.hpp"
#include <string>

auto object_manager_t::get_local_player() -> void
{
	global->context()._SdkGetLocalPlayer(&object_manager->local_player.object);

	global->context()._SdkEnumGameObjects([](void* object, unsigned net_id, void*) -> bool
	{
		if (object != object_manager->local_player.object)
			return true;

		global->context()._SdkGetObjectNetworkID(object, &object_manager->local_player.net_id);
		auto temp_name = "";
		global->context()._SdkGetAIName(object, &temp_name);
		object_manager->local_player.object_name = temp_name;
		global->context()._SdkGetObjectTeamID(object, &object_manager->local_player.team_id);

		return true;
	}, nullptr);
}

auto object_manager_t::update_local_player(void* object, unsigned net_id, void*) -> bool
{
	if (object != object_manager->local_player.object)
		return true;

	global->context()._SdkGetObjectPosition(object, PSDKVECTOR(&object_manager->local_player.object_position));
	global->context()._SdkGetNetPing(&object_manager->local_player.ping);
	global->context()._SdkGetAIAttackCastDelay(object, &object_manager->local_player.attack_cast_delay);
	global->context()._SdkGetAIAttackDelay(object, &object_manager->local_player.attack_delay);
	float temp_range(0);
	global->context()._SdkGetAIAttackRange(object, &temp_range);
	global->context()._SdkGetObjectBoundingRadius(object, &object_manager->local_player.bounding_radius);
	object_manager->local_player.attack_range = temp_range + object_manager->local_player.bounding_radius;
	global->context()._SdkGetAIAttackDamage(object, &object_manager->local_player.attack_damage);
	//object_manager->local_player.
	return true;
}

auto object_manager_t::get_ai_base(float range) -> std::vector<obj_ai_base_t>
{
	object_manager->vector_object_base.clear();
	object_manager->temp_range = range;

	global->context()._SdkEnumGameObjects([](void* object, unsigned net_id, void*) -> bool
	{
		if (object == object_manager->local_player.object)
			return true;

		Vector position;
		global->context()._SdkGetObjectPosition(object, PSDKVECTOR(&position));

		if (object_manager->local_player.object_position.DistanceTo(position) > object_manager->temp_range)
			return true;

		auto is_alive(false);
		global->context()._SdkIsObjectDead(object, &is_alive);
		if (!is_alive)
			return true;

		if (!((global->context()._SdkIsObjectUnit(object) == SDKSTATUS(0))))
			return true;

		obj_ai_base_t base_object;
		global->context()._SdkGetUnitHealth(object, &base_object.object_health);
		if (base_object.object_health.Current < 1.0f)
			return true;

		auto in_fog_of_war(false);
		global->context()._SdkIsUnitVisible(object, &in_fog_of_war);
		if (!in_fog_of_war)
			return true;

		base_object.object = object;
		base_object.net_id = net_id;
		auto temp_name = "";
		global->context()._SdkGetObjectName(object, &temp_name);
		base_object.object_name = temp_name;
		global->context()._SdkGetObjectBoundingRadius(object, &base_object.bounding_radius);

		object_manager->vector_object_base.push_back(std::move(base_object));
		return true;
	}, nullptr);
	object_manager->vector_object_base.resize(object_manager->vector_object_base.size());
	return object_manager->vector_object_base;
}

auto object_manager_t::get_ai_minions(float range) -> std::vector<obj_ai_minion_t>
{
	object_manager->temp_range = range;
	object_manager->vector_object_minion.clear();

	global->context()._SdkEnumMinions([](void* object, unsigned net_id, void*) -> bool
	{
		Vector position;
		global->context()._SdkGetObjectPosition(object, PSDKVECTOR(&position));
		if (object_manager->local_player.object_position.DistanceTo(position) > object_manager->temp_range)
			return true;

		auto is_alive(false);
		global->context()._SdkIsObjectDead(object, &is_alive);
		if (!is_alive)
			return true;

		if (!((global->context()._SdkIsObjectUnit(object) == SDKSTATUS(0))))
			return true;

		obj_ai_minion_t minion_object;
		global->context()._SdkGetUnitHealth(object, &minion_object.object_health);
		if (minion_object.object_health.Current < 1.0f)
			return true;

		auto in_fog_of_war(false);
		global->context()._SdkIsUnitVisible(object, &in_fog_of_war);
		if (!in_fog_of_war)
			return true;

		minion_object.object = object;
		minion_object.net_id = net_id;
		minion_object.object_position = position;
		global->context()._SdkGetObjectBoundingRadius(object, &minion_object.bounding_radius);
		global->context()._SdkGetAIAttackDamage(object, &minion_object.attack_damage);
		minion_object.distance = object_manager->local_player.object_position.DistanceTo(position);
		auto temp_name = "";
		global->context()._SdkGetObjectName(object, &temp_name);
		minion_object.object_name = temp_name;

		object_manager->vector_object_minion.push_back(std::move(minion_object));

		return true;
	}, nullptr);

	object_manager->vector_object_minion.resize(object_manager->vector_object_minion.size());

	return object_manager->vector_object_minion;
}

auto object_manager_t::get_ai_heroes(float range, bool all_teams) -> std::vector<obj_ai_hero_t>
{
	object_manager->temp_range = range;
	object_manager->temp_team = all_teams;
	object_manager->vector_object_hero.clear();

	global->context()._SdkEnumHeroes([](void* object, unsigned net_id, void*) -> bool
	{
		Vector position;
		global->context()._SdkGetObjectPosition(object, PSDKVECTOR(&position));
		if (object_manager->local_player.object_position.DistanceTo(position) > object_manager->temp_range)
			return true;

		auto team_id(0);
		global->context()._SdkGetObjectTeamID(object, &team_id);
		if (team_id == object_manager->local_player.team_id)
			return true;

		auto is_alive(false);
		global->context()._SdkIsObjectDead(object, &is_alive);
		if (!is_alive)
			return true;

		obj_ai_hero_t hero_object;
		hero_object.object = object;
		hero_object.net_id = net_id;
		hero_object.object_distance = object_manager->local_player.object_position.DistanceTo(position);
		global->context()._SdkGetAIAttackRange(object, &hero_object.attack_range);
		auto name = "";
		global->context()._SdkGetAIName(object, &name);
		hero_object.object_name = name;
		global->context()._SdkConsoleWrite(name);
	

		object_manager->vector_object_hero.push_back(std::move(hero_object));

		return true;
	}, nullptr);

	return object_manager->vector_object_hero;
}

auto object_manager_t::get_ai_turrets(float range) -> std::vector<obj_ai_turret_t>
{
	return object_manager->vector_object_turret;
}

std::unique_ptr<object_manager_t> object_manager = std::make_unique<object_manager_t>();
