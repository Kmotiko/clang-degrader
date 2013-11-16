#include "NullptrDegrader.hpp"

extern const char NullptrExpr[];




/**
 * execute ClangTool
 */
bool NullptrDegrader::exec(
        clang::tooling::CompilationDatabase &compilations, 
        std::vector<std::string> sources){

    // create ClangTool
    llvm::outs() << "execute NullptrDegrader\n";
    clang::tooling::ClangTool tool(compilations, sources);

    //create finder and matcher
    clang::ast_matchers::MatchFinder finder;
    clang::ast_matchers::StatementMatcher matcher = 
        clang::ast_matchers::nullPtrLiteralExpr().bind(NullptrExpr);
    NullptrFixer fixer(*this);
    finder.addMatcher(matcher, &fixer);

    return tool.run(clang::tooling::newFrontendActionFactory(&finder, this));
}


/**
 * Call back each match
 */
void NullptrFixer::run(const clang::ast_matchers::MatchFinder::MatchResult &result) {
    // cast to NullPtrLiteralExpr
    const clang::ast_matchers::BoundNodes nodes = result.Nodes;
    const clang::CXXNullPtrLiteralExpr *nullptr_expr = 
        nodes.getNodeAs<clang::CXXNullPtrLiteralExpr>(NullptrExpr);
    if(nullptr_expr==NULL)
        return;

    // get context
    clang::ASTContext *context = result.Context;

    // add replacement
    std::string text = "NULL";
    Degrader.addReplacement(clang::tooling::Replacement(
        context->getSourceManager(), nullptr_expr, text));
}
