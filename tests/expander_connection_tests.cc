#include "doctest.h"
#include "patch_to_yaml.hh"
#include "yaml_to_patch.hh"
#include <string>

TEST_CASE("expanders round-trip") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "Mix4", "Mix4x", "VCF"},
	};
	pd.patch_name = "expander_test";
	pd.expanders.push_back({.left_module_id = 1, .right_module_id = 2});
	pd.expanders.push_back({.left_module_id = 2, .right_module_id = 3});

	auto yaml = patch_to_yaml_string(pd);

	MetaModule::PatchData pd2;
	bool ok = yaml_string_to_patch(yaml, pd2);
	CHECK(ok);
	REQUIRE(pd2.expanders.size() == 2);
	CHECK(pd2.expanders[0].left_module_id == 1);
	CHECK(pd2.expanders[0].right_module_id == 2);
	CHECK(pd2.expanders[1].left_module_id == 2);
	CHECK(pd2.expanders[1].right_module_id == 3);
}

TEST_CASE("expanders YAML output format") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "Mix4", "Mix4x"},
	};
	pd.patch_name = "expander_yaml_test";
	pd.expanders.push_back({.left_module_id = 1, .right_module_id = 2});

	auto yaml = patch_to_yaml_string(pd);

	CHECK(yaml.find("expanders:") != std::string::npos);
	CHECK(yaml.find("left_module_id: 1") != std::string::npos);
	CHECK(yaml.find("right_module_id: 2") != std::string::npos);
}

TEST_CASE("expanders not written when empty") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "VCF"},
	};
	pd.patch_name = "empty_field_test";

	auto yaml = patch_to_yaml_string(pd);
	CHECK(yaml.find("expanders") == std::string::npos);
}

TEST_CASE("expanders backward compat - missing field") {
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
	CHECK(pd.expanders.empty());
}

TEST_CASE("blank_out_module removes its expander connections") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.expanders.push_back({.left_module_id = 1, .right_module_id = 2});
	pd.expanders.push_back({.left_module_id = 2, .right_module_id = 3});

	pd.blank_out_module(2);

	CHECK(pd.expanders.empty());
}

TEST_CASE("blank_out_module keeps unrelated expander connections") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.expanders.push_back({.left_module_id = 2, .right_module_id = 3});

	pd.blank_out_module(1);

	REQUIRE(pd.expanders.size() == 1);
	CHECK(pd.expanders[0].left_module_id == 2);
	CHECK(pd.expanders[0].right_module_id == 3);
}

TEST_CASE("remove_module removes and re-indexes expander connections") {
	MetaModule::PatchData pd{
		.module_slugs{"HubMedium", "A", "B", "C"},
	};
	pd.expanders.push_back({.left_module_id = 1, .right_module_id = 2});
	pd.expanders.push_back({.left_module_id = 2, .right_module_id = 3});

	pd.remove_module(1);

	// {1,2} is gone (module 1 removed); {2,3} became {1,2}
	REQUIRE(pd.expanders.size() == 1);
	CHECK(pd.expanders[0].left_module_id == 1);
	CHECK(pd.expanders[0].right_module_id == 2);
}
