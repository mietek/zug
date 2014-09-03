app_name := ZAgent

cc_flags := -O2 -Weverything -lobjc
cc_frameworks := -framework Carbon -framework Cocoa


all: build


.PHONY: build clean test

build: dist/$(app_name).app/Contents/Info.plist dist/$(app_name).app/Contents/PkgInfo dist/$(app_name).app/Contents/MacOS/$(app_name)

clean:
	rm -rf dist

test: build
	-killall $(app_name)
	open dist/$(app_name).app


dist/$(app_name).app/Contents/%: src/%
	mkdir -p $(@D)
	cp $< $@

dist/$(app_name).app/Contents/MacOS/$(app_name): src/$(app_name).m src/$(app_name).h src/ZCocoa.m src/ZCocoa.h src/ZCommon.c src/ZCommon.h
	mkdir -p $(@D)
	cc $(cc_flags) $(cc_frameworks) -o $@ $(filter-out %.h,$^)
