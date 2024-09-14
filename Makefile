# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# List of source files
SRCS = storage_mgr.c dberror.c test_assign1_1.c

# List of object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = test_assign1

# Default target to create the executable
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

