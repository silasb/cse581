include ../config.mk

EXE = test
SRC = ppm_read.c ../ppm.c
OBJ = $(SRC:.c=.o)

all: test trackball

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

trackball: trackball.o
	$(CC) $(LDFLAGS) $^ -o $@

.o:
	@echo CC $<
	@$(CC) -c $<

clean:
	rm -f ${OBJ} $(EXE)

dist: clean
	@echo creating dist tarball
	@mkdir -p ${EXE}-${AUTHOR}
	@cp -R ${SRC} ${EXE}-${AUTHOR}
	@zip -r ${EXE}-${AUTHOR}.zip ${EXE}-${AUTHOR}
	@rm -rf ${EXE}-${AUTHOR}

.PHONY: clean dist
