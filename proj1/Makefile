CC = gcc
CFLAGS = -g -Wall
OBJS = main.o file.o itemset.o database.o freqlist.o
TARGET = main
minsup = 10
TEST = test.txt

all: $(TARGET)

run: all
		./$(TARGET)

test: all
		./$(TARGET) -m$(minsup) -r $(TEST) -A -M -T5

retry: all
		./$(TARGET) -m$(minsup) -s $(TEST) -A -M -T5

record: all
		./$(TARGET) -m$(minsup) -s $(TEST) -A -M -T5 > record.txt

gdb: all
		gdb --args $(TARGET) -m5 -r $(TEST) -A -M -T5

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c 
		$(CC) $(CFLAGS) -c main.c 

file.o: file.c 
		$(CC) $(CFLAGS) -c file.c 

itemset.o: itemset.c
		$(CC) $(CFLAGS) -c itemset.c 

freqlist.o: freqlist.c
		$(CC) $(CFLAGS) -c freqlist.c

clean:
		rm -rf $(OBJS) $(TARGET)


.PHONY: all clean run test gdb retry record