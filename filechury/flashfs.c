#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"


FlashFileTbl FFt;
int first=0;

int fs_open(char *filename);
int fs_close(char * filename);
int fs_seek(char *filename, long offset);
int fs_read(char *filename, char *buf, int size);
int fs_write(char *filename, char *buf, int size);
int get_file_index(char *filename){
	int i=0;
	for(;i<FFt.numFiles;++i){
		if(strcmp(FFt.entry[i].fname,filename)==0)
			return i;
	}
	return -1;
}

int fs_open ( char * filename){
	if(first++==0){
		ftl_open();
	}
	int check=0;
	int i=0;
	for(;i<FFt.numFiles;++i){
		if(strcmp(filename,FFt.entry[i].fname)==0){
			check++;
			break;
		}
	}
	if(check==0){
		strcpy(FFt.entry[FFt.numFiles].fname,filename);
		FFt.numFiles++;
	}
	else{
		FFt.entry[i].curFilePos=0;
	}
	return 0;
}

int fs_close(char * filename){
	//do nothing
	return 0;
}
int fs_seek(char *filename, long offset){
	int i = get_file_index(filename);
	if(FFt.entry[i].curFilePos+offset<0
			||FFt.entry[i].curFilePos+offset>FFt.entry[i].fsize){
		printf("fs_seek error : range error\n");
		return -1;
	}

	else{
		FFt.entry[i].curFilePos+=offset;
	}
//	printf("%d",FFt.entry[i].curFilePos);
	return 0;
}
int fs_read(char *filename, char *buf, int size){
	int i = get_file_index(filename);
	if(FFt.entry[i].curFilePos+size>FFt.entry[i].fsize)return -1;
	char temp[513];
	int lpos = FFt.entry[i].curFilePos%512;
	int index  = FFt.entry[i].curFilePos/512;

	ftl_read(FFt.entry[i].sectors[index++],temp);
//	printf("fr : %s",temp);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s",temp+FFt.entry[i].curFilePos%512);
//	buf[strlen(buf)]='\0';
	if(FFt.entry[i].curFilePos+size<=512){
		buf[size]='\0';
		return 0;
	}
	size-=lpos;
	while(size > 512){
		ftl_read(FFt.entry[i].sectors[index++],temp);
		strcat(buf,temp);
		size-=512;
	}
	ftl_read(FFt.entry[i].sectors[index++],temp);
	temp[size]='\0';
	strcat(buf,temp);
	return 0;
}
int fs_write(char *filename, char *buf, int size){
	int i = get_file_index(filename);
	char temp[513];
	int index  = FFt.entry[i].curFilePos/512;
	int lpos = FFt.entry[i].curFilePos%512;
//	printf("fw : %s\n",buf);

	if(FFt.entry[i].curFilePos+size>512*20){
		printf("write error : file size over");
		return -1;
	}
	if(FFt.entry[i].fsize < FFt.entry[i].curFilePos+size){//new input is bigger
		FFt.entry[i].fsize =  FFt.entry[i].curFilePos+size;

//		printf("1");

		int j = (size + FFt.entry[i].curFilePos)/512;
		int iter;
		if(index > FFt.entry[i].numSectors){
			FFt.entry[i].numSectors++;
			FFt.entry[i].sectors[FFt.entry[i].numSectors] = FFt.nextEmptyLsn++;
		}
//		printf("1");
//		printf("index : %d\n",FFt.entry[i].sectors[index]);
		ftl_read(FFt.entry[i].sectors[index],temp);
//		printf("\ntemp:%s\n",temp);
//		printf("test : %s",temp);
		sprintf(temp+lpos,"%s",buf);
//		printf("\n\nfwif : %s\n\n",temp);
		temp[512] = '\0';

//		printf("1");

//		printf("%d\n%s",FFt.entry[i].sectors[index++],temp);
		ftl_write(FFt.entry[i].sectors[index++],temp);

//		printf("\n%s1\n",temp);
		lpos  =512 -lpos;
		if(FFt.entry[i].curFilePos+size<=512)goto end;
		printf("2");
		for(iter = 1 ;iter<j-1;++iter){
			memset(temp,0,512);
			if(index > FFt.entry[i].numSectors){
				FFt.entry[i].numSectors++;
				FFt.entry[i].sectors[FFt.entry[i].numSectors] = FFt.nextEmptyLsn++;
			}
			sprintf(temp,"%s",buf+lpos);
			temp[512] = '\0';
			ftl_write(FFt.entry[i].sectors[index++],temp);
			lpos+=512;
		}

//		printf("3");
		memset(temp,0,512);
		if(index > FFt.entry[i].numSectors){
			FFt.entry[i].numSectors++;
			FFt.entry[i].sectors[FFt.entry[i].numSectors] = FFt.nextEmptyLsn++;
		}
		sprintf(temp,"%s",buf+lpos);
		temp[512] = '\0';
		ftl_write(FFt.entry[i].sectors[index++],temp);

//		printf("4");
	}
	else{
		int j = (size + FFt.entry[i].curFilePos)/512;
		int iter;
		ftl_read(FFt.entry[i].sectors[index],temp);
		sprintf(temp+lpos,"%s",buf);
		temp[512] = '\0';
		ftl_write(FFt.entry[i].sectors[index++],temp);
		lpos  =512 -lpos;

		if(FFt.entry[i].curFilePos+size<=512)goto end;
		for(iter = 1 ;iter<j-1;++iter){
			memset(temp,0,512);
			sprintf(temp,"%s",buf+lpos);
			temp[512] = '\0';
			ftl_write(FFt.entry[i].sectors[index++],temp);
			lpos+=512;
		}
		memset(temp,0,512);
		sprintf(temp,"%s",buf+lpos);
		temp[512] = '\0';
		ftl_write(FFt.entry[i].sectors[index++],temp);

	}
//	if(index > FFt.entry[i].numSectors){
//		FFt.entry[i].numSectors++;
//		FFt.entry[i].sectors[FFt.entry[i].numSectors] = FFt.nextEmptyLsn++;
//	}
end :
	FFt.entry[i].curFilePos +=size;
//	printf("%s\n",temp);
	return 0;
}
