#include "global.hpp"
#include "../common/menu.hpp"

global_t::global_t(SDK_CONTEXT& context) : context_(context)
{
	context_._SdkRegisterOnLoad([](void*) -> void
	{
		object_manager->get_local_player();
		global->context()._SdkRegisterOnObjectUpdate(object_manager->update_local_player, nullptr);
		global->context()._SdkRegisterOnAIAttack(orbwalker->on_attack, nullptr);
		global->context()._SdkRegisterGameScene(RENDERSCENECALLBACK(orbwalker->orbwalk), nullptr);
		global->context()._SdkRegisterOverlayScene(RENDERSCENECALLBACK(menu->draw_menu), nullptr);
		menu->load_settings();
	}, nullptr);
}

auto global_t::context() const -> const SDK_CONTEXT&
{
	return context_;
}

std::unique_ptr<global_t> global = nullptr;