cmake_minimum_required (VERSION 3.21)

project (SimCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zi)
    add_link_options(/DEBUG)
endif()

add_library(simcore SHARED
	src/api/simcore_types.cpp
	src/api/simcore_api.cpp
    src/core/entity.cpp
	src/core/components/component.cpp
	src/core/components/health.cpp
	src/core/units/unit.cpp
    src/core/units/ugv.cpp
	src/core/world.cpp
)

set_target_properties(simcore PROPERTIES
    DEBUG_POSTFIX "d"
)
target_include_directories(simcore
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include/simcore
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(simcore PRIVATE SIMCORE_EXPORT)

add_executable (simcore_cli 
				cli/cli_main.cpp
)

target_link_libraries(simcore_cli PRIVATE simcore)
