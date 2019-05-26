


include_directories (
	"include"
	"../text/internal/include"
	"..//text/flite/include"
	"../synthesis/hts_engine/include"
	)

add_library (manager
   src/FliteTextAnalyzer.cpp
   src/HTSEngineSynthesizer.cpp
   src/InternalTextAnalyzer.cpp
   src/Label.cpp
   src/Synthesizer.cpp
   src/Text.cpp
   src/TextAnalyzer.cpp
   src/TTSLogger.cpp
   src/TTSManager.cpp
   src/utils.cpp
)

target_link_libraries (manager flite hts_engine textinternal)
