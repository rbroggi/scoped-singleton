DOCKER = docker
DOCKER_IMG = scoped-singleton-build-image
BUILD_FOLDER = build


default: test

dockerimgbuild:
	$(DOCKER) build -t $(DOCKER_IMG) .

cmakebuild: dockerimgbuild
	$(DOCKER) run --rm -v `pwd`:/app --user `id -u`:`id -g` $(DOCKER_IMG) cmake -B./$(BUILD_FOLDER)

build: cmakebuild
	$(DOCKER) run --rm -v `pwd`:/app --user `id -u`:`id -g` $(DOCKER_IMG) make -C $(BUILD_FOLDER)

test: build
	$(DOCKER) run --rm -v `pwd`:/app --user `id -u`:`id -g` $(DOCKER_IMG) ./$(BUILD_FOLDER)/tests/ScopedSingleton_tst --gtest_color=yes $(TSTARGS)

utest:
	$(DOCKER) run --rm -v `pwd`:/app --user `id -u`:`id -g` $(DOCKER_IMG) ./$(BUILD_FOLDER)/tests/ScopedSingleton_tst --gtest_color=yes $(TSTARGS)

clean:
	rm -r $(BUILD_FOLDER)

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... build"
	@echo "... cmakebuild"
	@echo "... dockerimgbuild"
	@echo "... test"
	@echo "... utest"

# disallow any parallelism (-j) for Make. This is necessary since some
# commands during the build process create temporary files that collide
# under parallel conditions.
.NOTPARALLEL:

.PHONY:
