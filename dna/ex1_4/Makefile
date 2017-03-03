PROJNAME := ex1_4

CPP := clang
CFLAGS := -Wall -std=c++14
LFLAGS := -lstdc++

ifeq (1,${DEBUG})
	CFLAGS += -O0 -g
else
	CFLAGS += -O2
endif

CPPFILES := $(wildcard *.cpp)
OBJFILES := $(CPPFILES:%.cpp=%.o)

.PHONY := clean

proj: ${OBJFILES}
	@echo "Linking: "${PROJNAME}
	@${CPP} -o ${PROJNAME} $< ${LFLAGS}

%.o: %.cpp
	@echo "Compiling "$<
	@${CPP} -o $@ -c ${CFLAGS} $<

clean: 
	@rm ${OBJFILES} 2>/dev/null || true
	@rm ${PROJNAME} 2>/dev/null || true

