CC=clang
PROJECT_DIR = .
CORE_DIR = $(PROJECT_DIR)
OBJ_DIR = $(PROJECT_DIR)/obj
BIN_DIR = $(PROJECT_DIR)/bin
LIBS=\
	-lclangFrontend	\
	-lclangParse	\
	-lclangSema		\
	-lclangAnalysis	\
	-lclangAST		\
	-lclangBasic	\
	-lclangDriver	\
	-lclangSerialization	\
	-lclangTooling	\
	-lclangIndex	\
	-lclangEdit		\
	-lclangLex		\
	-lclangFormat	\
	-lclangRewriteCore	\
	-lclangRewriteFrontend	\
	-lclangASTMatchers	\
	-lclangFrontendTool	\
	-lclangDynamicASTMatchers	\
	-lclangApplyReplacements \

SAMPLE_DIR = $(PROJECT_DIR)/sample

MAIN_SRC = ClangDegrader.cpp
MAIN_SRC_PATH = $(CORE_DIR)/$(MAIN_SRC)
MAIN_OBJ = $(OBJ_DIR)/$(MAIN_SRC:.cpp=.o)

TOOL = $(BIN_DIR)/clang-degrader
CONFIG = llvm-config
LFLAGS = --ldflags --libs
CFLAGS = --cxxflags


all:$(MAIN_OBJ) $(NULLPTR_OBJ) $(BASE_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) -g $(MAIN_OBJ) $(LIBS) `$(CONFIG) $(LFLAGS)` -static -ltinfo -lncurses -lstdc++ -lpthread -lm -ldl -o $(TOOL) 

$(MAIN_OBJ):$(MAIN_SRC_PATH)
	mkdir -p $(OBJ_DIR)
	$(CC) -g $(MAIN_SRC_PATH) `$(CONFIG) $(CFLAGS)` -fno-rtti -c -o $(MAIN_OBJ) 

clean:
	rm -rf $(MAIN_OBJ) $(TOOL)

run:
	$(TOOL) ./sample/sample.cpp -- -std=c++11
