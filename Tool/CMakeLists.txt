set(LLVM_LINK_COMPONENTS support)

set(ClangDegraderSources
    ClangDegrader.cpp
    )

file(GLOB_RECURSE DegraderBaseSources  "../DegraderBase/*.cpp")
list(APPEND ClangDegraderSources ${DegraderBaseSources})

file(GLOB_RECURSE NullptrDegraderSources "../NullptrDegrade/*.cpp")
list(APPEND ClangDegraderSources ${NullptrDegraderSources})

add_clang_executable(clang-degrader
    ${ClangDegraderSources}
)

target_link_libraries(clang-degrader
    clangFormat 
    clangEdit 
    clangTooling 
    clangBasic 
    clangAST 
    clangASTMatchers 
    clangRewriteFrontend
)

install(TARGETS clang-degrader
    DESTINATION bin)
