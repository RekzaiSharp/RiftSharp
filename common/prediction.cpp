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


std::unique_ptr<prediction_t> prediction = std::make_unique<prediction_t>();