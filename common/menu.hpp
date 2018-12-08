#pragma once
#include <memory>

class menu_t
{
public:
	//BM_Plugin
	bool bm_active = true;
	bool bm_visible = true;
	bool bm_disrespect = false;
	bool bm_mastery_emote = false;

	//Orbwalker
	bool ow_visible = true;
	bool ow_active = true;
	bool ow_auto_range = true;
	int ow_combo_key = 32;
	int ow_lane_clear_key = 86;
	int ow_jungle_clear_key = 86;
	int ow_harass_key = 67;
	int ow_last_hit_key = 88;
	float ow_extra_windup = 40.f;

	static auto save_settings() -> void;
	static auto load_settings() -> void;
	static auto draw_menu() -> void;
	static auto draw_overlay() -> void;
};

extern std::unique_ptr<menu_t> menu;