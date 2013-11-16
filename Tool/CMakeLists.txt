set(LLVM_LINK_COMPONENTS support)

set(SampleDegraderSources
    SampleDegrader.cpp
    )

file(GLOB_RECURSE DegraderBaseSources  "../DegraderBase/*.cpp")
list(APPEND SampleDegraderSources ${DegraderBaseSources})

file(GLOB_RECURSE NullptrDegraderSources "../NullptrDegrade/*.cpp")
list(APPEND SampleDegraderSources ${NullptrDegraderSources})

add_clang_executable(sample-tuyaml
    ${SampleDegraderSources}
)

target_link_libraries(sample-tuyaml
    clangFormat 
    clangEdit 
    clangTooling 
    clangBasic 
    clangAST 
    clangASTMatchers 
    clangRewriteFrontend
)

install(TARGETS sample-tuyaml
    DESTINATION bin)
