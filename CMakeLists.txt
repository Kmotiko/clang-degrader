set(LLVM_LINK_COMPONENTS support)

add_clang_executable(clang-degrader
    ClangDegrader.cpp
)

target_link_libraries(clang-degrader
    clangFormat 
    clangTooling 
    clangBasic 
    clangASTMatchers 
    clangRewriteFrontend
)

install(TARGETS clang-degrader
    DESTINATION bin)
