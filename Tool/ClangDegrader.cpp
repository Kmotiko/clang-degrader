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
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/system_error.h"

#include "../NullptrDegrade/NullptrDegrader.hpp"

/**
 * serialize and output TranslationReplacements
 */
void outputReplacementsYAML(
    const clang::tooling::TranslationUnitReplacements &replacements) {
  llvm::outs() << "serealizing : " << replacements.MainSourceFile << "\n";

  // create file path
  llvm::SmallString<128> replacement_file =
      llvm::StringRef(replacements.MainSourceFile + ".yaml");
  llvm::outs() << replacement_file << "\n";

  if (replacements.Replacements.size() == 0)
    return;

  // fd stream
  std::string errinfo;
  llvm::raw_fd_ostream file_stream(replacement_file.c_str(), errinfo,
                                   llvm::sys::fs::F_Binary);

  llvm::yaml::Output yaml_out(file_stream);
  yaml_out << const_cast<clang::tooling::TranslationUnitReplacements &>(
                  replacements);

  return;
}

int main(int argc, const char **argv) {

  // parse command line
  clang::tooling::CommonOptionsParser opt_parser(argc, argv);

  // create tool handler
  NullptrDegrader nullptr_degrader;
  nullptr_degrader.exec(opt_parser.getCompilations(),
                        opt_parser.getSourcePathList());

  // serealize
  std::map<std::string, clang::tooling::TranslationUnitReplacements>
  tu_replacements_map = nullptr_degrader.getTUReplacementsMap();
  for (std::map<std::string,
                clang::tooling::TranslationUnitReplacements>::iterator
           it = tu_replacements_map.begin(),
           end = tu_replacements_map.end();
       it != end; it++) {
    outputReplacementsYAML(it->second);
  }

  return 0;
}
