#include "ryml.hpp"
#include <cstdio>

namespace RymlInit
{

#if defined(LOG_LEVEL) && (LOG_LEVEL > 2)
#define ryml_printf printf
#else
#define ryml_printf(...)
#endif

void init_once() {
	static bool already_init = false;
	static c4::yml::Callbacks callbacks;
	if (!already_init) {
		already_init = true;
		callbacks.m_error = [](const char *msg, size_t /*msg_len*/, c4::yml::Location loc, void * /*user_data*/) {
			if (loc.name.empty())
				ryml_printf("[ryml] %s\n", msg);
			else
				ryml_printf("[ryml] %s in %s %zu:%zu)\n", msg, loc.name.data(), loc.line, loc.col);
		};
		c4::yml::set_callbacks(callbacks);
		c4::set_error_flags(c4::ON_ERROR_DEBUGBREAK | c4::ON_ERROR_LOG);
	}
}
} // namespace RymlInit
