.PHONY:clean all
CC=gcc

#CFLAGS =  -g --save-temps -lcurl
CFLAGS =  -g -lcurl

SKIP_PEER_VERIFICATION = OK
SKIP_HOSTNAME_VERIFICATION = OK
BIN= curl_global_init  curl_version https simplepost simplessl_1
all:$(BIN)
%:%.c 
	$(CC) $(CFLAGS)   $< -o $@ -DSKIP_PEER_VERIFICATION -DSKIP_HOSTNAME_VERIFICATION
	#$(CC) $(CFLAGS)   $< -o $@ -DSKIP_HOSTNAME_VERIFICATION
	#$(CC) $(CFLAGS)   $< -o $@ -DSKIP_PEER_VERIFICATION
	
clean:
	rm -f *.o $(BIN) *.s *.ii *.i
