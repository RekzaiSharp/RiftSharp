#pragma once
#include "object_manager.hpp"

class orbwalker_t
{

public:
	enum orbwalker_mode_t
	{
		mode_none,
		mode_last_hit,
		mode_lane_clear,
		mode_jungle_clear,
		mode_harass,
		mode_combo
	};

	static auto on_attack(void*, void*, bool, bool, void*) -> void;
	static auto can_attack() -> bool;
	static auto can_move() -> bool;
	static auto move_to_mouse() -> void;
	static auto orbwalk() -> void;

	orbwalker_mode_t orbwalker_mode = mode_none;
};

extern std::unique_ptr<orbwalker_t> orbwalker;