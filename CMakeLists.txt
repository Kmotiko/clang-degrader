set(LLVM_LINK_COMPONENTS support)

add_clang_executable(sample-degrader
    SampleDegrader.cpp
)

target_link_libraries(sample-degrader
    clangFormat 
    clangTooling 
    clangBasic 
    clangASTMatchers 
    clangRewriteFrontend
)

install(TARGETS sample-degrader
    DESTINATION bin)
