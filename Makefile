#! /bin/make

PROJECTS := Unittest BGame RenderSort
ifndef PROJECT
PROJECT := Unittest
endif

CONFIGS := debug release gold
CONFIG := debug
ACTION :=

PROJECTS_CONFIGS := $(foreach project, $(PROJECTS), $(foreach config, $(CONFIGS), $(project)_$(config) ) )
PROJECT_CONFIGS := $(foreach config, $(CONFIGS), $(PROJECT)_$(config) )

.PHONY: clean build rebuild run all test
.PHONY: cleanall buildall rebuildall runall testall
.PHONY: allcleanall allbuildall allrebuildall allrunall alltestall
.PHONY: fulltest
.PHONY: backup tags
.PHONY: $(PROJECTS)
.PHONY: $(PROJECTS_CONFIGS)
.PHONY: $(PROJECT_CONFIGS)

GET_PROJECT = $(word 1, $(subst _, ,$@))
GET_CONFIG = $(word 2, $(subst _, ,$@))

default: build

$(PROJECTS_CONFIGS): 
	@echo "-----Making $@ Project:$(GET_PROJECT) Config:$(ACTION)$(GET_CONFIG)-----"
	@$(MAKE) --no-print-directory -C $(GET_PROJECT) $(ACTION)$(GET_CONFIG)

clean:
	@rm -rvf Build
	@$(MAKE) --no-print-directory -C $(PROJECT) clean$(CONFIG)

build:
	@$(MAKE) --no-print-directory -C $(PROJECT) $(CONFIG)

all: ACTION:=
all: $(PROJECT_CONFIGS)

rebuild: clean 
	@$(MAKE) --no-print-directory build

run:
	@$(MAKE) --no-print-directory -C $(PROJECT) run

cleanall: ACTION:=clean
cleanall: $(PROJECT_CONFIGS)
			
buildall: ACTION:=
buildall: $(PROJECT_CONFIGS)
			
rebuildall: ACTION:=rebuild
rebuildall: $(PROJECT_CONFIGS)
			
runall: ACTION:=run
runall: $(PROJECT_CONFIGS)

fulltest: 
	@$(MAKE) --no-print-directory rebuildall
	@$(MAKE) --no-print-directory runall

allcleanall: ACTION:=clean
allcleanall: $(PROJECTS_CONFIGS)
			
allbuildall: ACTION:=
allbuildall: $(PROJECTS_CONFIGS)
			
allrebuildall: ACTION:=rebuild
allrebuildall: $(PROJECTS_CONFIGS)
			
allrunall: ACTION:=run
allrunall: $(PROJECTS_CONFIGS)

allfulltest: 
	@$(MAKE) --no-print-directory allrebuildall
	@$(MAKE) --no-print-directory allrunall

test: $(PROJECT)
	@echo $(PROJECT)

testall: $(PROJECT_CONFIGS)
	@echo $(PROJECT_CONFIGS)

alltestall: $(PROJECTS_CONFIGS)
	@echo $(PROJECTS_CONFIGS)

backup: BASE_DIR:=$(shell pwd )
backup:
	@$(MAKE) -f makefile.project --no-print-directory BASE_DIR=$(BASE_DIR) backup

tags: BASE_DIR:=$(shell pwd )
tags:
	@$(MAKE) -f makefile.project --no-print-directory BASE_DIR=$(BASE_DIR) tags

$(PROJECTS):
	@echo "-----Making $@ Config:$(CONFIG)-----"
	@$(MAKE) --no-print-directory -C $@ debug
	@$(MAKE) --no-print-directory -C $@ release
	@$(MAKE) --no-print-directory -C $@ gold

