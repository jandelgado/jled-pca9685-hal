.PHONY: phony
# makefile to build examples with platformio
# run with e.g. "make upload ENV=nanoatmega328"

run: phony
	pio run -e ${ENV}

clean: phony
	-pio run --target clean -e ${ENV}
	-rm -rf .pio/libdeps

upload: phony
	pio run --target upload -e ${ENV}

monitor: phony
	pio device monitor

phony:

