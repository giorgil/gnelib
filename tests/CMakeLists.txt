FILE( GLOB TEST_SRCS *.cpp )
SET_SOURCE_FILES_PROPERTIES(
    ${TEST_SRCS} PROPERTIES COMPILE_FLAGS "${GNE_COMMON_FLAGS}" )

ADD_EXECUTABLE( gne_tests ${TEST_SRCS} )
SET_TARGET_PROPERTIES(
    gne_tests PROPERTIES LINK_FLAGS "${GNE_LINKER_FLAGS}" )
ADD_DEPENDENCIES( gne_tests gnelib )
TARGET_LINK_LIBRARIES( gne_tests gnelib )
