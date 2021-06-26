DOCKER = docker
DOCKER_IMG = pres-builder
PRES_OUT = pres.pdf


default: build

dockerimgbuild:
	$(DOCKER) build -t $(DOCKER_IMG) .

build: dockerimgbuild
	$(DOCKER) run --rm -v `pwd`:/data --user `id -u`:`id -g` $(DOCKER_IMG)  /data/pres.md -t beamer -F pandoc-plantuml -M date="$(date "+%d %B %Y")"  -V classoption:aspectratio=169 -H preamble.tex -o /data/$(PRES_OUT)

clean:
	rm -r $(PRES_OUT)

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... build"
	@echo "... dockerimgbuild"

# disallow any parallelism (-j) for Make. This is necessary since some
# commands during the build process create temporary files that collide
# under parallel conditions.
.NOTPARALLEL:

.PHONY:
