#include "doctest.h"
#include "patch_to_yaml.hh"
#include "yaml_to_patch.hh"
#include <string>

TEST_CASE("screensaver_displays round-trip") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B"},
	};
	pd.patch_name = "screensaver_test";
	pd.set_screensaver_display(2, 5);

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("screensaver_displays:") != std::string::npos);

	MetaModule::PatchData pd2;
	bool ok = yaml_string_to_patch(yaml, pd2);
	CHECK(ok);
	REQUIRE(pd2.screensaver_displays.size() == 1);
	CHECK(pd2.screensaver_displays[0].module_id == 2);
	CHECK(pd2.screensaver_displays[0].light_id == 5);
}

TEST_CASE("screensaver_displays not written when empty") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "VCF"},
	};
	pd.patch_name = "empty_field_test";

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("screensaver_displays") == std::string::npos);
}

TEST_CASE("screensaver_displays missing field") {
	std::string yaml = R"(PatchData:
  patch_name: old_patch
  module_slugs:
    0: HubMedium
    1: VCF
  int_cables: []
  mapped_ins: []
  mapped_outs: []
  static_knobs: []
  mapped_knobs: []
  midi_maps:
    name: ''
    set: []
)";

	MetaModule::PatchData pd;
	bool ok = yaml_string_to_patch(yaml, pd);
	CHECK(ok);
	CHECK(pd.screensaver_displays.empty());
	CHECK_FALSE(pd.find_screensaver_display().has_value());
}

TEST_CASE("set_screensaver_display replaces the previous one") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.set_screensaver_display(1, 0);
	pd.set_screensaver_display(3, 2);

	CHECK(pd.screensaver_displays.size() == 1);
	CHECK(pd.is_screensaver_display(3, 2));
	CHECK_FALSE(pd.is_screensaver_display(1, 0));
	REQUIRE(pd.find_screensaver_display().has_value());
	CHECK(pd.find_screensaver_display()->module_id == 3);

	pd.clear_screensaver_display();
	CHECK(pd.screensaver_displays.empty());
}

TEST_CASE("remove_module shifts and erases screensaver displays") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.set_screensaver_display(3, 1);

	pd.remove_module(1);
	REQUIRE(pd.screensaver_displays.size() == 1);
	CHECK(pd.screensaver_displays[0].module_id == 2);
	CHECK(pd.screensaver_displays[0].light_id == 1);

	pd.remove_module(2);
	CHECK(pd.screensaver_displays.empty());
}

TEST_CASE("blank_out_module erases its screensaver display") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B"},
	};
	pd.set_screensaver_display(1, 0);
	pd.blank_out_module(2);
	CHECK(pd.screensaver_displays.size() == 1);

	pd.blank_out_module(1);
	CHECK(pd.screensaver_displays.empty());
}
