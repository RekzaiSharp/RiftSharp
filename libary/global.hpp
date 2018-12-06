#pragma once
#include <memory>
#include "sdkapi.hpp"
#include "../common/object_manager.hpp"
#include "../common/orbwalker.hpp"

class global_t
{
public:
	explicit global_t(SDK_CONTEXT& context);
	auto context() const -> const SDK_CONTEXT&;

private:
	SDK_CONTEXT context_;
};

extern std::unique_ptr<global_t> global;