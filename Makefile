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
.PHONY: $(PROJECTS)
.PHONY: $(PROJECTS_CONFIGS)
.PHONY: $(PROJECT_CONFIGS)

default: build

$(PROJECTS_CONFIGS): 
	@echo "-----Making $@ Project:$(word 1, $(subst _, ,$@)) Config:$(ACTION)$(word 2, $(subst _, ,$@))-----"
	@$(MAKE) --no-print-directory -C $(word 1, $(subst _, ,$@)) $(ACTION)$(word 2, $(subst _, ,$@))

clean:
	$(MAKE) --no-print-directory -C $(PROJECT) clean$(CONFIG)

build:
	$(MAKE) --no-print-directory -C $(PROJECT) $(CONFIG)

all: ACTION:=
all: $(PROJECT_CONFIGS)

rebuild: clean 
	$(MAKE) --no-print-directory build

run:
	$(MAKE) --no-print-directory -C $(PROJECT) run

cleanall: ACTION:=clean
cleanall: $(PROJECT_CONFIGS)
			
buildall: ACTION:=
buildall: $(PROJECT_CONFIGS)
			
rebuildall: ACTION:=rebuild
rebuildall: $(PROJECT_CONFIGS)
			
runall: ACTION:=run
runall: $(PROJECT_CONFIGS)

fulltest: 
	$(MAKE) --no-print-directory rebuildall
	$(MAKE) --no-print-directory runall

allcleanall: ACTION:=clean
allcleanall: $(PROJECTS_CONFIGS)
			
allbuildall: ACTION:=
allbuildall: $(PROJECTS_CONFIGS)
			
allrebuildall: ACTION:=rebuild
allrebuildall: $(PROJECTS_CONFIGS)
			
allrunall: ACTION:=run
allrunall: $(PROJECTS_CONFIGS)

allfulltest: 
	$(MAKE) --no-print-directory allrebuildall
	$(MAKE) --no-print-directory allrunall

test: $(PROJECT)
	@echo $(PROJECT)

testall: $(PROJECT_CONFIGS)
	@echo $(PROJECT_CONFIGS)

alltestall: $(PROJECTS_CONFIGS)
	@echo $(PROJECTS_CONFIGS)

$(PROJECTS):
	@echo "-----Making $@ Config:$(CONFIG)-----"
	@$(MAKE) --no-print-directory -C $@ debug
	@$(MAKE) --no-print-directory -C $@ release
	@$(MAKE) --no-print-directory -C $@ gold

