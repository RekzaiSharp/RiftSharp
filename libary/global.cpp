#include "global.hpp"
#include "../common/menu.hpp"
#include "../common/object_manager.hpp"
#include "../plugins/champion/ezreal/ezreal.hpp"
#include "../common/orbwalker.hpp"
#include "fnv.hpp"
#include "../plugins/champion/karthus/karthus.hpp"

global_t::global_t(SDK_CONTEXT& context) : context_(context)
{
	context_._SdkRegisterOnLoad([](void*) -> void
	{
		object_manager->get_local_player();
		global->context()._SdkRegisterOnObjectUpdate(object_manager->update_local_player, nullptr);

		global->supported_champions_.emplace_back(std::make_unique<ezreal_t>());
		global->supported_champions_.emplace_back(std::make_unique<karthus_t>());

		global->context()._SdkRegisterOnAIAttack(orbwalker->on_attack, nullptr);
		global->context()._SdkRegisterGameScene(RENDERSCENECALLBACK(on_tick), nullptr);
		global->context()._SdkRegisterOverlayScene(RENDERSCENECALLBACK(menu->draw_menu), nullptr);
		menu->load_settings();
	}, nullptr);
}

auto global_t::context() const -> const SDK_CONTEXT&
{
	return context_;
}

auto global_t::get_champion(uint32_t name) -> champion_t*
{
	for (auto& champ : supported_champions_)
		if (champ->name() == name)
			return champ.get();

	return nullptr;
}

auto global_t::on_tick() -> void
{
	auto& context = global->context();
	orbwalker->orbwalk();

	auto champ = global->get_champion(fnv_rt(object_manager->local_player.object_name));
	if (champ == nullptr)
		return;

	champ->on_tick();
}

std::unique_ptr<global_t> global = nullptr;