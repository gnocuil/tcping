CC     := gcc
CFLAGS := 
TARGET := tcping
OBJS   := main.o

all: $(TARGET)

$(TARGET) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
		$(CC) -c $(CFLAGS) $< -o $@

clean :
		rm -f $(TARGET) $(OBJS)
