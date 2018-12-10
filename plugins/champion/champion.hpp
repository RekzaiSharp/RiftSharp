#pragma once
#include <cstdint>

class champion_t
{
public:
	explicit champion_t(uint32_t name);
	virtual ~champion_t() {}

	virtual auto on_tick() -> void;
	virtual auto menu() -> void;
	auto name() const -> uint32_t;

protected:
	uint32_t name_;
};
