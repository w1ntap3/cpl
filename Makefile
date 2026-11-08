SRC1 := $(wildcard task1/*.c)
SRC2 := $(wildcard task2/*.c)
SRC3 := $(wildcard task3/*.c)
SRC4 := $(wildcard hw01/*.c)
SRC5 := $(wildcard hw02/*.c)

TARGET1 := password
TARGET2 := multiclass
TARGET3 := humanize
TARGET4 := bayesian
TARGET5 := staticmap

FLAGS := -Wall -Werror -O2 

.PHONY: task1 task2 task3 hw01 hw02 clean

task1:
	gcc -o $(TARGET1) $(SRC1) $(FLAGS)
	./$(TARGET1)

task2:
	gcc -o $(TARGET2) $(SRC2) $(FLAGS)
	./$(TARGET2)

task3:
	gcc -o $(TARGET3) $(SRC3) $(FLAGS) -lcurl
	./$(TARGET3)

hw01:
	gcc -o $(TARGET4) $(SRC4) $(FLAGS)
	zip -r hw01/hw01.zip ${SRC4}

hw02:
	@gcc -std=c99 -pedantic $(FLAGS) -g -o ./hw02/$(TARGET5) $(SRC5) -lm
	@echo "0.0 5.0" | ./hw02/$(TARGET5) 21.0 21.0 1.0 11.0 11.0 0.0
	@cd ./hw02 && zip -q hw02.zip main.c

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) ${TARGET5}
