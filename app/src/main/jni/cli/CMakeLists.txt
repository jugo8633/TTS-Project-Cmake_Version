
include_directories (
	"../engine/text/internal/include"
	"../engine/manager/include"
	)

add_executable (cli src/htstts.cpp)
target_link_libraries (cli manager)
