project(metamodule-patch-serial CXX)

add_library(metamodule-patch-serial STATIC)
add_library(metamodule::patch-serial ALIAS metamodule-patch-serial)

target_compile_features(metamodule-patch-serial PUBLIC cxx_std_20)

target_include_directories(metamodule-patch-serial PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}
	${CMAKE_CURRENT_LIST_DIR}/ryml
	${CMAKE_CURRENT_LIST_DIR}/ryml/rapidyaml/src
	${CMAKE_CURRENT_LIST_DIR}/ryml/rapidyaml/ext/c4core/src	
)

target_sources(metamodule-patch-serial PRIVATE
	yaml_to_patch.cc
	patch_to_yaml.cc
	ryml/ryml_init.cc
	ryml/ryml_serial.cc
)

add_subdirectory(ryml/rapidyaml)

target_link_libraries(metamodule-patch-serial PUBLIC ryml cpputil)

