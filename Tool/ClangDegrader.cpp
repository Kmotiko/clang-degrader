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
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/system_error.h"

#include "../NullptrDegrade/NullptrDegrader.hpp"

static llvm::cl::OptionCategory DegraderCategory("Degrader-Sample Category");

static llvm::cl::list<std::string>
SourcePaths(llvm::cl::Positional,
            llvm::cl::desc("<source 0> [ ... <sources N>]"),
            llvm::cl::OneOrMore, llvm::cl::cat(DegraderCategory));

static llvm::cl::opt<std::string>
OutputDir("dir",
          llvm::cl::desc("specify the output serealized file directory."),
          llvm::cl::value_desc("dirname"), llvm::cl::init("-"),
          llvm::cl::cat(DegraderCategory));

static llvm::cl::opt<unsigned>
StartLine("start-line", llvm::cl::desc("secify the start line"),
          llvm::cl::init(0), llvm::cl::cat(DegraderCategory));

static llvm::cl::opt<unsigned> EndLine("end-line",
                                       llvm::cl::desc("secify the end line"),
                                       llvm::cl::init(0),
                                       llvm::cl::cat(DegraderCategory));

/**
 * serialize and output TranslationReplacements
 */
void outputReplacementsYAML(
    const clang::tooling::TranslationUnitReplacements &replacements) {
  llvm::outs() << "serealizing : " << replacements.MainSourceFile << "\n";

  // create file path
  llvm::SmallString<128> replacement_file =
      llvm::StringRef(replacements.MainSourceFile + ".yaml");

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
  // get options and set hidden to pre-defined options
  llvm::StringMap<llvm::cl::Option *> Options;
  llvm::cl::getRegisteredOptions(Options);
  for (llvm::StringMap<llvm::cl::Option *>::iterator I = Options.begin(),
                                                     E = Options.end();
       I != E; ++I) {
    if (I->first() == "help" || I->first() == "help-hidden" ||
        I->first() == "version")
      continue;
    if (I->second->Category->getName() != DegraderCategory.getName())
      I->second->setHiddenFlag(llvm::cl::ReallyHidden);
  }

  // create compilation database
  llvm::OwningPtr<clang::tooling::CompilationDatabase> compilations(
      clang::tooling::FixedCompilationDatabase::loadFromCommandLine(argc,
                                                                    argv));

  // re-create compilationdatabase
  if (!compilations) {
    std::string optionstrs[] = { "-std=c++11" };
    compilations.reset(
        new clang::tooling::FixedCompilationDatabase(".", optionstrs));
  }

  // parse commandline options
  llvm::cl::ParseCommandLineOptions(argc, argv);

  if (SourcePaths.size() > 1 && (StartLine != 0 || EndLine != 0)) {
    llvm::errs() << "line option can only be used with one input file\n";
    return 1;
  }

  // create tool handler
  NullptrDegrader nullptr_degrader(StartLine, EndLine);
  nullptr_degrader.exec(*compilations, SourcePaths);

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
