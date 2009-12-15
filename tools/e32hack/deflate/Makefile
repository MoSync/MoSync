CXXFLAGS = -Iinc -ggdb

deflate.a: deflate/compress.o deflate/decode.o deflate/deflate.o deflate/encode.o deflate/inflate.o deflate/panic.o host/h_mem.o host/h_utl.o
	ar rcs $@ $+

clean:
	rm -f deflate.a deflate/*.o host/*.o

