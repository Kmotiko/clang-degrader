#include "DegraderBase.hpp"

/**
 * called before a source file is processed
 */
bool DegraderBase::handleBeginSource(clang::CompilerInstance &CI,
                                     llvm::StringRef Filename) {
  CurrentSource = Filename;
  llvm::outs() << "set current source : " << CurrentSource << "\n";
  return true;
}

/**
 * add Replacement to TlanslationUnitReplacements
 */
bool
DegraderBase::addReplacement(const clang::tooling::Replacement &replacement) {
  clang::tooling::TranslationUnitReplacements &tu_replacements =
      TUReplacementsMap[CurrentSource];
  tu_replacements.Replacements.push_back(replacement);
  if (tu_replacements.MainSourceFile.empty())
    tu_replacements.MainSourceFile = CurrentSource;
  return true;
}
