#include "prediction.hpp"
#include "target_selector.hpp"
#include <algorithm>
#include "../libary/math/geometry.hpp"

auto prediction_t::get_position_on_path(prediction_input_t pred_input) -> prediction_output_t
{
	auto speed = target_selector->forced_target.move_speed;
	Vector end_point;
	PSDKVECTOR waypoint;
	global->context()._SdkGetAINavData(target_selector->forced_target.object, nullptr, PSDKVECTOR(&end_point), nullptr, &target_path_count, &waypoint, nullptr, nullptr, nullptr, nullptr);

	if (target_path_count <=  1)
	{
		prediction_output_t prediction_output;
		prediction_output.prediction_input = pred_input;
		prediction_output.unit_position = target_selector->forced_target.object_position;
		prediction_output.hitchance = hitchance_veryhigh;
		return prediction_output;
	}

	auto path_length = target_selector->forced_target.object_position.DistanceTo(end_point);
	auto t_distance = pred_input.spell_delay * speed - target_selector->forced_target.bounding_radius;

	if (path_length >= t_distance)
	{
		for (auto i = 0; i < target_path_count - 1; ++i)
		{
			auto a = waypoint[i];
			Vector scuffed_a = { a.x, a.y, a.z };
			auto b = waypoint[i + 1];
			Vector scuffed_b = { b.x, b.y, b.z };
			auto d = scuffed_a.DistanceTo(scuffed_b);

			if (d >= t_distance)
			{
				auto direction = (scuffed_b - scuffed_a).Normalize();
				auto cp = scuffed_a + direction * t_distance;
				auto p = scuffed_a + direction * ((i == target_path_count - 2) ? std::minmax(t_distance + pred_input.spell_radius, d).first : t_distance + pred_input.spell_radius);
				prediction_output_t prediction_output;
				prediction_output.prediction_input = pred_input;
				prediction_output.unit_position = p;
				prediction_output.hitchance = hitchance_high;
				return prediction_output;
			}
			t_distance -= d;
		}
	}

	if (path_length >= pred_input.spell_delay * speed - pred_input.spell_radius > 0)
	{
		auto d = pred_input.spell_delay * speed - pred_input.spell_radius;

		if (pred_input.skillshot_type == skillshot_line || pred_input.skillshot_type == skillshot_cone)
		{
			if (target_selector->forced_target.object_position.DistanceTo(object_manager->local_player.object_position) < 200 * 200)
			{
				d = pred_input.spell_delay * speed;
			}
		}

		auto waypoint_cut = prediction->cut_path(waypoint, d);
		auto t_T = 0.f;
		for (auto i = 0; i < target_path_count - 1; i++)
		{
			auto a = waypoint_cut[i];
			Vector scuffed_a = { a.x, a.y, a.z };
			auto b = waypoint_cut[i + 1];
			Vector scuffed_b = { b.x, b.y, b.z };
			auto t_B = scuffed_a.DistanceTo(scuffed_b) / speed;
			auto direction = (scuffed_b - scuffed_a).Normalize();
			scuffed_a = scuffed_a - direction * speed * t_T;
			POINT from_2d;
			global->context()._SdkWorldToScreen(PSDKVECTOR(&pred_input.caster_pos), PSDKPOINT(&from_2d));
			Vector vec_2d = {from_2d.x, from_2d.y, 0};
			auto sol = geometry->vector_movement_collision(scuffed_a, scuffed_b, speed, vec_2d, pred_input.spell_speed, t_T);
			auto t = float(sol.t1);
			auto pos = sol.vec;
			if (pos.IsValid() && t >= t_T && t <= t_T + t_B)
			{
				if (pos.DistanceTo(scuffed_b) < 20) break;
				auto p = pos + direction * pred_input.spell_radius;

				if (pred_input.skillshot_type == skillshot_line && false)
				{
					auto alpha = (vec_2d - p).AngleBetween(scuffed_a - scuffed_b);
					if (alpha > 30 && alpha < 180 - 30)
					{
						auto beta = float(asin(pred_input.spell_radius / p.DistanceTo(pred_input.caster_pos)));
						auto cp_1 = vec_2d + (p - vec_2d.Rotated(beta));
						auto cp_2 = vec_2d + (p - vec_2d.Rotated(-beta));

						pos = cp_1.DistanceTo(pos) < cp_2.DistanceTo(pos) ? cp_1 : cp_2;
					}
				}

				prediction_output_t output;
				output.prediction_input = pred_input;
				POINT tex = { pos.x, pos.y };
				Vector true_pos;
				global->context()._SdkScreenToWorld(PSDKPOINT(&tex), PSDKVECTOR(&true_pos));
				output.unit_position = true_pos;
				output.hitchance = hitchance_high;
				return output;
			}
			t_T += t_B;
		}
	}

	auto position = end_point;
	prediction_output_t output;
	output.prediction_input = pred_input;
	output.unit_position = position;
	output.hitchance = hitchance_medium;
	return output;
}

auto prediction_t::get_path_length(PSDKVECTOR waypoints) -> float
{
	auto distance = 0.f;
}

auto prediction_t::cut_path(PSDKVECTOR waypoints, float distance) -> std::vector<Vector>
{
	std::vector<Vector> out;
	auto Distance = distance;
	if (distance < 0)
	{
		Vector vec_0 = { waypoints[0].x, waypoints[0].y, waypoints[0].z };
		Vector vec_1 = { waypoints[1].x, waypoints[1].y, waypoints[1].z };
		vec_0 = vec_0 + ((vec_1 - vec_0).Normalize() * distance);
		return { vec_0 };
	}

	for (auto i = 0; i < target_path_count - 1; i++)
	{
		Vector temp_vec = { waypoints[i].x, waypoints[i].y, waypoints[i].z };
		Vector temp_vec2 = { waypoints[i + 1].x ,waypoints[i + 1].y, waypoints[i + 1].z };
		auto dist = temp_vec.DistanceTo(temp_vec2);
		if (dist > Distance)
		{
			out.push_back(std::move(temp_vec + ((temp_vec2 - temp_vec).Normalize() * Distance)));
			for (auto j = i + 1; j < target_path_count; j++)
			{
				Vector vec_out = { waypoints[j].x, waypoints[j].y, waypoints[j].z };
				out.push_back(std::move(vec_out));
			}

			break;
		}
		Distance -= dist;
	}
	std::vector<Vector> last = { {out.back()} };
	return !out.empty() ? out : last;
}

auto prediction_t::get_standart_prediction(prediction_input_t pred_input) -> prediction_output_t
{
	auto speed = target_selector->forced_target.move_speed;

	if (target_selector->forced_target.object_position.DistanceTo(pred_input.caster_pos) < 200 * 200)
	{
		speed /= 1.5f;
	}

	return prediction->get_position_on_path(pred_input);;
}

std::unique_ptr<prediction_t> prediction = std::make_unique<prediction_t>();