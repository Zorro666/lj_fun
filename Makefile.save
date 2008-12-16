#! /bin/make

.PHONY: debug clean release cleanrelease gold cleangold

PROJECT:=RenderSort
EXE_NAME:=renderSort

BUILDROOT:=Build
BASE_DIR:=$(shell pwd)
BUILD_DIR=$(BUILDROOT)/$(PROJECT)_$(CONFIG)
CLEAN=

debug: CONFIG:=DEBUG
debug: build updateexe

release: CONFIG:=RELWITHDEBINFO
release: build updateexe

gold: CONFIG:=RELEASE
gold: build updateexe

clean: CONFIG:=DEBUG
clean: CLEAN=clean
clean: build

cleanrelease: CONFIG:=RELWITHDEBINFO
cleanrelease: CLEAN=clean
cleanrelease: build

cleangold: CONFIG:=RELEASE
cleangold: CLEAN=clean
cleangold: build

build:
	@cp Makefile Makefile.save
	@cp $(PROJECT).cmake CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(CONFIG) $(BASE_DIR) $* && $(MAKE) -s $(CLEAN)

updateexe:
	@cp -v --preserve=all $(BUILD_DIR)/$(PROJECT)/a.out $(BASE_DIR)/$(EXE_NAME)
	@ls -lh $(EXE_NAME)

all: debug 

test:
	@echo "PROJECT=$(PROJECT)"
	@echo "BUILDROOT=$(BUILDROOT)"
	@echo "BASE_DIR=$(BASE_DIR)"
	@echo "BUILD_DIR=$(BUILD_DIR)"
	@echo "EXE_NAME=$(EXE_NAME)"

