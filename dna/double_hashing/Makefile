PROJNAME := double_hashing

CPP := clang
CFLAGS := -Wall -std=c++14
LFLAGS := -lstdc++ -lm

ifeq (1,${DEBUG})
	CFLAGS += -O0 -g
	OUTDIR := bin/out/debug
else
	CFLAGS += -O2
	OUTDIR := bin/out/release
endif

CPPFILES := $(wildcard *.cpp)
OBJFILES := $(addprefix ${OUTDIR}/obj/, $(CPPFILES:%.cpp=%.o))

$(info Objects: ${OBJFILES})

.PHONY := clean dirs

all: dirs proj

proj: ${OBJFILES}
	@echo "Linking: "${PROJNAME}
	${CPP} -o ${OUTDIR}/${PROJNAME} $? ${LFLAGS}

${OUTDIR}/obj/%.o: %.cpp
	@echo "Compiling "$<
	${CPP} -o $@ -c ${CFLAGS} $<

clean: 
	@rm -r ./bin 2>/dev/null || true
	@rm ${OBJFILES} 2>/dev/null || true
	@rm ${PROJNAME} 2>/dev/null || true

dirs:
	mkdir -p ${OUTDIR}/obj
