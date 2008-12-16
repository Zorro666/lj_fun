#! /bin/make

.PHONY: debug clean release cleanrelease gold cleangold

PROJECT:=RenderSort
EXE_NAME:=renderSort

BASE_DIR:=$(BASEROOT)
BUILDROOT:=Build
BUILDROOT:=$(BASE_DIR)/$(BUILDROOT)
BUILD_DIR=$(BUILDROOT)/$(PROJECT)_$(CONFIG)
PROJECT_DIR:=$(BASE_DIR)/$(PROJECT)
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
	@cp $(PROJECT_DIR)/Makefile $(PROJECT_DIR)/Makefile.save
	@cp $(PROJECT_DIR)/$(PROJECT).cmake $(BASE_DIR)/CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(CONFIG) $(BASE_DIR) $* && $(MAKE) -s $(CLEAN)

updateexe:
	@cp -v --preserve=all $(BUILD_DIR)/$(PROJECT)/a.out $(PROJECT_DIR)/$(EXE_NAME)
	@ls -lh $(PROJECT_DIR)/$(EXE_NAME)

all: debug 

test:
	@echo "PROJECT=$(PROJECT)"
	@echo "BUILDROOT=$(BUILDROOT)"
	@echo "BASE_DIR=$(BASE_DIR)"
	@echo "BUILD_DIR=$(BUILD_DIR)"
	@echo "EXE_NAME=$(EXE_NAME)"
	@echo "RPOJECT_DIR=$(PROJECT_DIR)"

