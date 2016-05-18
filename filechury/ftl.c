#define PRINT_FOR_DEBUG

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
int temp=0;


void ftl_open()
{
	int i;

	// initialize the address mapping table
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		blkmaptbl.entry[i].pbn =-1;
//		ramtbl.mapdir.entry[i].offset = 0;
	}

	//
	// 추가적으로 필요한 작업이 있으면 수행할 것
	//

	return;
}

//
// file system을 위한 write interfac
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'
//
void ftl_write(int lsn, char *sectorbuf)
{

#ifdef PRINT_FOR_DEBUG			// 필요 시 현재의 block mapping table을 출력해 볼 수 있음
	print_blkmaptbl();
#endif
	char * buffer;
	SpareData *spare;
	spare = (SpareData *)malloc(SPARE_SIZE);
	spare->lsn = lsn;
	buffer = (char *)malloc(sizeof(char)*PAGE_SIZE);
	char * sector;
	sector = (char *)malloc(sizeof(char)*SECTOR_SIZE);
	memcpy(buffer,sector,SECTOR_SIZE);
	memcpy(buffer,sectorbuf,strlen(sectorbuf));
	memcpy(buffer+SECTOR_SIZE,spare,SPARE_SIZE);
	int pbn = blkmaptbl.entry[lsn/32].pbn;
//	printf("sb : %s\n",sectorbuf);
	//다 찻을경우 예외처리
	if (pbn == -1&&temp<BLOCKS_PER_DEVICE-1){
		blkmaptbl.entry[lsn/32].pbn = temp++;
		int ppn = blkmaptbl.entry[lsn/32].pbn*32+ lsn%32;
//		printf("\nppn : %d\n",ppn);
		write(ppn,buffer);
	}else {
		int j=0;
		char *pagebuf;
		pagebuf = (char *)malloc(PAGE_SIZE);

		for(j=0;j<PAGES_PER_BLOCK;j++){
			if(j==lsn%32){
				write(j+spareblock*32,buffer);
				continue;
			}
			read(j+spareblock*32,pagebuf);
			write(j+spareblock*32,pagebuf);
		}
		erase(pbn);
		int imsi = blkmaptbl.entry[lsn/32].pbn;
		blkmaptbl.entry[lsn/32].pbn = spareblock;
		spareblock = imsi;
	}

	return;
}

//
// file system을 위한 read interface
// 'sectorbuf'가 가리키는 메모리의 크기는 'SECTOR_SIZE'
//
void ftl_read(int lsn, char *sectorbuf)
{
#ifdef PRINT_FOR_DEBUG			// 필요 시 현재의 block mapping table을 출력해 볼 수 있음
	print_blkmaptbl();
#endif
	if(blkmaptbl.entry[lsn/32].pbn==-1){
		return;
	}
	char *pagebuf;
	SpareData *sdata;

	pagebuf = (char *)malloc(PAGE_SIZE);
	sdata = (SpareData *)malloc(SPARE_SIZE);
	int ppn = blkmaptbl.entry[lsn/32].pbn*32+ lsn%32;
//	printf("\nppn : %d\n",ppn);
	if(read(ppn,pagebuf)==-1){
		printf("no record");
		return;
	}
	memcpy(sectorbuf, pagebuf, SECTOR_SIZE);
	memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
//	printf("fr%d : %s\n",ppn,sectorbuf);
	return;
}

//
// initialize flash memory where each byte are set to 'OxFF'
//
void initialize_flash_memory()
{
    	char *blockbuf;
	int i;

	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	free(blockbuf);

	return;
}

void print_block(int pbn)
{
	char *pagebuf;
	SpareData *sdata;
	int i;

	pagebuf = (char *)malloc(PAGE_SIZE);
	sdata = (SpareData *)malloc(SPARE_SIZE);

	printf("Physical Block Number: %d\n", pbn);

	for(i = pbn*PAGES_PER_BLOCK; i < (pbn+1)*PAGES_PER_BLOCK; i++)
	{
		read(i, pagebuf);
		memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
		printf("\t   %5d-[%7d]\n", i, sdata->lsn);
	}

	free(pagebuf);
	free(sdata);

	return;
}

void print_blkmaptbl()
{
	int i;

	printf("Block Mapping Table: \n");
	for(i = 0; i <= temp; i++)
	{
		if(blkmaptbl.entry[i].pbn >= 0)
		{
			printf("[%d %d]\n", i, blkmaptbl.entry[i].pbn);
		}
	}
}
