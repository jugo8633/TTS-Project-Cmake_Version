
include_directories (
	"include"
	"../../manager/include"
	)

add_library (textinternal
	src/AlbanianNormalizer.cpp
    src/AustrianGermanNormalizer.cpp
    src/Lexicon.cpp
    src/LTSTree.cpp
    src/Normalizer.cpp
	)
