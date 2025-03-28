#include <types.h>
#include <print.h>
#include <stream.h>

void *memcpy(void *dst, const void *src, size_t n) {
	void *dstaddr = dst;
	void *max = dst + n;

	if (((u_long)src & 3) != ((u_long)dst & 3)) {
		while (dst < max) {
			*(char *)dst++ = *(char *)src++;
		}
		return dstaddr;
	}

	while (((u_long)dst & 3) && dst < max) {
		*(char *)dst++ = *(char *)src++;
	}

	// copy machine words while possible
	while (dst + 4 <= max) {
		*(uint32_t *)dst = *(uint32_t *)src;
		dst += 4;
		src += 4;
	}

	// finish the remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst++ = *(char *)src++;
	}
	return dstaddr;
}

void *memset(void *dst, int c, size_t n) {
	void *dstaddr = dst;
	void *max = dst + n;
	u_char byte = c & 0xff;
	uint32_t word = byte | byte << 8 | byte << 16 | byte << 24;

	while (((u_long)dst & 3) && dst < max) {
		*(u_char *)dst++ = byte;
	}

	// fill machine words while possible
	while (dst + 4 <= max) {
		*(uint32_t *)dst = word;
		dst += 4;
	}

	// finish the remaining 0-3 bytes
	while (dst < max) {
		*(u_char *)dst++ = byte;
	}
	return dstaddr;
}

size_t strlen(const char *s) {
	int n;

	for (n = 0; *s; s++) {
		n++;
	}

	return n;
}

char *strcpy(char *dst, const char *src) {
	char *ret = dst;

	while ((*dst++ = *src++) != 0) {
	}

	return ret;
}

const char *strchr(const char *s, int c) {
	for (; *s; s++) {
		if (*s == c) {
			return s;
		}
	}
	return 0;
}

int strcmp(const char *p, const char *q) {
	while (*p && *p == *q) {
		p++, q++;
	}

	if ((u_int)*p < (u_int)*q) {
		return -1;
	}

	if ((u_int)*p > (u_int)*q) {
		return 1;
	}

	return 0;
}



FILE *fmemopen(FILE *stream, void *buf, const char *mode){
	if(strcmp(mode,"w")==0){
		stream->ptr = (char*)buf;
		stream->base = (char*)buf;
		stream->base = (char*)buf;	
	}
	else if (strcmp(mode,"a")==0){
		stream->base = (char*)buf;
		char  *s = (char*)strchr(buf,'\0');
		stream->ptr = s;
		stream->end = s;
	}
	else{
		return NULL;
	}
	return stream;
}


void outputfile(FILE *data,const char *buf,size_t len){
    
         char *s = data->ptr;
         for(int i = 0; i<len ; i++){
                 *s = buf[i];
                 s++;
         }
         data->ptr = s;
         data->end = s;
 }

int fmemprintf(FILE *stream, const char *fmt, ...){
	char *ptr1 = stream->ptr;
	va_list ap;
	va_start(ap,fmt);
	vprintfmt(outputfile,stream,fmt,ap);
	va_end(ap);		
	char *ptr2 = stream->ptr;
	int len = ptr2-ptr1;
	return len;
}


int fseek(FILE *stream, long offset, int fromwhere){
	char *s = stream->ptr;
	char *base = stream->base;
	char *end = stream->end; 
	s = s + (fromwhere + offset);
	
	if ((s-base > 0)&&(end-s > 0)){
		stream->ptr = s;
		return 0;

	}
	else{
		return -1;
	}

}

int fclose(FILE *stream){
	char *end = stream->end;
	*end = '\0';
	return 0;
}









