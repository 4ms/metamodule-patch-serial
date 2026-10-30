#include "doctest.h"
#include "patch/panels/panel_medium_defs.hh"
#include "patch/panels/panel_pro_defs.hh"

// A patch's mappings mean the same on either panel
TEST_CASE("The medium and pro panels share knob and jack IDs") {
	// The medium's 12 knobs are the pro's ENC1..12 (by ID: the panels name them differently)
	CHECK(MediumPanelDef::NumKnobs == 12);
	CHECK(ProPanelDef::NumKnobs == 16);
	for (unsigned id = 0; id < MediumPanelDef::NumKnobs; id++)
		CHECK(ProPanelDef::get_map_param_name(id) != "?");

	// The medium has no knob 13 (the pro's ENC13)
	CHECK(MediumPanelDef::get_map_param_name(12) == "?");
	CHECK(ProPanelDef::get_map_param_name(12) != "?");

	for (unsigned id = 0; id < MediumPanelDef::NumUserFacingInJacks; id++)
		CHECK(MediumPanelDef::get_map_injack_name(id) == ProPanelDef::get_map_injack_name(id));
	for (unsigned id = 0; id < MediumPanelDef::NumUserFacingOutJacks; id++)
		CHECK(MediumPanelDef::get_map_outjack_name(id) == ProPanelDef::get_map_outjack_name(id));

	CHECK(std::string_view{MediumPanelDef::typeID} == std::string_view{ProPanelDef::typeID});
}

TEST_CASE("Audio Expander jack IDs follow the medium panel's jacks") {
	using namespace MetaModule;
	CHECK(AudioExpander::get_map_injack_num(0) == 8);
	CHECK(AudioExpander::get_map_outjack_num(0) == 8);
	CHECK(AudioExpander::is_expander_input(13));
	CHECK_FALSE(AudioExpander::is_expander_input(14));
	CHECK_FALSE(AudioExpander::is_expander_input(MediumPanelDef::NumUserFacingInJacks - 1));
}
