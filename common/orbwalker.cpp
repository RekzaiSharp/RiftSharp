#include "orbwalker.hpp"
#include <array>
#include <ppl.h>
#include "../libary/global.hpp"
#include "target_selector.hpp"
#include "../plugins/utility/bm_plugin.hpp"
#include "menu.hpp"
#include "prediction.hpp"
#include <string>

auto orbwalker_t::on_attack(void* object, void*, bool, bool, void*) -> void
{
	if (object != object_manager->local_player.object)
		return;

	object_manager->local_player.last_attack_tick = float(GetTickCount());
	object_manager->local_player.attack_completed = false;
	object_manager->local_player.missile_launched = false;
}

auto orbwalker_t::can_attack() -> bool
{
	if (object_manager->local_player.is_casting)
		return false;

	return float(GetTickCount()) + object_manager->local_player.ping / 2.f + 25.f >= object_manager->local_player.last_attack_tick + object_manager->local_player.attack_delay * 1000.f;
}

auto orbwalker_t::can_move() -> bool
{
	if (!object_manager->local_player.attack_completed && object_manager->local_player.missile_launched)
		return true;

	return float(GetTickCount()) + object_manager->local_player.ping * 0.5f >= object_manager->local_player.last_attack_tick + object_manager->local_player.attack_cast_delay * 1000.f + (object_manager->local_player.ping * 1.5f) + menu->ow_extra_windup;
}

auto orbwalker_t::move_to_mouse() -> void
{
	POINT p;
	if (!GetCursorPos(&p))
		return;

	SDKPOINT point = { p.x, p.y };
	SDKVECTOR vec = { 0, 0, 0 };
	global->context()._SdkScreenToWorld(&point, &vec);
	global->context()._SdkMoveLocalPlayer(&vec, false);
}

auto orbwalker_t::orbwalk() -> void
{ 
	if (menu->bm_active)
		bm_plugin->update_death_list();

	menu->draw_overlay();

	if (!menu->ow_active)
		return;

	std::array<std::pair<orbwalker_mode_t, int>, 5> modes =
	{
		std::make_pair(mode_last_hit, 88),
		std::make_pair(mode_lane_clear, 86),
		std::make_pair(mode_jungle_clear, 86),
		std::make_pair(mode_harass, 67),
		std::make_pair(mode_combo, 32)
	};

	orbwalker->orbwalker_mode_ = mode_none;
	for (auto& entry : modes)
	{
		if (!(GetAsyncKeyState(entry.second) & 0x8000))
			continue;

		orbwalker->orbwalker_mode_ = entry.first;
		break;
	}

	if (orbwalker->orbwalker_mode_ != mode_none && orbwalker->can_move())
		orbwalker->move_to_mouse();

	if (GetKeyState(VK_LBUTTON) & 0x80)
	{
		target_selector->force_target();
	}

	if (target_selector->forced_active) 
	{
		object_manager->update_forced_target();
		target_selector->main_target = target_selector->forced_target;
		if (orbwalker->orbwalker_mode_ == mode_combo && can_attack())
		{
			auto pred = prediction->get_line_prediction(1150.f, 2000.f, 0.250f);
			
			global->context()._SdkCastSpellLocalPlayer(nullptr, PSDKVECTOR(&pred), 0, SPELL_CAST_START);
			std::string temp = "prediction: {X: " + std::to_string(pred.x) + ", Y: " + std::to_string(pred.y) + ", Z:" + std::to_string(pred.z) + " }";
			global->context()._SdkConsoleWrite(temp.c_str());
			if (object_manager->local_player.object_position.DistanceTo(target_selector->forced_target.object_position) <= object_manager->local_player.attack_range)
				global->context()._SdkAttackTargetLocalPlayer(target_selector->forced_target.object, false);
		}
	}
	
}

std::unique_ptr<orbwalker_t> orbwalker = std::make_unique<orbwalker_t>();