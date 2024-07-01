FILE(GLOB AllShaderSources *.sc)

set(SHADERC_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/../../_deps/bgfx-build/cmake/bgfx/shaderc.exe")

foreach(shader_source_file ${AllShaderSources})
	get_filename_component(shader_name ${shader_source_file} NAME_WE)
	set(shader_output_file_bin "${CMAKE_CURRENT_BINARY_DIR}/${shader_name}.bin")
	set(shader_output_file_header "${CMAKE_CURRENT_SOURCE_DIR}/${shader_name}_bin.h")
	set(all_shader_output_file_header "${CMAKE_CURRENT_SOURCE_DIR}/shaders.h")
	string(SUBSTRING ${shader_name} 0 1 shader_type)
#	message(STATUS  "${shader_output_file_bin}")
#	message(STATUS "${shader_output_file_header}")
	add_custom_command(
		OUTPUT ${shader_output_file_bin}
		COMMAND ${SHADERC_COMMAND} -f ${shader_source_file} -o ${shader_output_file_bin} --type ${shader_type} --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/varying/standard.def.sc -p s_5_0
		DEPENDS ${shader_source_file}
		COMMENT "Compiling shader ${shader_name}"
	)
	add_custom_command(
		OUTPUT ${shader_output_file_header}
		COMMAND ${SHADERC_COMMAND} -f ${shader_source_file} -o ${shader_output_file_header} --bin2c ${shader_name} --type ${shader_type} --varyingdef ${CMAKE_CURRENT_SOURCE_DIR}/varying/standard.def.sc -p s_5_0
		DEPENDS ${shader_source_file}
		COMMENT "Compiling shader header ${shader_name}"
	)
	list(APPEND AllShaderOutputsBin ${shader_output_file_bin})
	list(APPEND AllShaderHeaders ${shader_output_file_header})
endforeach()

 add_custom_target(compile_bgfx_shaders DEPENDS ${AllShaderOutputsBin} ${AllShaderHeaders})