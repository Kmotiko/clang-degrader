#ifndef __NULLPTRDEGRADER_HPP__
#define __NULLPTRDEGRADER_HPP__

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"
#include "llvm/ADT/SmallString.h"

#include "../DegraderBase/DegraderBase.hpp"

const char NullptrExpr[] = "nullptr_expr";

/**
 * Handler of ClangTool for Nullptr Degrader
 *
 */
class NullptrDegrader : public DegraderBase {
public:
  NullptrDegrader(unsigned start_line = 0, unsigned end_line = 0)
      : StartLine(start_line), EndLine(end_line) {}
  ~NullptrDegrader() {}
  virtual bool exec(clang::tooling::CompilationDatabase &compilations,
                    std::vector<std::string> sources);

private:
  unsigned StartLine; // StartLine of translate
  unsigned EndLine;   // EndLien of translate
};

/**
 * MatchCallback for NullptrDegrader
 */
class NullptrFixer : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  NullptrFixer(NullptrDegrader &degrader, unsigned start_line = 0,
               unsigned end_line = 0)
      : Degrader(degrader), StartLine(start_line), EndLine(end_line) {}
  virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &result);

private:
  NullptrDegrader &Degrader;
  unsigned StartLine;
  unsigned EndLine;
};

#endif
