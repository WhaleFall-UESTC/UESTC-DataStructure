CC = gcc
CFLAGS = -g -Wall
OBJS = main.o file.o itemset.o database.o freqlist.o heap.o
HEAP = heap.o itemset.o freqlist.o
TARGET = main
minsup = 8
topn = 5
TEST = test.txt
SRC = src.txt

all: $(TARGET)

run: all
		./$(TARGET) -m$(minsup) -s $(SRC) -A -M -T$(topn)

rand: all
		./$(TARGET) -R $(SRC)

test: all
		./$(TARGET) -m$(minsup) -r $(TEST) -A -M -T$(topn)

retry: all
		./$(TARGET) -m$(minsup) -s $(TEST) -A -M -T$(topn)

record: all
		./$(TARGET) -m$(minsup) -s $(TEST) -A -M -T$(topn) > record.txt

gdb: all
		gdb --args $(TARGET) -m5 -r $(TEST) -A -M -T$(topn)

heap: $(HEAP)
		$(CC) $(CFLAGS) -o heap $(HEAP)
		./heap

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

heap.o: heap.c
		$(CC) $(CFLAGS) -c heap.c

clean:
		rm -rf $(OBJS) $(TARGET)


.PHONY: all clean run test gdb retry record