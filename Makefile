#! /bin/make

.PHONY: debug clean release cleanrelease rebuildrelease gold cleangold rebuildgold tags cleantags rebuildtags rebuildall

BASE_DIR:=$(shell cd .. ; pwd )
BUILDROOT:=Build
BUILDROOT:=$(BASE_DIR)/$(BUILDROOT)
BUILD_DIR=$(BUILDROOT)/$(PROJECT)_$(CONFIG)
PROJECT_DIR:=$(BASE_DIR)/$(PROJECT)
CLEAN=

debug: CONFIG:=DEBUG
debug: CLEAN=
debug: build updateexe

release: CONFIG:=RELWITHDEBINFO
release: CLEAN=
release: build updateexe

gold: CONFIG:=RELEASE
gold: CLEAN=
gold: build updateexe

cleandebug: CONFIG:=DEBUG
cleandebug: CLEAN=clean
cleandebug: build

cleanrelease: CONFIG:=RELWITHDEBINFO
cleanrelease: CLEAN=clean
cleanrelease: build

cleangold: CONFIG:=RELEASE
cleangold: CLEAN=clean
cleangold: build

rebuilddebug: 
	@$(MAKE) --no-print-directory cleandebug
	@$(MAKE) --no-print-directory debug

rebuildrelease: 
	@$(MAKE) --no-print-directory cleanrelease
	@$(MAKE) --no-print-directory release

rebuildgold: 
	@$(MAKE) --no-print-directory cleangold
	@$(MAKE) --no-print-directory gold

build:
	@echo ----- $(CLEAN) $(CONFIG) -----
	@cp $(PROJECT_DIR)/$(PROJECT).cmake $(BASE_DIR)/CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(CONFIG) $(BASE_DIR) $* && $(MAKE) --no-print-directory $(CLEAN)

updateexe:
	@cp -v --preserve=all $(BUILD_DIR)/$(PROJECT)/a.out $(PROJECT_DIR)/$(EXE_NAME)
	@ls -lh $(PROJECT_DIR)/$(EXE_NAME)

all: debug 

rebuild: rebuilddebug

tags:
	@echo ----- tags -----
	@$(MAKE) -C $(BASE_DIR) -f makefile.tags --no-print-directory

cleantags:
	@echo ----- clean tags -----
	@$(MAKE) -C $(BASE_DIR) -f makefile.tags --no-print-directory clean

rebuildtags: cleantags tags

rebuildall: 
	@$(MAKE) --no-print-directory rebuilddebug
	@$(MAKE) --no-print-directory rebuildrelease
	@$(MAKE) --no-print-directory rebuildgold
	@$(MAKE) --no-print-directory rebuildtags

test:
	@echo "PROJECT=$(PROJECT)"
	@echo "BUILDROOT=$(BUILDROOT)"
	@echo "BASE_DIR=$(BASE_DIR)"
	@echo "BUILD_DIR=$(BUILD_DIR)"
	@echo "EXE_NAME=$(EXE_NAME)"
	@echo "RPOJECT_DIR=$(PROJECT_DIR)"

