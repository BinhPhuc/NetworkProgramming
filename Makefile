CC = gcc
CFLAGS = -o
TARGET = build

server: server.c
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) $(TARGET)/server server.c

client: client.c
	mkdir -p $(TARGET)
	$(CC) $(CFLAGS) $(TARGET)/client client.c

clean:
	rm -f $(TARGET)/server $(TARGET)/client
