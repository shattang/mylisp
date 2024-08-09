# Create the MyLisp library
add_library(MyLisp STATIC
	Lexer.cpp
    	Parser.cpp
)

# Target properties for MyLisp
target_include_directories(MyLisp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
