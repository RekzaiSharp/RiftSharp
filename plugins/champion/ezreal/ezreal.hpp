#pragma once
#include "../champion.hpp"

class ezreal_t : public champion_t
{
public:
	ezreal_t();
	virtual auto on_tick() -> void;

	auto spell_mysticshot() -> void;
};