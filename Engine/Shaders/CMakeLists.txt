find_program(FXC fxc DOC "FX shader compiler" REQUIRED)

set(VS_SHADER_SRCS
	"test_VS.hlsl"
)

set(PS_SHADER_SRCS
	"test_PS.hlsl"
)

function(compile_shader ownerTarget shaderFileName shaderProfile shaderEntryPoint)
	get_filename_component(fileName ${shaderFileName} NAME_WE)
	add_custom_command(
		TARGET ${ownerTarget}
		POST_BUILD
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${shaderFileName}
		COMMAND ${FXC} /T "${shaderProfile}" /E "${shaderEntryPoint}" /Od /Zi /nologo /Fo "${CMAKE_BINARY_DIR}/Shaders/${fileName}.cso" ${CMAKE_CURRENT_SOURCE_DIR}/${shaderFileName}
		BYPRODUCTS ${CMAKE_BINARY_DIR}/Shaders/${fileName}.cso
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endfunction()

add_custom_target(HowliteShaders DEPENDS ${VS_SHADER_SRCS} ${PS_SHADER_SRCS})

foreach(VS_SRC ${VS_SHADER_SRCS})
	compile_shader(HowliteShaders ${VS_SRC} "vs_5_0" "main")
endforeach()

foreach(PS_SRC ${PS_SHADER_SRCS})
	compile_shader(HowliteShaders ${PS_SRC} "ps_5_0" "main")
endforeach()