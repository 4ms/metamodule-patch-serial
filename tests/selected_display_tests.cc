#include "doctest.h"
#include "patch_to_yaml.hh"
#include "yaml_to_patch.hh"
#include <string>

TEST_CASE("selected_displays round-trip") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B"},
	};
	pd.patch_name = "selected_display_test";
	pd.set_selected_display(2, 5);

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("selected_displays:") != std::string::npos);

	MetaModule::PatchData pd2;
	bool ok = yaml_string_to_patch(yaml, pd2);
	CHECK(ok);
	REQUIRE(pd2.selected_displays.size() == 1);
	CHECK(pd2.selected_displays[0].module_id == 2);
	CHECK(pd2.selected_displays[0].light_id == 5);
}

TEST_CASE("selected_displays not written when empty") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "VCF"},
	};
	pd.patch_name = "empty_field_test";

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("selected_displays") == std::string::npos);
}

TEST_CASE("selected_displays missing field") {
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
	CHECK(pd.selected_displays.empty());
	CHECK_FALSE(pd.find_selected_display().has_value());
}

TEST_CASE("set_selected_display replaces the previous one") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.set_selected_display(1, 0);
	pd.set_selected_display(3, 2);

	CHECK(pd.selected_displays.size() == 1);
	CHECK(pd.is_selected_display(3, 2));
	CHECK_FALSE(pd.is_selected_display(1, 0));
	REQUIRE(pd.find_selected_display().has_value());
	CHECK(pd.find_selected_display()->module_id == 3);

	pd.clear_selected_display();
	CHECK(pd.selected_displays.empty());
}

TEST_CASE("remove_module shifts and erases selected displays") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.set_selected_display(3, 1);

	pd.remove_module(1);
	REQUIRE(pd.selected_displays.size() == 1);
	CHECK(pd.selected_displays[0].module_id == 2);
	CHECK(pd.selected_displays[0].light_id == 1);

	pd.remove_module(2);
	CHECK(pd.selected_displays.empty());
}

TEST_CASE("blank_out_module erases its selected display") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B"},
	};
	pd.set_selected_display(1, 0);
	pd.blank_out_module(2);
	CHECK(pd.selected_displays.size() == 1);

	pd.blank_out_module(1);
	CHECK(pd.selected_displays.empty());
}

TEST_CASE("selected display by position round-trip") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B"},
	};
	pd.patch_name = "selected_display_pos_test";
	pd.set_selected_display_at(2, 12.5f, 30.25f);

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("x_mm:") != std::string::npos);
	CHECK(yaml.find("light_id") == std::string::npos);

	MetaModule::PatchData pd2;
	CHECK(yaml_string_to_patch(yaml, pd2));
	REQUIRE(pd2.selected_displays.size() == 1);
	CHECK(pd2.selected_displays[0].module_id == 2);
	REQUIRE(pd2.selected_displays[0].position_mm.has_value());
	CHECK(pd2.selected_displays[0].position_mm->first == doctest::Approx(12.5f));
	CHECK(pd2.selected_displays[0].position_mm->second == doctest::Approx(30.25f));
}

TEST_CASE("selected display by position doesn't match a light id") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A"},
	};
	pd.set_selected_display_at(1, 0.f, 0.f);
	CHECK_FALSE(pd.is_selected_display(1, 0));

	// Setting by light id clears the position
	pd.set_selected_display(1, 3);
	REQUIRE(pd.selected_displays.size() == 1);
	CHECK_FALSE(pd.selected_displays[0].position_mm.has_value());
	CHECK(pd.is_selected_display(1, 3));
}

TEST_CASE("selected display entry needs a light_id or a position") {
	std::string yaml = R"(PatchData:
  patch_name: bad_entry
  module_slugs:
    0: HubMedium
    1: VCF
  selected_displays:
    - module_id: 1
)";

	MetaModule::PatchData pd;
	yaml_string_to_patch(yaml, pd);
	CHECK(pd.selected_displays.empty());
}
