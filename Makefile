BASE = rai

DEPEND = KOMO Core Geo Kin Gui Optim

test_paths = $(shell find test -maxdepth 3 -name 'Makefile' -printf "%h ")

build: $(DEPEND:%=makeDepend/%) $(test_paths:%=makePath/%)

initSubmodules: force
	git submodule init
	git submodule update
	cp config.mk rai/build/config.mk

initUbuntuPackages: $(DEPEND:%=initUbuntuPackages/%)

clean: $(DEPEND:%=cleanPath/%) $(test_paths:%=cleanPath/%)$

runTests: $(test_paths:%=makePath/%) $(test_paths:%=runPath/%)

include $(BASE)/build/generic.mk
