find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	date_time
	system
	thread
 	filesystem
	date_time
 	chrono
 	regex
	unit_test_framework
)

compile_project( ${tests_name} "*.cpp" "*.h" BINARY tests ${module_name} Boost )
register_test( ${tests_name} 5 4 )

