# YetOS

MikanOS を M1 Mac のエミュレータ上で動かすためのドキュメント & スクリプト。

動作確認環境
```sh
> uname -a 
Darwin Kentas-MacBook-Air.local 21.5.0 Darwin Kernel Version 21.5.0: Tue Apr 26 21:08:29 PDT 2022; root:xnu-8020.121.3~4/RELEASE_ARM64_T8101 arm64
```

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
ln -s ~/osbook /workspaces/mikanos-devcontainer/
ln -s ~/edk2 /workspaces/mikanos-devcontainer/
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

```sh
# Setup env vars for build.
source ~/osbook/devenv/buildenv.sh
echo $CPPFLAGS

# Build kernel.
cd $YETOS_DIR/kernel
make clean && make

cd ~/edk2
ln -s $YETOS_DIR/YetLoaderPkg ~/edk2/
source edksetup.sh
cp $WS_DIR/target_yetos.txt ~/edk2/Conf/target.txt
cp $WS_DIR/tools_def.txt ~/edk2/Conf/tools_def.txt

build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/YetLoaderX64/DEBUG_CLANG38/X64/Loader.efi $YETOS_DIR/kernel/kernel.elf
```


## 1.4 エミュレータでのやり方

```sh
cd ~/osbook/day01/bin/
~/osbook/devenv/run_qemu.sh hello.efi
```


## 1.9 C言語でハローワールド

```sh
cd ~/osbook/day01/c/
clang -target x86_64-pc-win32-coff -mno-red-zone -fno-stack-protector -fshort-wchar -Wall -c hello.c
lld-link /subsystem:efi_application /entry:EfiMain /out:hello.efi hello.o

~/osbook/devenv/run_qemu.sh hello.efi
```

## 2.2 EDK2でハローワールド

ここから mikanos のリポジトリのコードを使う。

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day02a
find . -type f -name '*.d' -delete -o -name '*.o' -delete

cd ~/edk2
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

## 2.7 メモリマップの確認

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day02b

# Setup EDK2
cd ~/edk2
ln -s /workspaces/mikanos-devcontainer/mikanos/MikanLoaderPkg ./
source edksetup.sh

# Configure for cross compile.
cp /workspaces/mikanos-devcontainer/target.txt ~/edk2/Conf/target.txt

# EDK2 setup
cp /workspaces/mikanos-devcontainer/target.txt ~/edk2/Conf/target.txt

cd ~/edk2
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

## 3.3 初めてのカーネル

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day03a
cd kernel
clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
ld.lld --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o

cd ~/edk2
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## 3.4 ブートローダからピクセルを描く

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day03b
cd kernel
clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -c main.cpp
ld.lld --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o

cd ~/edk2
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build
~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## 3.5 カーネルからピクセルを描く

```sh
source ~/osbook/devenv/buildenv.sh
echo $CPPFLAGS
cd $MIKANOS_DIR
git reset --hard osbook_day03c
cd kernel
clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c main.cpp
ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o
cd ~/edk2
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build
~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## 4.2 ピクセルを自在に描く

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day04b
cd kernel
source ~/osbook/devenv/buildenv.sh
make

cd ~/edk2
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh

build
~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## 5.1 文字を書いてみる

```sh
cd $MIKANOS_DIR
git reset --hard osbook_day05a
cd kernel
source ~/osbook/devenv/buildenv.sh
make

cd ~/edk2
rm -fr ~/edk2/MikanLoaderPkg && ln -s $MIKANOS_DIR/MikanLoaderPkg ~/edk2/
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## 5.1 文字を書いてみる

```sh
$MIKANOS_DIR/tools/makefont.py -o $MIKANOS_DIR/kernel/hankaku.bin $MIKANOS_DIR/kernel/hankaku.txt
objcopy -I binary -O elf64-x86-64 -B aarm:x86-64 $MIKANOS_DIR/kernel/hankaku.bin $OS_DIR/kernel/hankaku.o

cd $MIKANOS_DIR
git reset --hard osbook_day05d
cd kernel
source ~/osbook/devenv/buildenv.sh
make clean && make

cd ~/edk2
rm -fr ~/edk2/MikanLoaderPkg && ln -s $MIKANOS_DIR/MikanLoaderPkg ~/edk2/
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```

## Masterブランチの MikanOS を動かす

```sh
cd $MIKANOS_DIR
git reset --hard master
cd kernel
source ~/osbook/devenv/buildenv.sh
make

cd ~/edk2
source edksetup.sh
source /workspaces/mikanos-devcontainer/m1_setup.sh
build

~/osbook/devenv/run_qemu.sh ~/edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $MIKANOS_DIR/kernel/kernel.elf
```