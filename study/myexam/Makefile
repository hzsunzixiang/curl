.PHONY:clean all
CC=gcc

#CFLAGS =  -g --save-temps -lcurl
CFLAGS =  -g -lcurl

BIN= curl_global_init  curl_version
all:$(BIN)
%:%.c 
	$(CC) $(CFLAGS)   $< -o $@
	
clean:
	rm -f *.o $(BIN) *.s *.ii *.i
