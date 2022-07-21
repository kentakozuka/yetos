# YetOS

YetOS is DIY OS.
The codes are mostly copied from and based on [MikanOS](https://github.com/uchan-nos/mikanos), which is educational OS witten in [ゼロからのOS自作入門](https://book.mynavi.jp/ec/products/detail/id=121220).
The dev tools are cloned from [mikanos-build](https://github.com/uchan-nos/mikanos-build). Devcontainer's Dockerfile is forked from [sarisia/mikanos-docker](https://github.com/sarisia/mikanos-docker)

Devlopment environement

```sh
> uname -a 
Darwin Kentas-MacBook-Air.local 21.5.0 Darwin Kernel Version 21.5.0: Tue Apr 26 21:08:29 PDT 2022; root:xnu-8020.121.3~4/RELEASE_ARM64_T8101 arm64
```

## Reference

- 元の書籍
[ゼロからのOS自作入門](https://book.mynavi.jp/ec/products/detail/id=121220)
  - [mikanos-build](https://github.com/uchan-nos/mikanos-build): ビルド手順およびツール群

- 基本この記事の著者のツールを使っています。
[Docker ではじめる "ゼロからのOS自作入門"](https://zenn.dev/sarisia/articles/6b57ea835344b6)
  - このリポジトリのテンプレ: [sarisia/mikanos-devcontainer](https://github.com/sarisia/mikanos-devcontainer)
  - Dockerfileはこちら: [sarisia/mikanos-docker](https://github.com/sarisia/mikanos-docker)

- とても為になった記事
[「ゼロからのOS自作入門」の副読本的記事](https://zenn.dev/karaage0703/articles/1bdb8930182c6c)

## セットアップ

最初の1回でよい。

```sh
# MikanOSのクローン
cd $WS_DIR
git clone https://github.com/uchan-nos/mikanos.git
# やらなくても良い、がコードが見やすいので
ln -s ~/osbook $WS_DIR/
ln -s ~/edk2 $WS_DIR/
```

## OVFM

ビルド通らない。

```sh
cd ~/edk2
source edksetup.sh
cp $WS_DIR/target_ovfm.txt ~/edk2/Conf/target.txt
cp $WS_DIR/tools_def.txt ~/edk2/Conf/tools_def.txt
build
ll ~/edk2/Build/
```

## YetOS

- Clang-formatやるとimportの順番変わってビルド通らなくなる（`uefi.hpp`)
- message.hppもビルドおかしくなる

```sh
source ~/osbook/devenv/buildenv.sh
echo $CPPFLAGS

cd ~/edk2
rm $YETOS_DIR/YetLoaderPkg && ln -s $YETOS_DIR/YetLoaderPkg ~/edk2/
source edksetup.sh
cp $WS_DIR/target_yetos.txt ~/edk2/Conf/target.txt
cp $WS_DIR/tools_def.txt ~/edk2/Conf/tools_def.txt
build

cd $YETOS_DIR
source build.sh run


cd $YETOS_DIR
cd kernel
source ~/osbook/devenv/buildenv.sh
make clean && make

# cd ~/edk2
# source edksetup.sh
# ln -s $WS_DIR/mikanos/MikanLoaderPkg ./
# source $WS_DIR/m1_setup.sh
# build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/YetLoaderX64/DEBUG_CLANG38/X64/Loader.efi $YETOS_DIR/kernel/kernel.elf
```

## Masterブランチの MikanOS を動かす

```sh
cd $MIKANOS_DIR
git reset --hard master
find . -type f -name '*.d' -delete -o -name '*.o' -delete -o -name '*.elf' -delete
sudo chmod 777 ~/.vscode-server/extensions/ms-vscode.cpptools-1.10.8-linux-arm64/LLVM/bin/clang-format
find . -regex '.*\.\(c\|h\|cpp\|hpp\|cc\|cxx\)' -exec ~/.vscode-server/extensions/ms-vscode.cpptools-1.10.8-linux-arm64/LLVM/bin/clang-format -i -style=file {} \;

cd $MIKANOS_DIR
cd kernel
source ~/osbook/devenv/buildenv.sh
make

cd ~/edk2
source edksetup.sh
ln -s $WS_DIR/mikanos/MikanLoaderPkg ./
source $WS_DIR/m1_setup.sh
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```
