#include "karthus.hpp"
#include "../common/orbwalker.hpp"
#include <filesystem>
#include "../../../libary/fnv.hpp"
#include "../../../common/target_selector.hpp"
#include "../../../common/prediction.hpp"

karthus_t::karthus_t() : champion_t(fnv("Karthus")) {}

auto karthus_t::on_tick() -> void
{
	spell_laywaste();
}

auto karthus_t::menu() -> void
{
	auto k_op(true);
	global->context()._SdkUiBeginWindow("[RiftSharp] Karthus", &menu_visible, &k_op);
	if (menu_visible)
	{
		global->context()._SdkUiText("Karthus Loaded");
	}
	global->context()._SdkUiEndWindow();
}

auto karthus_t::spell_laywaste() -> void
{
	const auto target = target_selector->get_hero_target(target_selector_t::target_closest, object_manager->get_ai_heroes(875.f, false));
	target_selector->main_target = target;
	object_manager->update_main_target();
	auto pred = prediction->get_line_prediction(875.f, 1000.f, 0.625f);
	if (target.is_valid && orbwalker->orbwalker_mode == orbwalker_t::mode_combo)
		global->context()._SdkCastSpellLocalPlayer(nullptr, PSDKVECTOR(&pred), 0, SPELL_CAST_START);
}

