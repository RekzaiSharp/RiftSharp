#include "champion.hpp"
#include "../../libary/global.hpp"
// haben champs alle die gleichen functions Q W E R 

champion_t::champion_t(uint32_t name) : name_(name) {}

auto champion_t::on_tick() -> void
{
	
}

auto champion_t::menu() -> void
{
	global->context()._SdkConsoleWrite("[RiftSharp] Champion not supported");
}


auto champion_t::name() const -> uint32_t
{
	return name_;
}
