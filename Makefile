CC=clang++
PROJECT_DIR = .
TOOL_DIR = $(PROJECT_DIR)/Tool
NULLPTR_DIR = $(PROJECT_DIR)/NullptrDegrade
BASE_DIR = $(PROJECT_DIR)/DegraderBase
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
MAIN_SRC_PATH = $(TOOL_DIR)/$(MAIN_SRC)
MAIN_OBJ = $(TOOL_DIR)/$(MAIN_SRC:.cpp=.o)

NULLPTR_SRC = NullptrDegrader.cpp
NULLPTR_SRC_PATH = $(NULLPTR_DIR)/$(NULLPTR_SRC)
NULLPTR_OBJ = $(NULLPTR_DIR)/$(NULLPTR_SRC:.cpp=.o)

BASE_SRC = DegraderBase.cpp
BASE_SRC_PATH = $(BASE_DIR)/$(BASE_SRC)
BASE_OBJ = $(BASE_DIR)/$(BASE_SRC:.cpp=.o)

TOOL = $(BIN_DIR)/clang-degrader
CONFIG = llvm-config
LFLAGS = --ldflags --libs
CFLAGS = --cxxflags


all:$(MAIN_OBJ) $(NULLPTR_OBJ) $(BASE_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) -g $(MAIN_OBJ) $(NULLPTR_OBJ) $(BASE_OBJ) $(LIBS) `$(CONFIG) $(LFLAGS)` -static -ltinfo -lncurses -lstdc++ -lpthread -lm -ldl -o $(TOOL) 

$(MAIN_OBJ):$(MAIN_SRC_PATH)
	$(CC) -g $(MAIN_SRC_PATH) `$(CONFIG) $(CFLAGS)` -fno-rtti -c -o $(MAIN_OBJ) 

$(NULLPTR_OBJ):$(NULLPTR_SRC_PATH)
	$(CC) -g $(NULLPTR_SRC_PATH) `$(CONFIG) $(CFLAGS)` -fno-rtti -c -o $(NULLPTR_OBJ) 

$(BASE_OBJ):$(BASE_SRC_PATH)
	$(CC) -g $(BASE_SRC_PATH) `$(CONFIG) $(CFLAGS)` -fno-rtti -c -o $(BASE_OBJ) 

clean:
	rm -rf $(MAIN_OBJ) $(NULLPTR_OBJ) $(BASE_OBJ) $(TOOL)

run:
	$(TOOL) -start-line=3 -end-line=5 ./sample/sample.cpp
