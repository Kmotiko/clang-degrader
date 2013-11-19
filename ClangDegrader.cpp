#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/ReplacementsYaml.h"
#include "clang/Tooling/Refactoring.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/YAMLTraits.h"

const char NullptrExpr[] = "nullptr_expr";

/**
 * MatchCallback for NullptrDegrader
 */
class NullptrFixer : public clang::ast_matchers::MatchFinder::MatchCallback {
private:
  clang::tooling::Replacements &replacements;

public:
  NullptrFixer(clang::tooling::Replacements &replacements)
      : replacements(replacements) {}
  virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);
};

/**
 * run method for NullptrDegrader
 */
void
NullptrFixer::run(const clang::ast_matchers::MatchFinder::MatchResult &result) {
  llvm::outs() << "match nullptr\n";

  // cast to NullPtrLteralExpr
  clang::ASTContext *context = result.Context;
  const clang::ast_matchers::BoundNodes nodes = result.Nodes;
  const clang::CXXNullPtrLiteralExpr *nullptr_expr =
      nodes.getNodeAs<clang::CXXNullPtrLiteralExpr>(NullptrExpr);
  if (nullptr_expr == NULL)
    return;

  clang::SourceRange range(nullptr_expr->getLocStart(),
                           nullptr_expr->getLocEnd());

  std::string text = "NULL";
  replacements.insert(clang::tooling::Replacement(
      context->getSourceManager(), clang::CharSourceRange::getTokenRange(range),
      text));
}

/**
 * main
 */
int main(int argc, const char **argv) {
  clang::tooling::CommonOptionsParser opt_parser(argc, argv);
  clang::tooling::RefactoringTool tool(opt_parser.getCompilations(),
                                       opt_parser.getSourcePathList());

  // create Finder and Matcher
  clang::ast_matchers::MatchFinder finder;
  clang::ast_matchers::StatementMatcher matcher =
      clang::ast_matchers::nullPtrLiteralExpr().bind(NullptrExpr);
  NullptrFixer fixer(tool.getReplacements());
  finder.addMatcher(matcher, &fixer);

  // run and save
  return tool.runAndSave(clang::tooling::newFrontendActionFactory(&finder));
}
