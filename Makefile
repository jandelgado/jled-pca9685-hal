.PHONY: phony

build: phony
	pio run

clean: phony
	pio run --target clean

upload: phony
	pio run --target upload


monitor: phony
	pio device monitor

phony:

