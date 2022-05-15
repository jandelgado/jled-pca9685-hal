.PHONY: phony

build: phony
	pio run

ci: phony
	pio ci --keep-build-dir --project-conf platformio.ini .

clean: phony
	pio run --target clean

upload: phony
	pio run --target upload


monitor: phony
	pio device monitor

phony:

