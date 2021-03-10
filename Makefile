
.PHONY: all
all: build
	ninja -C build

.PHONY: watch
watch: build
	while inotifywait -e close_write,moved_to,create src test; do make test; done

.PHONY: test
test: build
	meson test -C build --print-errorlogs --no-stdsplit

build:
	meson build

.PHONY: clean
clean:
	rm -rf build
