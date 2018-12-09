#include "geometry.hpp"
#include <cmath>
#include <float.h> 
#include <math.h>
#include <limits>
#include <algorithm>

auto geometry_t::vector_movement_collision(Vector start_point_1, Vector end_point_1, float v_1, Vector start_point_2,
	float v_2, float delay) -> object_t
{
	const auto sp_1_x = start_point_1.x;
	const auto sp_1_y = start_point_1.y;
	const auto ep_1_x = end_point_1.x;
	const auto ep_1_y = end_point_1.y;
	const auto sp_2_x = start_point_2.x;
	const auto sp_2_y = start_point_2.y;

	const auto d = ep_1_x - sp_1_x;
	const auto e = ep_1_y - sp_1_y;
	const auto dist = float(sqrt(d * d + e * e));
	auto t1 = 0.f;
	const auto s = abs(dist) > 0.f ? v_1 * d / dist : 0.f;
	const auto k = abs(dist) > 0.f ? v_1 * e / dist : 0.f;

	const auto r = sp_2_x - sp_1_x;
	const auto j = sp_2_y - sp_1_y;
	const auto c = r * r + j * j;

	if (dist > 0.f)
	{
		if (abs(v_1 - FLT_MAX) < FLT_EPSILON)
		{
			const auto t = dist / v_1;
			t1 = v_2 * t >= 0.f ? t : 0.f;
		}
		else if (abs(v_2 - FLT_MAX) < FLT_EPSILON)
		{
			t1 = 0.f;
		}
		else
		{
			const auto a = s * s + k * k - v_2 * v_2;
			const auto b = -r * s - j * k;

			if (abs(a) < FLT_EPSILON)
			{
				if (abs(b) < FLT_EPSILON)
				{
					t1 = abs(c) < FLT_EPSILON ? 0.f : NAN;
				}
				else
				{
					auto t = -c / (2 * b);
					t1 = (v_2 * t >= 0.f) ? t : NAN;
				}
			}
			else
			{
				const auto sqr = b * b - a * c;
				if (sqr >= 0)
				{
					const auto nom = sqrt(sqr);
					auto t = (-nom - b) / a;
					t1 = v_2 * t >= 0.f ? t : NAN;
					t = (nom - b) / a;
					const auto t2 = (v_2 * t >= 0.f) ? t : NAN;
					if (!std::isnan(t2) && !std::isnan(t1))
					{
						if (t1 >= delay && t2 >= delay)
						{
							t1 = std::minmax(t1, t2).first;
						}
						else if (t2 >= delay)
						{
							t1 = t2;
						}
					}
				}
			}
		}
	}
	else if (abs(dist) < FLT_EPSILON)
	{
		t1 = 0.f;
	}
	object_t object;
	object.t1 = t1;
	Vector vec1 = { sp_1_x + s * t1, sp_1_y + k * t1, 0.f};
	Vector vec2;
	object.vec = !std::isnan(t1) ? vec1 : vec2;
	return object;
}

auto geometry_t::close(float a, float b, float eps) -> bool
{
	if (abs(eps) < FLT_EPSILON)
	{
		eps = float(1e-9);
	}
	return abs(a - b) <= eps;
}

std::unique_ptr<geometry_t> geometry = std::make_unique<geometry_t>();
