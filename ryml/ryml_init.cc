#include "ryml.hpp"
#include <cstdio>
#ifdef __cpp_exceptions
#include <stdexcept>
#endif

namespace RymlInit
{

#if (defined(LOG_LEVEL) && (LOG_LEVEL > 2)) || defined(TESTPROJECT)
#define ryml_printf printf
#else
#define ryml_printf(...)
#endif

// ryml >= 0.6 requires error callbacks to not return: throw and let the
// parse/emit entry points (yaml_raw_to_patch etc.) catch and report failure.
[[noreturn]] static void throw_error() {
#ifdef __cpp_exceptions
	throw std::runtime_error("ryml error");
#endif
}

void init_once() {
	static bool already_init = false;
	static c4::yml::Callbacks callbacks;
	if (!already_init) {
		already_init = true;

		callbacks.m_error_basic = [](c4::csubstr msg, c4::yml::ErrorDataBasic const &errdata, void *) {
			ryml_printf("[ryml] %.*s (%s %zu:%zu)\n",
						(int)msg.len,
						msg.str,
						errdata.location.name.empty() ? "" : errdata.location.name.data(),
						errdata.location.line,
						errdata.location.col);
			throw_error();
		};
		callbacks.m_error_parse = [](c4::csubstr msg, c4::yml::ErrorDataParse const &errdata, void *) {
			ryml_printf(
				"[ryml] parse error: %.*s (%zu:%zu)\n", (int)msg.len, msg.str, errdata.ymlloc.line, errdata.ymlloc.col);
			throw_error();
		};
		callbacks.m_error_visit = [](c4::csubstr msg, c4::yml::ErrorDataVisit const &, void *) {
			ryml_printf("[ryml] visit error: %.*s\n", (int)msg.len, msg.str);
			throw_error();
		};

		c4::yml::set_callbacks(callbacks);
		c4::set_error_flags(c4::ON_ERROR_DEBUGBREAK | c4::ON_ERROR_LOG);
	}
}
} // namespace RymlInit
