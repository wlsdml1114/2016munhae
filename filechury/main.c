#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"

BlkMapTbl blkmaptbl;
extern FILE *devicefp;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void initialize_flash_memory();
void print_block(int pbn);
void print_blkmaptbl();

#include <stdio.h>
#include <string.h>
#include "blkmap.h"

FILE *devicefp;				// flash devi

FlashFileTbl FFt;

int main(int argc, char *argv[])
{
	// 아래 세 개의 변수는 테스트할 때 필요하면 사용하기 바람
//	FILE *workloadfp;
//	char sectorbuf[SECTOR_SIZE];
//	int lsn;

	// 가상 flash memory의 파일명은 'flashmemory'을 가정함
	devicefp = fopen("flashmemory", "w+b");

	if(devicefp == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	initialize_flash_memory();

//	ftl_open();
//	char * buffer;
//	buffer = (char *)malloc(sizeof(char)*PAGE_SIZE);
	fs_open("hi");
	char buf[100];
	memset(buf,65,100);
	buf[100]='\0';
//	printf("%s",buf);
	fs_write("hi",buf,100);
//	printf("%d",FFt.entry[0].curFilePos);
	memset(buf,66,100);
	buf[100]='\0';
	fs_write("hi",buf,100);
//	printf("%d",FFt.entry[0].curFilePos);
	memset(buf,67,100);
	buf[100]='\0';
//	fs_write("hi",buf,50*512+1);
	fs_write("hi",buf,100);
	fs_write("hi",buf,100);
	fs_write("hi",buf,100);
//	fs_write("hey",buf,100);
//	printf("%d",FFt.entry[0].curFilePos);
	fs_seek("hi",-350);
//	printf("%d",FFt.entry[0].curFilePos);
	fs_read("hi",buf,100);
//	printf("%d",FFt.entry[0].curFilePos);
	printf("%s\n",buf);
	fs_seek("hi",-100);
	fs_read("hi",buf,100);
	printf("%s\n",buf);
	fs_seek("hi",-100);
	fs_read("hi",buf,100);
	printf("%s\n",buf);
	fs_seek("hi",170);
	fs_read("hi",buf,100);
	printf("%s\n",buf);
	fs_open("hi");
	fs_seek("hi",50);
	fs_read("hi",buf,100);
	printf("%s\n",buf);
//	char buf[10] = "aaaasdf";
//	char aa[5] = "as";
//	strcat(buf,"123");
//	printf("%s\n",buf);
//	buf[3] = '\0';
//	printf("%s\n",buf);
//	strncpy(aa+2,buf,3);
//	printf("%s",aa);

//	if(fs_seek("hi",-1)==-1){
//		printf("error");
//	}
//	ftl_write(1,"finished");
//	ftl_write(130,"finished");
//	ftl_write(405,"finished");
//	ftl_write(799,"finished");
//	ftl_read(1,buffer);
//	printf("%s",buffer);
//	ftl_read(130,buffer);
//	printf("%s",buffer);
//	ftl_read(405,buffer);
//	printf("%s",buffer);
//	ftl_read(799,buffer);
//	printf("%s",buffer);
	//
	// ftl_write() 및 ftl_read() 테스트를 위한 코드를 자유자재로 만드세요
	//


	fclose(devicefp);

	return 0;
}
