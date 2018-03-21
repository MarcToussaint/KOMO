# KOMO (k-order Markov Path Optimization)

This is a container repo, exposing the KOMO algorithm of the RAI code. See https://github.com/MarcToussaint/rai for a README of the RAI code and why this is packaged as is.

## Quick Start

```
git clone git@github.com:MarcToussaint/KOMO.git
cd KOMO

# if you don't have a github account: (switch submodules to public url)
git config --file=.gitmodules submodule.rai.url https://github.com/MarcToussaint/rai.git
git config --file=.gitmodules submodule.rai-robotModels.url https://github.com/MarcToussaint/rai-robotModels.git

git submodule init
git submodule update

make -j1 initUbuntuPackages  # calls sudo apt-get install; you can always interrupt
make                         # builds libs and tests
make runTests
```
Check `test/KOMO/komo` and `test/KOMO/tutorial` to get started.
