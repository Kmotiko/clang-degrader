sample-degrader
===============


# はじめに
c85「きつねさんとおぼえるClang おかわり！」用のClanToolのサンプルとして作成した
ソースコード中の「nullptr」を「NULL」に置換してしまうアレなツールです．
気が向いたら他の機能も実装してみます．

# 動作環境
以下の環境で動作確認しています。

|項目       |説明       |
|:----------|:----------|
|OS         | Ubuntu 13.04 64bit|
|GCC        | GCC4.7.3|
|LLVM/Clang | 3.4-rc1|



# 使い方
使い方の説明を簡単に書いておきます。


## インストール/実行

### CMake/ninja

```shell
# git clone
$ cd <path to clang-tools-extra>
$ git clone git@github.com:Kmotiko/sample-degrader.git
$ echo "add_subdirectory(sample-degrader)" >> CMakeLists.txt

# build/install 
$ cd <path to LLVM/Clang build directory>
$ ninja
$ sudo ninja install

# 実行
$ cd <path to clang-tools-extra>/sample-degrader
$ sample-degrader ./sample/sample.cpp -- -std=c++11
```

### Makefile

```shell
# git clone/build
$ cd <path to any directory>
$ git clone git@github.com:Kmotiko/sample-degrader.git
$ cd sample-degrader
$ make

# 実行
$ ./bin/sample-degrader ./sample/sample.cpp -- -std=c++11
```
