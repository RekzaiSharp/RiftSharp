#include "ezreal.hpp"
#include "../common/orbwalker.hpp"
#include <filesystem>
#include "../../../libary/fnv.hpp"

ezreal_t::ezreal_t() : champion_t(fnv("Ezreal")) {}
auto ezreal_t::on_tick() -> void
{
	global->context()._SdkConsoleWrite("ezreal ontick");
}