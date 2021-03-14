BIN=count_nice_numbers
SRC=count_nice_numbers.cpp

${BIN}: ${SRC}
	c++ ${SRC} -o $@ -O3 -Wall -Werror

clean:
	rm ${BIN}
