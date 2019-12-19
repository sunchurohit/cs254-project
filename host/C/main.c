/* 
 * Copyright (C) 2012-2014 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <makestuff.h>
#include <libfpgalink.h>
#include <libbuffer.h>
#include <liberror.h>
#include <libdump.h>
#include <argtable2.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif


void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}


char *strndup(const char *s, size_t n) {
    char *p = memchr(s, '\0', n);
    if (p != NULL)
        n = p - s;
    p = malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, n);
        p[n] = '\0';
    }
    return p;
}



struct my_record {
    int xcor , ycor ; 
    int x , y , z; 
    };

void dec2bin(int c, char *ans)
{
   int i = 0;
   for(i = 31; i >= 0; i--){
     if((c & (1 << i)) != 0){
       //printf("1");
       ans[31-i]='1';
     }else{
       //printf("0");
       ans[31-i]='0';
     } 
   }
}


//concat function taken from internet .
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
// does xorr
char* xorr(const char *s1, const char *s2)
{
	char *result = "" ; 
	for(int i=0;i<32;i++)
	{
		if(s1[i] == s2[i])
		{
			result = concat(result,"0") ;
		}
		else
		{
			result = concat(result,"1") ;
		}
	}

	return result ;
}
// does carry.
char* carry(const char *s1,const char *s2)
{
	char* result = "" ;

	int temp = 0 ;

	for(int i=3;i>=0;i--)
	{
		if(s1[i] == '0' && s2[i] == '0' && temp == 0)
		{
			result = concat("0",result) ;
		}
		else if (s1[i] == '0' && s2[i] == '0' && temp == 1)
		{
			result = concat("1",result) ; temp = 0 ;
		}
		else if (s1[i] == '1' && s2[i] == '0' && temp == 0)
		{
			result = concat("1",result) ;
		}
		else if (s1[i] == '1' && s2[i] == '0' && temp == 1)
		{
			result = concat("0",result); 
		}
		else if (s1[i] == '0' && s2[i] == '1' && temp == 0)
		{
			result = concat("1",result) ;
		}
		else if (s1[i] == '0' && s2[i] == '1' && temp == 1)
		{
			result = concat("0",result) ; 
		}
		else if (s1[i] == '1' && s2[i] == '1' && temp == 0)
		{
			result = concat("0",result) ; temp = 1 ;
		}
		else
		{
			result = concat("1",result) ;
		}
	}

	return result ;
}

// does encrytion.
char* enc(char* k,char* p)
{
	int n1 = 0;
	for(int i=0;i<32;i++)
	{
		if(k[i] == '1')
		{
			n1++;
		};
	};
	char* c = "" ;
	
	c = concat(c,p) ;

	char* t = "" ; 

	int temp = 0;

	for(int i=31;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	for(int i=30;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0;

	for(int i=29;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	for(int i=28;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	//printf("%s\n",t) ;

	for(int i=0;i<n1;i++)
	{
		char* tt = "" ;

		for(int j=0;j<8;j++)
		{
			tt = concat(tt,t) ;
		}

		c = xorr(c,tt) ;

		t = carry(t,"0001") ;
	}
	return c; 
}

char* dec(char* k,char* c)
{
	int n0 = 0;
	for(int i=0;i<32;i++)
	{
		if(k[i] == '0')
		{
			n0++;
		};
	};
	char* p = "" ;

	p = concat(p,c) ;

	char* t = "" ;

	int temp = 0;

	for(int i=31;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	for(int i=30;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0;

	for(int i=29;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	for(int i=28;i>=0;i=i-4)
	{
		if(k[i] == '1')
		{
			temp++ ;
		}
	}

	if(temp%2 == 0)
	{
		t = concat(t,"0") ;
	}
	else
	{
		t = concat(t,"1") ;
	}

	temp = 0 ;

	//printf("%s\n",t) ;

	t = carry(t,"1111") ;

	for(int i=0;i<n0;i++)
	{
		char* tt = "" ;

		for(int j=0;j<8;j++)
		{
			tt = concat(tt,t) ;
		}

		p = xorr(p,tt) ;

		t = carry(t,"1111") ;
	}

	return p ;
}
// encrypts an a byte
void enc_array(uint8* a,char* k){
	for(int i=0;i<4;i++){
		char* buffer = malloc(32);
		dec2bin(a[i],buffer);
		char* enc_msg = enc(k,buffer);
		for(int j=0;j<32;j++)
		{
 			a[i] *= 2;
		 if (*enc_msg++ == '1') a[i] += 1;
		}
	}
}
// decrypts an array.
void dec_array(uint8* a,char* k){
	for(int i=1;i<5;i++){
		char* buffer = malloc(32);
		dec2bin(a[i],buffer);
		char* enc_msg = dec(k,buffer);
		for(int j=0;j<32;j++)
		{
 			a[i] *= 2;
		 if (*enc_msg++ == '1') a[i] += 1;
		}
	}
}



bool sigIsRaised(void);
void sigRegisterHandler(void);

static const char *ptr;
static bool enableBenchmarking = false;

static bool isHexDigit(char ch) {
	return
		(ch >= '0' && ch <= '9') ||
		(ch >= 'a' && ch <= 'f') ||
		(ch >= 'A' && ch <= 'F');
}

static uint16 calcChecksum(const uint8 *data, size_t length) {
	uint16 cksum = 0x0000;
	while ( length-- ) {
		cksum = (uint16)(cksum + *data++);
	}
	return cksum;
}

static bool getHexNibble(char hexDigit, uint8 *nibble) {
	if ( hexDigit >= '0' && hexDigit <= '9' ) {
		*nibble = (uint8)(hexDigit - '0');
		return false;
	} else if ( hexDigit >= 'a' && hexDigit <= 'f' ) {
		*nibble = (uint8)(hexDigit - 'a' + 10);
		return false;
	} else if ( hexDigit >= 'A' && hexDigit <= 'F' ) {
		*nibble = (uint8)(hexDigit - 'A' + 10);
		return false;
	} else {
		return true;
	}
}

static int getHexByte(uint8 *byte) {
	uint8 upperNibble;
	uint8 lowerNibble;
	if ( !getHexNibble(ptr[0], &upperNibble) && !getHexNibble(ptr[1], &lowerNibble) ) {
		*byte = (uint8)((upperNibble << 4) | lowerNibble);
		byte += 2;
		return 0;
	} else {
		return 1;
	}
}

static const char *const errMessages[] = {
	NULL,
	NULL,
	"Unparseable hex number",
	"Channel out of range",
	"Conduit out of range",
	"Illegal character",
	"Unterminated string",
	"No memory",
	"Empty string",
	"Odd number of digits",
	"Cannot load file",
	"Cannot save file",
	"Bad arguments"
};

typedef enum {
	FLP_SUCCESS,
	FLP_LIBERR,
	FLP_BAD_HEX,
	FLP_CHAN_RANGE,
	FLP_CONDUIT_RANGE,
	FLP_ILL_CHAR,
	FLP_UNTERM_STRING,
	FLP_NO_MEMORY,
	FLP_EMPTY_STRING,
	FLP_ODD_DIGITS,
	FLP_CANNOT_LOAD,
	FLP_CANNOT_SAVE,
	FLP_ARGS
} ReturnCode;

static ReturnCode doRead(
	struct FLContext *handle, uint8 chan, uint32 length, FILE *destFile, uint16 *checksum,
	const char **error)
{
	ReturnCode retVal = FLP_SUCCESS;
	uint32 bytesWritten;
	FLStatus fStatus;
	uint32 chunkSize;
	const uint8 *recvData;
	uint32 actualLength;
	const uint8 *ptr;
	uint16 csVal = 0x0000;
	#define READ_MAX 65536

	// Read first chunk
	chunkSize = length >= READ_MAX ? READ_MAX : length;
	fStatus = flReadChannelAsyncSubmit(handle, chan, chunkSize, NULL, error);
	CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doRead()");
	length = length - chunkSize;

	while ( length ) {
		// Read chunk N
		chunkSize = length >= READ_MAX ? READ_MAX : length;
		fStatus = flReadChannelAsyncSubmit(handle, chan, chunkSize, NULL, error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doRead()");
		length = length - chunkSize;
		
		// Await chunk N-1
		fStatus = flReadChannelAsyncAwait(handle, &recvData, &actualLength, &actualLength, error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doRead()");

		// Write chunk N-1 to file
		bytesWritten = (uint32)fwrite(recvData, 1, actualLength, destFile);
		CHECK_STATUS(bytesWritten != actualLength, FLP_CANNOT_SAVE, cleanup, "doRead()");

		// Checksum chunk N-1
		chunkSize = actualLength;
		ptr = recvData;
		while ( chunkSize-- ) {
			csVal = (uint16)(csVal + *ptr++);
		}
	}

	// Await last chunk
	fStatus = flReadChannelAsyncAwait(handle, &recvData, &actualLength, &actualLength, error);
	CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doRead()");
	
	// Write last chunk to file
	bytesWritten = (uint32)fwrite(recvData, 1, actualLength, destFile);
	CHECK_STATUS(bytesWritten != actualLength, FLP_CANNOT_SAVE, cleanup, "doRead()");

	// Checksum last chunk
	chunkSize = actualLength;
	ptr = recvData;
	while ( chunkSize-- ) {
		csVal = (uint16)(csVal + *ptr++);
	}
	
	// Return checksum to caller
	*checksum = csVal;
cleanup:
	return retVal;
}

static ReturnCode doWrite(
	struct FLContext *handle, uint8 chan, FILE *srcFile, size_t *length, uint16 *checksum,
	const char **error)
{
	ReturnCode retVal = FLP_SUCCESS;
	size_t bytesRead, i;
	FLStatus fStatus;
	const uint8 *ptr;
	uint16 csVal = 0x0000;
	size_t lenVal = 0;
	#define WRITE_MAX (65536 - 5)
	uint8 buffer[WRITE_MAX];

	do {
		// Read Nth chunk
		bytesRead = fread(buffer, 1, WRITE_MAX, srcFile);
		if ( bytesRead ) {
			// Update running total
			lenVal = lenVal + bytesRead;

			// Submit Nth chunk
			fStatus = flWriteChannelAsync(handle, chan, bytesRead, buffer, error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doWrite()");

			// Checksum Nth chunk
			i = bytesRead;
			ptr = buffer;
			while ( i-- ) {
				csVal = (uint16)(csVal + *ptr++);
			}
		}
	} while ( bytesRead == WRITE_MAX );

	// Wait for writes to be received. This is optional, but it's only fair if we're benchmarking to
	// actually wait for the work to be completed.
	fStatus = flAwaitAsyncWrites(handle, error);
	CHECK_STATUS(fStatus, FLP_LIBERR, cleanup, "doWrite()");

	// Return checksum & length to caller
	*checksum = csVal;
	*length = lenVal;
cleanup:
	return retVal;
}

static int parseLine(struct FLContext *handle, const char *line, const char **error) {
	ReturnCode retVal = FLP_SUCCESS, status;
	FLStatus fStatus;
	struct Buffer dataFromFPGA = {0,};
	BufferStatus bStatus;
	uint8 *data = NULL;
	char *fileName = NULL;
	FILE *file = NULL;
	double totalTime, speed;
	#ifdef WIN32
		LARGE_INTEGER tvStart, tvEnd, freq;
		DWORD_PTR mask = 1;
		SetThreadAffinityMask(GetCurrentThread(), mask);
		QueryPerformanceFrequency(&freq);
	#else
		struct timeval tvStart, tvEnd;
		long long startTime, endTime;
	#endif
	bStatus = bufInitialise(&dataFromFPGA, 1024, 0x00, error);
	CHECK_STATUS(bStatus, FLP_LIBERR, cleanup);
	ptr = line;
	do {
		while ( *ptr == ';' ) {
			ptr++;
		}
		switch ( *ptr ) {
		case 'r':{
			uint32 chan;
			uint32 length = 1;
			char *end;
			ptr++;
			
			// Get the channel to be read:
			errno = 0;
			chan = (uint32)strtoul(ptr, &end, 16);
			CHECK_STATUS(errno, FLP_BAD_HEX, cleanup);

			// Ensure that it's 0-127
			CHECK_STATUS(chan > 127, FLP_CHAN_RANGE, cleanup);
			ptr = end;

			// Only three valid chars at this point:
			CHECK_STATUS(*ptr != '\0' && *ptr != ';' && *ptr != ' ', FLP_ILL_CHAR, cleanup);

			if ( *ptr == ' ' ) {
				ptr++;

				// Get the read count:
				errno = 0;
				length = (uint32)strtoul(ptr, &end, 16);
				CHECK_STATUS(errno, FLP_BAD_HEX, cleanup);
				ptr = end;
				
				// Only three valid chars at this point:
				CHECK_STATUS(*ptr != '\0' && *ptr != ';' && *ptr != ' ', FLP_ILL_CHAR, cleanup);
				if ( *ptr == ' ' ) {
					const char *p;
					const char quoteChar = *++ptr;
					CHECK_STATUS(
						(quoteChar != '"' && quoteChar != '\''),
						FLP_ILL_CHAR, cleanup);
					
					// Get the file to write bytes to:
					ptr++;
					p = ptr;
					while ( *p != quoteChar && *p != '\0' ) {
						p++;
					}
					CHECK_STATUS(*p == '\0', FLP_UNTERM_STRING, cleanup);
					fileName = malloc((size_t)(p - ptr + 1));
					CHECK_STATUS(!fileName, FLP_NO_MEMORY, cleanup);
					CHECK_STATUS(p - ptr == 0, FLP_EMPTY_STRING, cleanup);
					strncpy(fileName, ptr, (size_t)(p - ptr));
					fileName[p - ptr] = '\0';
					ptr = p + 1;
				}
			}
			if ( fileName ) {
				uint16 checksum = 0x0000;

				// Open file for writing
				file = fopen(fileName, "wb");
				CHECK_STATUS(!file, FLP_CANNOT_SAVE, cleanup);
				free(fileName);
				fileName = NULL;

				#ifdef WIN32
					QueryPerformanceCounter(&tvStart);
					status = doRead(handle, (uint8)chan, length, file, &checksum, error);
					QueryPerformanceCounter(&tvEnd);
					totalTime = (double)(tvEnd.QuadPart - tvStart.QuadPart);
					totalTime /= freq.QuadPart;
					speed = (double)length / (1024*1024*totalTime);
				#else
					gettimeofday(&tvStart, NULL);
					status = doRead(handle, (uint8)chan, length, file, &checksum, error);
					gettimeofday(&tvEnd, NULL);
					startTime = tvStart.tv_sec;
					startTime *= 1000000;
					startTime += tvStart.tv_usec;
					endTime = tvEnd.tv_sec;
					endTime *= 1000000;
					endTime += tvEnd.tv_usec;
					totalTime = (double)(endTime - startTime);
					totalTime /= 1000000;  // convert from uS to S.
					speed = (double)length / (1024*1024*totalTime);
				#endif
				if ( enableBenchmarking ) {
					printf(
						"Read %d bytes (checksum 0x%04X) from channel %d at %f MiB/s\n",
						length, checksum, chan, speed);
				}
				CHECK_STATUS(status, status, cleanup);

				// Close the file
				fclose(file);
				file = NULL;
			} else {
				size_t oldLength = dataFromFPGA.length;
				bStatus = bufAppendConst(&dataFromFPGA, 0x00, length, error);
				CHECK_STATUS(bStatus, FLP_LIBERR, cleanup);
				#ifdef WIN32
					QueryPerformanceCounter(&tvStart);
					fStatus = flReadChannel(handle, (uint8)chan, length, dataFromFPGA.data + oldLength, error);
					QueryPerformanceCounter(&tvEnd);
					totalTime = (double)(tvEnd.QuadPart - tvStart.QuadPart);
					totalTime /= freq.QuadPart;
					speed = (double)length / (1024*1024*totalTime);
				#else
					gettimeofday(&tvStart, NULL);
					fStatus = flReadChannel(handle, (uint8)chan, length, dataFromFPGA.data + oldLength, error);
					gettimeofday(&tvEnd, NULL);
					startTime = tvStart.tv_sec;
					startTime *= 1000000;
					startTime += tvStart.tv_usec;
					endTime = tvEnd.tv_sec;
					endTime *= 1000000;
					endTime += tvEnd.tv_usec;
					totalTime = (double)(endTime - startTime);
					totalTime /= 1000000;  // convert from uS to S.
					speed = (double)length / (1024*1024*totalTime);
				#endif
				if ( enableBenchmarking ) {
					printf(
						"Read %d bytes (checksum 0x%04X) from channel %d at %f MiB/s\n",
						length, calcChecksum(dataFromFPGA.data + oldLength, length), chan, speed);
				}
				CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			}
			break;
		}
		case 'w':{
			unsigned long int chan;
			size_t length = 1, i;
			char *end, ch;
			const char *p;
			ptr++;
			
			// Get the channel to be written:
			errno = 0;
			chan = strtoul(ptr, &end, 16);
			CHECK_STATUS(errno, FLP_BAD_HEX, cleanup);

			// Ensure that it's 0-127
			CHECK_STATUS(chan > 127, FLP_CHAN_RANGE, cleanup);
			ptr = end;

			// There must be a space now:
			CHECK_STATUS(*ptr != ' ', FLP_ILL_CHAR, cleanup);

			// Now either a quote or a hex digit
		   ch = *++ptr;
			if ( ch == '"' || ch == '\'' ) {
				uint16 checksum = 0x0000;

				// Get the file to read bytes from:
				ptr++;
				p = ptr;
				while ( *p != ch && *p != '\0' ) {
					p++;
				}
				CHECK_STATUS(*p == '\0', FLP_UNTERM_STRING, cleanup);
				fileName = malloc((size_t)(p - ptr + 1));
				CHECK_STATUS(!fileName, FLP_NO_MEMORY, cleanup);
				CHECK_STATUS(p - ptr == 0, FLP_EMPTY_STRING, cleanup);
				strncpy(fileName, ptr, (size_t)(p - ptr));
				fileName[p - ptr] = '\0';
				ptr = p + 1;  // skip over closing quote

				// Open file for reading
				file = fopen(fileName, "rb");
				CHECK_STATUS(!file, FLP_CANNOT_LOAD, cleanup);
				free(fileName);
				fileName = NULL;
				
				#ifdef WIN32
					QueryPerformanceCounter(&tvStart);
					status = doWrite(handle, (uint8)chan, file, &length, &checksum, error);
					QueryPerformanceCounter(&tvEnd);
					totalTime = (double)(tvEnd.QuadPart - tvStart.QuadPart);
					totalTime /= freq.QuadPart;
					speed = (double)length / (1024*1024*totalTime);
				#else
					gettimeofday(&tvStart, NULL);
					status = doWrite(handle, (uint8)chan, file, &length, &checksum, error);
					gettimeofday(&tvEnd, NULL);
					startTime = tvStart.tv_sec;
					startTime *= 1000000;
					startTime += tvStart.tv_usec;
					endTime = tvEnd.tv_sec;
					endTime *= 1000000;
					endTime += tvEnd.tv_usec;
					totalTime = (double)(endTime - startTime);
					totalTime /= 1000000;  // convert from uS to S.
					speed = (double)length / (1024*1024*totalTime);
				#endif
				if ( enableBenchmarking ) {
					printf(
						"Wrote "PFSZD" bytes (checksum 0x%04X) to channel %lu at %f MiB/s\n",
						length, checksum, chan, speed);
				}
				CHECK_STATUS(status, status, cleanup);

				// Close the file
				fclose(file);
				file = NULL;
			} else if ( isHexDigit(ch) ) {
				// Read a sequence of hex bytes to write
				uint8 *dataPtr;
				p = ptr + 1;
				while ( isHexDigit(*p) ) {
					p++;
				}
				CHECK_STATUS((p - ptr) & 1, FLP_ODD_DIGITS, cleanup);
				length = (size_t)(p - ptr) / 2;
				data = malloc(length);
				dataPtr = data;
				for ( i = 0; i < length; i++ ) {
					getHexByte(dataPtr++);
					ptr += 2;
				}
				#ifdef WIN32
					QueryPerformanceCounter(&tvStart);
					fStatus = flWriteChannel(handle, (uint8)chan, length, data, error);
					QueryPerformanceCounter(&tvEnd);
					totalTime = (double)(tvEnd.QuadPart - tvStart.QuadPart);
					totalTime /= freq.QuadPart;
					speed = (double)length / (1024*1024*totalTime);
				#else
					gettimeofday(&tvStart, NULL);
					fStatus = flWriteChannel(handle, (uint8)chan, length, data, error);
					gettimeofday(&tvEnd, NULL);
					startTime = tvStart.tv_sec;
					startTime *= 1000000;
					startTime += tvStart.tv_usec;
					endTime = tvEnd.tv_sec;
					endTime *= 1000000;
					endTime += tvEnd.tv_usec;
					totalTime = (double)(endTime - startTime);
					totalTime /= 1000000;  // convert from uS to S.
					speed = (double)length / (1024*1024*totalTime);
				#endif
				if ( enableBenchmarking ) {
					printf(
						"Wrote "PFSZD" bytes (checksum 0x%04X) to channel %lu at %f MiB/s\n",
						length, calcChecksum(data, length), chan, speed);
				}
				CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
				free(data);
				data = NULL;
			} else {
				FAIL(FLP_ILL_CHAR, cleanup);
			}
			break;
		}
		case '+':{
			uint32 conduit;
			char *end;
			ptr++;

			// Get the conduit
			errno = 0;
			conduit = (uint32)strtoul(ptr, &end, 16);
			CHECK_STATUS(errno, FLP_BAD_HEX, cleanup);

			// Ensure that it's 0-127
			CHECK_STATUS(conduit > 255, FLP_CONDUIT_RANGE, cleanup);
			ptr = end;

			// Only two valid chars at this point:
			CHECK_STATUS(*ptr != '\0' && *ptr != ';', FLP_ILL_CHAR, cleanup);

			fStatus = flSelectConduit(handle, (uint8)conduit, error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			break;
		}
		default:
			FAIL(FLP_ILL_CHAR, cleanup);
		}
	} while ( *ptr == ';' );
	CHECK_STATUS(*ptr != '\0', FLP_ILL_CHAR, cleanup);

	dump(0x00000000, dataFromFPGA.data, dataFromFPGA.length);

cleanup:
	bufDestroy(&dataFromFPGA);
	if ( file ) {
		fclose(file);
	}
	free(fileName);
	free(data);
	if ( retVal > FLP_LIBERR ) {
		const int column = (int)(ptr - line);
		int i;
		fprintf(stderr, "%s at column %d\n  %s\n  ", errMessages[retVal], column, line);
		for ( i = 0; i < column; i++ ) {
			fprintf(stderr, " ");
		}
		fprintf(stderr, "^\n");
	}
	return retVal;
}

static const char *nibbles[] = {
	"0000",  // '0'
	"0001",  // '1'
	"0010",  // '2'
	"0011",  // '3'
	"0100",  // '4'
	"0101",  // '5'
	"0110",  // '6'
	"0111",  // '7'
	"1000",  // '8'
	"1001",  // '9'

	"XXXX",  // ':'
	"XXXX",  // ';'
	"XXXX",  // '<'
	"XXXX",  // '='
	"XXXX",  // '>'
	"XXXX",  // '?'
	"XXXX",  // '@'

	"1010",  // 'A'
	"1011",  // 'B'
	"1100",  // 'C'
	"1101",  // 'D'
	"1110",  // 'E'
	"1111"   // 'F'
};

int main(int argc, char *argv[]) {
	ReturnCode retVal = FLP_SUCCESS, pStatus;
	struct arg_str *ivpOpt = arg_str0("i", "ivp", "<VID:PID>", "            vendor ID && product ID (e.g 04B4:8613)");
	struct arg_str *vpOpt = arg_str1("v", "vp", "<VID:PID[:DID]>", "       VID, PID && opt. dev ID (e.g 1D50:602B:0001)");
	struct arg_str *fwOpt = arg_str0("f", "fw", "<firmware.hex>", "        firmware to RAM-load (or use std fw)");
	struct arg_str *portOpt = arg_str0("d", "ports", "<bitCfg[,bitCfg]*>", " read/write digital ports (e.g B13+,C1-,B2?)");
	struct arg_str *queryOpt = arg_str0("q", "query", "<jtagBits>", "         query the JTAG chain");
	struct arg_str *progOpt = arg_str0("p", "program", "<config>", "         program a device");
	struct arg_uint *conOpt = arg_uint0("c", "conduit", "<conduit>", "        which comm conduit to choose (default 0x01)");
	struct arg_str *actOpt = arg_str0("a", "action", "<actionString>", "    a series of CommFPGA actions");
	struct arg_lit *shellOpt  = arg_lit0("s", "shell", "                    start up an interactive CommFPGA session");
	struct arg_lit *benOpt  = arg_lit0("b", "benchmark", "                enable benchmarking & checksumming");
	struct arg_lit *rstOpt  = arg_lit0("r", "reset", "                    reset the bulk endpoints");

	struct arg_lit *myOpt  = arg_lit0("y", "lab", "                    lab function");
	
	struct arg_str *dumpOpt = arg_str0("l", "dumploop", "<ch:file.bin>", "   write data from channel ch to file");
	struct arg_lit *helpOpt  = arg_lit0("h", "help", "                     print this help && exit");
	struct arg_str *eepromOpt  = arg_str0(NULL, "eeprom", "<std|fw.hex|fw.iic>", "   write firmware to FX2's EEPROM (!!)");
	struct arg_str *backupOpt  = arg_str0(NULL, "backup", "<kbitSize:fw.iic>", "     backup FX2's EEPROM (e.g 128:fw.iic)\n");
	struct arg_end *endOpt   = arg_end(20);
	void *argTable[] = {
		ivpOpt, vpOpt, fwOpt, portOpt, queryOpt, progOpt, conOpt, actOpt,myOpt,
		shellOpt, benOpt, rstOpt, dumpOpt, helpOpt, eepromOpt, backupOpt, endOpt 
	};
	const char *progName = "flcli";
	int numErrors;
	struct FLContext *handle = NULL;
	FLStatus fStatus;
	const char *error = NULL;
	const char *ivp = NULL;
	const char *vp = NULL;
	bool isNeroCapable, isCommCapable;
	uint32 numDevices, scanChain[16], i;
	const char *line = NULL;
	uint8 conduit = 0x01;

	if ( arg_nullcheck(argTable) != 0 ) {
		fprintf(stderr, "%s: insufficient memory\n", progName);
		FAIL(1, cleanup);
	}

	numErrors = arg_parse(argc, argv, argTable);

	if ( helpOpt->count > 0 ) {
		printf("FPGALink Command-Line Interface Copyright (C) 2012-2014 Chris McClelland\n\nUsage: %s", progName);
		arg_print_syntax(stdout, argTable, "\n");
		printf("\nInteract with an FPGALink device.\n\n");
		arg_print_glossary(stdout, argTable,"  %-10s %s\n");
		FAIL(FLP_SUCCESS, cleanup);
	}

	if ( numErrors > 0 ) {
		arg_print_errors(stdout, endOpt, progName);
		fprintf(stderr, "Try '%s --help' for more information.\n", progName);
		FAIL(FLP_ARGS, cleanup);
	}

	fStatus = flInitialise(0, &error);
	CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);

	vp = vpOpt->sval[0];

	printf("Attempting to open connection to FPGALink device %s...\n", vp);
	fStatus = flOpen(vp, &handle, NULL);
	if ( fStatus ) {
		if ( ivpOpt->count ) {
			int count = 60;
			uint8 flag;
			ivp = ivpOpt->sval[0];
			printf("Loading firmware into %s...\n", ivp);
			if ( fwOpt->count ) {
				fStatus = flLoadCustomFirmware(ivp, fwOpt->sval[0], &error);
			} else {
				fStatus = flLoadStandardFirmware(ivp, vp, &error);
			}
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			
			printf("Awaiting renumeration");
			flSleep(1000);
			do {
				printf(".");
				fflush(stdout);
				fStatus = flIsDeviceAvailable(vp, &flag, &error);
				CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
				flSleep(250);
				count--;
			} while ( !flag && count );
			printf("\n");
			if ( !flag ) {
				fprintf(stderr, "FPGALink device did not renumerate properly as %s\n", vp);
				FAIL(FLP_LIBERR, cleanup);
			}

			printf("Attempting to open connection to FPGLink device %s again...\n", vp);
			fStatus = flOpen(vp, &handle, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		} else {
			fprintf(stderr, "Could not open FPGALink device at %s && no initial VID:PID was supplied\n", vp);
			FAIL(FLP_ARGS, cleanup);
		}
	}

	printf(
		"Connected to FPGALink device %s (firmwareID: 0x%04X, firmwareVersion: 0x%08X)\n",
		vp, flGetFirmwareID(handle), flGetFirmwareVersion(handle)
	);

	if ( eepromOpt->count ) {
		if ( !strcmp("std", eepromOpt->sval[0]) ) {
			printf("Writing the standard FPGALink firmware to the FX2's EEPROM...\n");
			fStatus = flFlashStandardFirmware(handle, vp, &error);
		} else {
			printf("Writing custom FPGALink firmware from %s to the FX2's EEPROM...\n", eepromOpt->sval[0]);
			fStatus = flFlashCustomFirmware(handle, eepromOpt->sval[0], &error);
		}
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
	}

	if (myOpt->count ) {
		uint8 channel_to_be_read;
		int array[8][4];
		int once = 1;
		int j = 0;
		while(1){
		int e = 0;
		char* cordinates;

		char* enc_cordinates;

		char* k = "11111111111111111111111111111111" ;
		
		uint8* input6 ; 
		input6 = malloc(40) ;

		uint32 reqlength ;
		uint32 actlength ;

		const uint8 *recvvvdata ;
		recvvvdata = malloc(32) ; 
		
		//flReadChannelAsyncSubmit(handle,4,4,input6,&error) ;

		//flReadChannel(handle,4,5, input6,&error);

		//waitFor(1) ;

		//flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;
		//printf("%d,%d,%d,%d\n", input6[4] , input6[3] , input6[2] , input6[1] );
		//printf("%d,%d,%d,%d\n", recvvvdata[4] , recvvvdata[1] , recvvvdata[2] , recvvvdata[3] );
		//waitFor(1) ;
		//continue ;  
		/// Polling.		
		for(uint8 i = j;i<64;i++){
		printf("checking%d\n" , i) ; 
		//flReadChannel(handle,2*i,5, input6,&error);	
		// get advertised data.
		flReadChannelAsyncSubmit(handle,(uint8)2*i,5,input6,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;

		char * buffer2 = malloc(32);
		dec2bin(input6[4],buffer2);
		cordinates = dec(k,buffer2);
		printf("%s\n" , buffer2) ; 
		enc_cordinates = enc(k,cordinates);
		uint8* enc_cor = malloc(32);
		enc_cor[0]=0;
		enc_cor[1]=0;
		enc_cor[2]=0;
		for(int j=0;j<32;j++)
		{
 			enc_cor[3] *= 2;
		 if (*enc_cordinates++ == '1') enc_cor[3] += 1;
		}
		/// resend data.
		flWriteChannel(handle,2*i+1,4,enc_cor,&error);
		uint8* input3 = malloc(40);
		//flReadChannel(handle,2*i,5, input3,&error);
		// read ack again.
		flReadChannelAsyncSubmit(handle,2*i,5,input3,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
		
		dec_array(input3,k);
		printf("ack received %d\n", input3[4]);

		if(input3[1]==255 && input3[2]==255 && input3[3]==255 && input3[4]==255){
		// got required ack. Found the channel to read.
			channel_to_be_read = 2*i;
			j=i;
			input3[0]=255;
			printf("%d\n" , channel_to_be_read) ; 
			enc_array(input3,k);
			flWriteChannel(handle,2*i+1,4,input3,&error);
			break;
		}
		else{
			waitFor(5);
			// wait for more time.
			uint8* input7 = malloc(40);
			//flReadChannel(handle,2*i,5, input7,&error);
			flReadChannelAsyncSubmit(handle,2*i,5,input7,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
		
			printf("ack received %d\n", input3[4]);
			if(input7[1]==255 && input7[2]==255 && input7[3]==255 && input7[4]==255){
			// got required ack. Found the channel to read.
			printf("%d\n" , channel_to_be_read) ; 
			channel_to_be_read = 2*i;
			j=i;
			input7[0]=255;
			flWriteChannel(handle,2*i+1,4,input7,&error);
			break;

		}
		printf("fail %d\n" , i) ; 
		}
		}
		uint32 count2 = 5;
	/*	int r = 0;
		while(2>1)
		{
		uint32 count2 = 5;
		r++;
		if(r!=1){
		
		const char** error1;			
		uint8* input2 ; 
		
		input2 = malloc(40) ; 
		const char** error2;
		
		flReadChannel(handle,channel_to_be_read,count2, input2,&error1);
		
		
		//input[0] = 8;
		//int c = (int)input[0];
		char * buffer = malloc(32);
		printf("%d\n%d\n%d\n%d\ne" , input2[1] , input2[2] , input2[3], input2[4]) ;  		
		dec2bin(input2[4],buffer);
		
		//char* cordinates = dec(k,buffer);

		//char* enc_cordinates = enc(k,cordinates);

		uint8* enc_cor = malloc(32);

		enc_cor[0]=0;
		enc_cor[1]=0;
		enc_cor[2]=0;
		for(int j=0;j<32;j++)
		{
 			enc_cor[3] *= 2;
		 if (*enc_cordinates++ == '1') enc_cor[3] += 1;
		}


		flWriteChannel(handle,channel_to_be_read+1,4,enc_cor,error2);

		uint8* input3 = malloc(40);
		flReadChannel(handle,channel_to_be_read,count2, input3,&error);

		uint8* next = malloc(32);
		for(size_t i =0;i<4;i++ ) next[i] = 255;

		flWriteChannel(handle,channel_to_be_read+1,4,next,&error);
}*/
		char *p1;
    	char *p2;
    	// get the coordinates.
    	p1 = strndup(cordinates+24,27);
    	p2 = strndup(cordinates+28,31);
    	
    	int total1 = 0;
		for(int i=0;i<4;i++)
		{
 			total1 *= 2;
		 if (*p1++ == '1') total1 += 1;
		}

		int total2 = 0;
		for(int i=0;i<4;i++)
		{
 			total2 *= 2;
		 if (*p2++ == '1') total2 += 1;
		}
	
		

		FILE* my_file = fopen("track_data.csv","r");
    	struct my_record records[100];
    	size_t count7 = 0;
    
    

    	for (; count7 < 100; ++count7)
    	{
        	int got = fscanf(my_file, "%d,%d,%d,%d,%d", &records[count7].xcor, &records[count7].ycor
								,&records[count7].x, &records[count7].y ,&records[count7].z ); 
       	if (got != 5) break; // wrong number of tokens - maybe end of file
    	}

    /// load array from csv file using coordinates.
    int inp1=total1,inp2=total2;
    //inp1 = (inp1 - inp2)/16;
    //inp1 = atoi(argv[1]);;
    //inp2 = atoi(argv[2]);;
    printf("%d,%d\n",inp1,inp2 );
    int array[8][4];
    if(once == 1){
    for (size_t i = 0; i < 8; i++){
        array[i][0]=(int)i;
        array[i][1]=0;
        array[i][2]=0;
        array[i][3]=0;
    }



    for(size_t f = 0 ; f < count7 ; f++)
    {   if(inp1 == records[f].xcor && inp2 == records[f].ycor){
        //printf("%d %d %d %d %d\n" , records[f].xcor, records[f].ycor,records[f].x, records[f].y ,records[f].z) ;
        array[records[f].x][1]=1;
        array[records[f].x][2]=records[f].y;
        array[records[f].x][3]=records[f].z;
        }
    }
	once--;
	}
    size_t len=8;
    uint8*  data;
	data = malloc(len);

    for (size_t i = 0; i < 8; i++){
        int k = array[i][1]*128+array[i][2]*64+array[i][0]*8+array[i][3];
        data[i] = k;
    }
    
    

    for(size_t i=0;i<8;i++){
    	int k2 = (int)data[i];
    	char * temp;
    	temp = malloc(32);
    	dec2bin(k2,temp);
    	char * message = enc(k,temp);
    	
    	
    	data[i]=0;
		for(int j=0;j<32;j++)
		{
 			data[i] *= 2;
		 if (*message++ == '1') data[i] += 1;
		}
    }
	for (size_t i = 0; i < 8; i++){
        printf("%d,%d,%d,%d,%d\n",array[i][0],array[i][1],array[i][2],array[i][3],data[i]);
       
    }    
    fclose(my_file);
    	
		uint8 channel=channel_to_be_read+1;
		const char** error;
		/////// send 4 bytes.
		flWriteChannel(handle,channel,1,&data[0],error);
		flWriteChannel(handle,channel,1,&data[1],error);
		flWriteChannel(handle,channel,1,&data[2],error);
		flWriteChannel(handle,channel,1,&data[3],error);
		uint8* input4 = malloc(40);
		//flReadChannel(handle,channel_to_be_read,5, input4,&error);
		// Reading ack and checking if it is okay.
		flReadChannelAsyncSubmit(handle,channel_to_be_read,5,input4,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
		
		dec_array(input4,k);
		int counter = 0;
		if(input4[1]==255 && input4[2]==255 && input4[3]==255 && input4[4]==255){

		}
		else{
			while(2>1){

			counter++;
			waitFor(1);
			//flReadChannel(handle,channel_to_be_read,count2, input4,&error);
			flReadChannelAsyncSubmit(handle,channel_to_be_read,5,input4,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
		
			dec_array(input4,k);
			if(input4[1]==255 && input4[2]==255 && input4[3]==255 && input4[4]==255){
				break;
			}
			else if(counter == 256){
				e = 1;
				break;

			}}			
		}

		// second 4 bytes

		flWriteChannel(handle,channel,1,&data[4],error);
		flWriteChannel(handle,channel,1,&data[5],error);
		flWriteChannel(handle,channel,1,&data[6],error);
		flWriteChannel(handle,channel,1,&data[7],error);
		uint8* input5 = malloc(40);


		//flReadChannel(handle,channel_to_be_read,count2, input5,&error);


		FLStatus name2 = flReadChannelAsyncSubmit(handle,channel_to_be_read,5,input5,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;
		

		dec_array(input5,k);
		int counter2 = 0;
		if(input5[1]==255 && input5[2]==255 && input5[3]==255 && input5[4]==255){

		}
		else{
			while(2>1){
			counter2++;
			waitFor(1);
			//flReadChannel(handle,channel_to_be_read,count2, input5,&error);
		// reading ack, final ack.
			flReadChannelAsyncSubmit(handle,channel_to_be_read,5,input5,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
			dec_array(input4,k);
			if(input5[1]==255 && input5[2]==255 && input5[3]==255 && input5[4]==255){
				break;
			}
			else if(counter2 == 256){
				e = 1;
				break;
			}}			
		}		

		
		/// final ack to the board.
		uint8* ack = malloc(32);
		ack[0]=255;
		ack[1]=255;
		ack[2]=255;
		ack[3]=255;
		enc_array(ack,k);
		flWriteChannel(handle,channel,4,ack,error);
		
//////////// waiting for S3.
		 waitFor(45);// this should be less than board cycle , this should be more
		 // if you want to update
		// printf("Waiting for data,\n");
		uint8* change = malloc(40);
/////////// Read byte in S3.
		flReadChannelAsyncSubmit(handle,channel_to_be_read,5,change,&error) ;
		flReadChannelAsyncAwait(handle,&recvvvdata,&actlength,&actlength,&error) ;		
		//CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		printf("%d:%d:%d:%d\n", change[1],change[2],change[3],change[4]);
		if(change[3] !=255){
		/////// check if S3 is activated.
			int i = ((change[4] % 64) - (change[4] % 8))/ 8 ; 
			array[i][0] = i ; 
			array[i][1] = change[4] / 128 ;
			array[i][2] =  (change[4] / 64) - 2 * array[i][1] ;
			array[i][3] = change[4] % 8 ; 
			printf("changing the direction %d entry to %d\n", i , change[4]);
			}

		waitFor(60); // waiting for S4, S5 , S6.
	

	}
		
	}

	if ( backupOpt->count ) {
		const char *fileName;
		const uint32 kbitSize = strtoul(backupOpt->sval[0], (char**)&fileName, 0);
		if ( *fileName != ':' ) {
			fprintf(stderr, "%s: invalid argument to option --backup=<kbitSize:fw.iic>\n", progName);
			FAIL(FLP_ARGS, cleanup);
		}
		fileName++;
		printf("Saving a backup of %d kbit from the FX2's EEPROM to %s...\n", kbitSize, fileName);
		fStatus = flSaveFirmware(handle, kbitSize, fileName, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
	}

	if ( rstOpt->count ) {
		// Reset the bulk endpoints (only needed in some virtualised environments)
		fStatus = flResetToggle(handle, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
	}

	if ( conOpt->count ) {
		conduit = (uint8)conOpt->ival[0];
	}

	isNeroCapable = flIsNeroCapable(handle);
	isCommCapable = flIsCommCapable(handle, conduit);

	if ( portOpt->count ) {
		uint32 readState;
		char hex[9];
		const uint8 *p = (const uint8 *)hex;
		printf("Configuring ports...\n");
		fStatus = flMultiBitPortAccess(handle, portOpt->sval[0], &readState, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		sprintf(hex, "%08X", readState);
		printf("Readback:   28   24   20   16    12    8    4    0\n          %s", nibbles[*p++ - '0']);
		printf(" %s", nibbles[*p++ - '0']);
		printf(" %s", nibbles[*p++ - '0']);
		printf(" %s", nibbles[*p++ - '0']);
		printf("  %s", nibbles[*p++ - '0']);
		printf(" %s", nibbles[*p++ - '0']);
		printf(" %s", nibbles[*p++ - '0']);
		printf(" %s\n", nibbles[*p++ - '0']);
		flSleep(100);
	}

	if ( queryOpt->count ) {
		if ( isNeroCapable ) {
			fStatus = flSelectConduit(handle, 0x00, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fStatus = jtagScanChain(handle, queryOpt->sval[0], &numDevices, scanChain, 16, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			if ( numDevices ) {
				printf("The FPGALink device at %s scanned its JTAG chain, yielding:\n", vp);
				for ( i = 0; i < numDevices; i++ ) {
					printf("  0x%08X\n", scanChain[i]);
				}
			} else {
				printf("The FPGALink device at %s scanned its JTAG chain but did not find any attached devices\n", vp);
			}
		} else {
			fprintf(stderr, "JTAG chain scan requested but FPGALink device at %s does not support NeroProg\n", vp);
			FAIL(FLP_ARGS, cleanup);
		}
	}

	if ( progOpt->count ) {
		printf("Programming device...\n");
		if ( isNeroCapable ) {
			fStatus = flSelectConduit(handle, 0x00, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fStatus = flProgram(handle, progOpt->sval[0], NULL, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		} else {
			fprintf(stderr, "Program operation requested but device at %s does not support NeroProg\n", vp);
			FAIL(FLP_ARGS, cleanup);
		}
	}

	if ( benOpt->count ) {
		enableBenchmarking = true;
	}
	
	if ( actOpt->count ) {
		printf("Executing CommFPGA actions on FPGALink device %s...\n", vp);
		if ( isCommCapable ) {
			uint8 isRunning;
			fStatus = flSelectConduit(handle, conduit, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fStatus = flIsFPGARunning(handle, &isRunning, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			if ( isRunning ) {
				pStatus = parseLine(handle, actOpt->sval[0], &error);
				CHECK_STATUS(pStatus, pStatus, cleanup);
			} else {
				fprintf(stderr, "The FPGALink device at %s is not ready to talk - did you forget --program?\n", vp);
				FAIL(FLP_ARGS, cleanup);
			}
		} else {
			fprintf(stderr, "Action requested but device at %s does not support CommFPGA\n", vp);
			FAIL(FLP_ARGS, cleanup);
		}
	}

	if ( dumpOpt->count ) {
		const char *fileName;
		unsigned long chan = strtoul(dumpOpt->sval[0], (char**)&fileName, 10);
		FILE *file = NULL;
		const uint8 *recvData;
		uint32 actualLength;
		if ( *fileName != ':' ) {
			fprintf(stderr, "%s: invalid argument to option -l|--dumploop=<ch:file.bin>\n", progName);
			FAIL(FLP_ARGS, cleanup);
		}
		fileName++;
		printf("Copying from channel %lu to %s", chan, fileName);
		file = fopen(fileName, "wb");
		CHECK_STATUS(!file, FLP_CANNOT_SAVE, cleanup);
		sigRegisterHandler();
		fStatus = flSelectConduit(handle, conduit, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		fStatus = flReadChannelAsyncSubmit(handle,
		 (uint8)chan, 22528, NULL, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		do {
			fStatus = flReadChannelAsyncSubmit(handle, (uint8)chan, 22528, NULL, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fStatus = flReadChannelAsyncAwait(handle, &recvData, &actualLength, &actualLength, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fwrite(recvData, 1, actualLength, file);
			printf(".");
		} while ( !sigIsRaised() );
		printf("\nCaught SIGINT, quitting...\n");
		fStatus = flReadChannelAsyncAwait(handle, &recvData, &actualLength, &actualLength, &error);
		CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
		fwrite(recvData, 1, actualLength, file);
		fclose(file);
	}

	if ( shellOpt->count ) {
		printf("\nEntering CommFPGA command-line mode:\n");
		if ( isCommCapable ) {
		   uint8 isRunning;
			fStatus = flSelectConduit(handle, conduit, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			fStatus = flIsFPGARunning(handle, &isRunning, &error);
			CHECK_STATUS(fStatus, FLP_LIBERR, cleanup);
			if ( isRunning ) {
				do {
					do {
						line = readline("> ");
					} while ( line && !line[0] );
					if ( line && line[0] && line[0] != 'q' ) {
						add_history(line);
						pStatus = parseLine(handle, line, &error);
						CHECK_STATUS(pStatus, pStatus, cleanup);
						free((void*)line);
					}
				} while ( line && line[0] != 'q' );
			} else {
				fprintf(stderr, "The FPGALink device at %s is not ready to talk - did you forget --xsvf?\n", vp);
				FAIL(FLP_ARGS, cleanup);
			}
		} else {
			fprintf(stderr, "Shell requested but device at %s does not support CommFPGA\n", vp);
			FAIL(FLP_ARGS, cleanup);
		}
	}

cleanup:
	free((void*)line);
	flClose(handle);
	if ( error ) {
		fprintf(stderr, "%s\n", error);
		flFreeError(error);
	}
	return retVal;
}
