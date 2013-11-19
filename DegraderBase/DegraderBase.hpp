#ifndef __DEGRADER_BASE_HPP__
#define __DEGRADER_BASE_HPP__

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"
#include <map>

/**
 * Base of Degrader extends SourceFileCallbacks
 *
 */
class DegraderBase : public clang::tooling::SourceFileCallbacks {
public:
  DegraderBase() {};
  virtual ~DegraderBase() {};

  /**
   * SourceFileCallbacksのvirtualメソッド
   * FrontendActionがファイルを処理する際に呼び出される.
   */
  virtual bool handleBeginSource(clang::CompilerInstance &CI,
                                 llvm::StringRef Filename);

  /**
   * Replacement をTUReplacementsMapに追加する
   */
  bool addReplacement(const clang::tooling::Replacement &replacement);

  /**
   * TUReplacementsMapに返却する
   */
  std::map<std::string, clang::tooling::TranslationUnitReplacements>
  getTUReplacementsMap() {
    return this->TUReplacementsMap;
  }

  /**
   * ClangToolを実行するvirtualメソッド
   */
  virtual bool exec(clang::tooling::CompilationDatabase &compilations,
                    std::vector<std::string> sources) = 0;

protected:
  /**
   * map of TranslationUnitReplacements
   * Key is Filename, Value is TranslationUnitReplacements
   */
  std::map<std::string, clang::tooling::TranslationUnitReplacements>
  TUReplacementsMap;

  /**
   * store file name when handleBeginSource is called
   */
  std::string CurrentSource;
};

#endif
