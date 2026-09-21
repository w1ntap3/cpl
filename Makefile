SRC1 := $(wildcard task1/*.c)
SRC2 := $(wildcard task2/*.c)
SRC3 := $(wildcard task3/*.c)

TARGET1 := website
TARGET2 := multiclass
TARGET3 := humanize

FLAGS := -Wall -Werror

.PHONY: task1 task2 task3 clean

all: task1 task2 task3

task1:
	gcc -o $(TARGET1) $(SRC1) $(FLAGS)
	./$(TARGET1)

task2:
	gcc -o $(TARGET2) $(SRC2) $(FLAGS)
	./$(TARGET2)

task3:
	gcc -o $(TARGET3) $(SRC3) $(FLAGS) -lcurl
	./$(TARGET3)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
