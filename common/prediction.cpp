#include "prediction.hpp"
#include "target_selector.hpp"
#include <algorithm>

auto prediction_t::get_line_prediction(const float spell_range, const float missile_speed, const float cast_time) -> Vector
{
	auto t = (target_selector->main_target.object_position - object_manager->local_player.object_position).Length() / missile_speed;
	t += cast_time;
	Vector velocity;
	global->context()._SdkGetAINavData(target_selector->main_target.object, nullptr, nullptr, nullptr, nullptr, nullptr, PSDKVECTOR(&velocity), nullptr, nullptr, nullptr);
	velocity.y = 0;
	const auto orientation = velocity.Normalize();

	if (velocity.x == 0 && velocity.z == 0)
	{
		return target_selector->main_target.object_position;
	}

	auto result = target_selector->main_target.object_position + orientation * target_selector->main_target.move_speed * t;
	
	return result;
}

auto prediction_t::will_collide(collisionable_objects_t collision_type, Vector pred_pos, float spell_radius) -> bool
{
	prediction->pred_pos = pred_pos;
	prediction->temp_radius = spell_radius;
	prediction->is_colliding = false;

	global->context()._SdkEnumMinions([](void* object, unsigned net_id, void*) -> bool
	{
		auto team_id(0);
		global->context()._SdkGetObjectTeamID(object, &team_id);
		if (team_id == object_manager->local_player.team_id)
			return true;

		Vector pos;
		global->context()._SdkGetObjectPosition(object, PSDKVECTOR(&pos));
		SDKPOINT minion_pos, target_pos, player_pos;
		global->context()._SdkWorldToScreen(PSDKVECTOR(&pos), &minion_pos);
		global->context()._SdkWorldToScreen(PSDKVECTOR(&object_manager->local_player.object_position), &player_pos);
		global->context()._SdkWorldToScreen(PSDKVECTOR(&prediction->pred_pos), &target_pos);

		if (prediction->point_on_line(Vector(player_pos.x, player_pos.y, 0.f), Vector(target_pos.x, target_pos.y, 0.f), Vector(minion_pos.x, minion_pos.y, 0.f), prediction->temp_radius))
		{
			prediction->is_colliding = true;
			return true;
		}
		return true;
	}, nullptr);

	return prediction->is_colliding;
}

auto prediction_t::point_on_line(Vector pt1, Vector pt2, Vector pt, double epsilon) -> bool
{
	if (pt.x - std::fmax(pt1.x, pt2.x) > epsilon ||
		std::fmin(pt1.x, pt2.x) - pt.x > epsilon ||
		pt.y - std::fmax(pt1.y, pt2.y) > epsilon ||
		std::fmin(pt1.y, pt2.y) - pt.y > epsilon)
		return false;

	if (abs(pt2.x - pt1.x) < epsilon)
		return abs(pt1.x - pt.x) < epsilon || abs(pt2.x - pt.x) < epsilon;
	if (abs(pt2.y - pt1.y) < epsilon)
		return abs(pt1.y - pt.y) < epsilon || abs(pt2.y - pt.y) < epsilon;

	double x = pt1.x + (pt.y - pt1.y) * (pt2.x - pt1.x) / (pt2.y - pt1.y);
	double y = pt1.y + (pt.x - pt1.x) * (pt2.y - pt1.y) / (pt2.x - pt1.x);

	return abs(pt.x - x) < epsilon || abs(pt.y - y) < epsilon;
}

std::unique_ptr<prediction_t> prediction = std::make_unique<prediction_t>();
