app_name := ZAgent
app_dir  := dist/$(app_name).app/Contents
bin_dir  := $(app_dir)/MacOS

cc_flags := -O2 -Weverything -lobjc
cc_frameworks := -framework Carbon -framework Cocoa


all: build


.PHONY: build clean test

build: $(app_dir)/Info.plist $(app_dir)/PkgInfo $(bin_dir)/$(app_name)

clean:
	rm -rf dist

test: build
	-killall $(app_name)
	open dist/$(app_name).app


$(app_dir):
	mkdir -p $(app_dir)

$(app_dir)/%: src/% | $(app_dir)
	cp $< $@

$(bin_dir):
	mkdir -p $(bin_dir)

$(bin_dir)/$(app_name): src/*.c src/*.m src/*.h | $(bin_dir)
	cc $(cc_flags) $(cc_frameworks) -o $@ $(filter-out %.h,$^)
