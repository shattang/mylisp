# Create the MyLispRunner executable
add_executable(MyLispRunner 
    main.cpp
)

target_link_libraries(MyLispRunner MyLisp)
