find_package( minute_calculator ${SEARCH_PARAMETERS} COMPONENTS
	minute_calculator
)

find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries minute_calculator Boost )

