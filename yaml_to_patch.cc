#include "yaml_to_patch.hh"
#include "ryml/ryml_init.hh"
#include "ryml/ryml_serial.hh"

namespace MetaModule
{

bool yaml_raw_to_patch(char *yaml, size_t size, PatchData &pd) {
	RymlInit::init_once();

	ryml::Tree tree = ryml::parse_in_place(ryml::substr(yaml, size));

	if (tree.num_children(0) == 0)
		return false;

	ryml::ConstNodeRef root = tree.rootref();

	if (!root.has_child("PatchData"))
		return false;

	ryml::ConstNodeRef patchdata = root["PatchData"];

	if (!patchdata.has_child("patch_name"))
		return false;

	patchdata["patch_name"] >> pd.patch_name;

	if (patchdata.has_child("description"))
		patchdata["description"] >> pd.description;

	patchdata["module_slugs"] >> pd.module_slugs;
	patchdata["int_cables"] >> pd.int_cables;
	patchdata["mapped_ins"] >> pd.mapped_ins;

	patchdata["mapped_outs"] >> pd.mapped_outs;
	patchdata["static_knobs"] >> pd.static_knobs;
	patchdata["mapped_knobs"] >> pd.knob_sets;

	if (patchdata.has_child("midi_maps"))
		patchdata["midi_maps"] >> pd.midi_maps;

	if (patchdata.has_child("midi_poly_num"))
		patchdata["midi_poly_num"] >> pd.midi_poly_num;

	if (patchdata.has_child("midi_poly_mode")) {
		unsigned x = 0xFF;
		patchdata["midi_poly_mode"] >> x;
		if (x <= 3)
			pd.midi_poly_mode = static_cast<PolyMode>(x);
	}

	if (patchdata.has_child("midi_pitchwheel_range"))
		patchdata["midi_pitchwheel_range"] >> pd.midi_pitchwheel_range;

	if (patchdata.has_child("mapped_lights"))
		patchdata["mapped_lights"] >> pd.mapped_lights;

	if (patchdata.has_child("vcvModuleStates"))
		patchdata["vcvModuleStates"] >> pd.module_states;

	if (patchdata.has_child("suggested_samplerate"))
		patchdata["suggested_samplerate"] >> pd.suggested_samplerate;
	else
		pd.suggested_samplerate = 0;

	if (patchdata.has_child("suggested_blocksize"))
		patchdata["suggested_blocksize"] >> pd.suggested_blocksize;
	else
		pd.suggested_blocksize = 0;

	return true;
}

bool yaml_raw_to_patch(std::span<char> yaml, PatchData &pd) {
	return yaml_raw_to_patch(yaml.data(), yaml.size_bytes(), pd);
}

bool yaml_string_to_patch(std::string yaml, PatchData &pd) {
	return yaml_raw_to_patch(yaml.data(), yaml.size(), pd);
}

} // namespace MetaModule
