#include "ezreal.hpp"
#include "../common/orbwalker.hpp"
#include <filesystem>
#include "../../../libary/fnv.hpp"
#include "../../../common/target_selector.hpp"
#include "../../../common/prediction.hpp"

ezreal_t::ezreal_t() : champion_t(fnv("Ezreal")) {}
auto ezreal_t::on_tick() -> void
{
	spell_mysticshot();
}

auto ezreal_t::spell_mysticshot() -> void
{

}
