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
    NullptrFixer fixer(*this, StartLine, EndLine);
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

    // get Location
    clang::SourceLocation start_loc = nullptr_expr->getLocStart();
    clang::SourceLocation end_loc = nullptr_expr->getLocEnd();

    // get sourcemanager
    const clang::SourceManager &source_manager = context->getSourceManager();

    // get FileID
    clang::FileID file_id = source_manager.getMainFileID();

    // get translate location
    clang::SourceLocation translate_start;
    clang::SourceLocation translate_end;

    // get translate location(start)
    if(StartLine != 0)
        translate_start = source_manager.translateLineCol(file_id, StartLine, 0);
    else 
        translate_start = source_manager.getLocForStartOfFile(file_id);

    // get translate location(end)
    if(EndLine != 0)
        translate_end = source_manager.translateLineCol(file_id, EndLine, 0);
    else 
        translate_end = source_manager.getLocForEndOfFile(file_id);

    // check NullptrLiteralExpr in the StartLine - EndLine
    if( source_manager.isBeforeInTranslationUnit(translate_start, start_loc) &&
        source_manager.isBeforeInTranslationUnit(end_loc, translate_end)){
        // add replacement
        std::string text = "NULL";
        Degrader.addReplacement(
            clang::tooling::Replacement( 
                context->getSourceManager(),
                nullptr_expr,
                text)
        );
    }
}
