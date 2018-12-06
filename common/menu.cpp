#include "menu.hpp"
#include "orbwalker.hpp"

SDKCOLOR color_red = { 0, 0, 255, 255 };
SDKVECTOR direction_vector = { 0, 0, 1.f };

auto menu_t::save_settings() -> void
{
	//BM
	global->context()._SdkSetSettingBool("BM_ENABLED", menu->bm_active);
	global->context()._SdkSetSettingBool("BM_DISRESPECT", menu->bm_disrespect);
	global->context()._SdkSetSettingBool("BM_MASTERY", menu->bm_mastery_emote);

	//Orbwalker
	global->context()._SdkSetSettingBool("BM_ENABLED", menu->ow_active);
	global->context()._SdkSetSettingBool("BM_DRAWMYAUTO", menu->ow_auto_range);
	global->context()._SdkSetSettingNumber("BM_KEYCOMBO", menu->ow_combo_key);
	global->context()._SdkSetSettingNumber("BM_KEYHARASS", menu->ow_harass_key);
	global->context()._SdkSetSettingNumber("BM_KEYLANECLEAR", menu->ow_lane_clear_key);
	global->context()._SdkSetSettingNumber("BM_KEYLASTHIT", menu->ow_last_hit_key);
	global->context()._SdkSetSettingNumber("BM_KEYJUNGLECLEAR", menu->ow_jungle_clear_key);
}

auto menu_t::load_settings() -> void
{
	//BM
	global->context()._SdkGetSettingBool("BM_ENABLED", &menu->bm_active, true);
	global->context()._SdkGetSettingBool("BM_DISRESPECT", &menu->bm_disrespect, false);
	global->context()._SdkGetSettingBool("BM_MASTERY", &menu->bm_mastery_emote, false);

	//Orbwalker
	global->context()._SdkGetSettingBool("BM_ENABLED", &menu->ow_active, true);
	global->context()._SdkGetSettingBool("BM_DRAWMYAUTO", &menu->ow_auto_range, true);
	global->context()._SdkGetSettingNumber("BM_KEYCOMBO", &menu->ow_combo_key, 32);
	global->context()._SdkGetSettingNumber("BM_KEYHARASS", &menu->ow_harass_key, 67);
	global->context()._SdkGetSettingNumber("BM_KEYLANECLEAR", &menu->ow_lane_clear_key, 86);
	global->context()._SdkGetSettingNumber("BM_KEYLASTHIT", &menu->ow_last_hit_key, 88);
	global->context()._SdkGetSettingNumber("BM_KEYJUNGLECLEAR", &menu->ow_jungle_clear_key, 86);
}

auto menu_t::draw_menu() -> void
{
	global->context()._SdkUiText("[SharpOrbwalker] Loaded.");
	global->context()._SdkUiText("[SharpBM] Loaded.");

	auto bm_col(true);
	global->context()._SdkUiBeginWindow("SharpBM", &menu->bm_visible, &bm_col);
	if (menu->bm_visible && bm_col)
	{
		global->context()._SdkUiCheckbox("Enable", &menu->bm_active, nullptr);
		global->context()._SdkUiCheckbox("Disrespect", &menu->bm_disrespect, nullptr);
		global->context()._SdkUiCheckbox("Mastery Emote", &menu->bm_mastery_emote, nullptr);
		global->context()._SdkUiEndTree();
	}
	global->context()._SdkUiEndWindow();


	auto o_col(true);
	global->context()._SdkUiBeginWindow("SharpOrbwalker", &menu->ow_visible, &o_col);
	if (menu->ow_visible && o_col)
	{
		global->context()._SdkUiText("Early Access Orbwalker\n");
		global->context()._SdkUiCheckbox("Enable", &menu->ow_active, nullptr);

		auto b_drawings(false);
		global->context()._SdkUiBeginTree("Drawings", &b_drawings);
		if (b_drawings)
		{
			global->context()._SdkUiCheckbox("My Attack Range", &menu->ow_auto_range, nullptr);
			global->context()._SdkUiEndTree();
		}

		auto b_keys(false);
		global->context()._SdkUiBeginTree("Keys", &b_keys);
		if (b_keys)
		{
			global->context()._SdkUiInputNumber("Combo", &menu->ow_combo_key, nullptr);
			global->context()._SdkUiInputNumber("Harass", &menu->ow_harass_key, nullptr);
			global->context()._SdkUiInputNumber("Lane Clear", &menu->ow_lane_clear_key, nullptr);
			global->context()._SdkUiInputNumber("Jungle Clear", &menu->ow_jungle_clear_key, nullptr);
			global->context()._SdkUiInputNumber("Last Hit", &menu->ow_last_hit_key, nullptr);
			global->context()._SdkUiEndTree();
		}
	}
	global->context()._SdkUiEndWindow();


	save_settings();
}

auto menu_t::draw_overlay() -> void
{
	if (menu->ow_active)
	{
		if (menu->ow_auto_range)
		{
			global->context()._SdkDrawCircle((PSDKVECTOR)&object_manager->local_player.object_position, object_manager->local_player.attack_range, &color_red, 0, &direction_vector);
		}
	}
}


std::unique_ptr<menu_t> menu = std::make_unique<menu_t>();
