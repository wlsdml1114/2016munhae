#ifndef	_BLOCK_MAPPING_H_
#define	_BLOCK_MAPPING_H_

#define TRUE			1
#define	FALSE			0

#define	SECTOR_SIZE		512
#define	SPARE_SIZE		16
#define	PAGE_SIZE		(SECTOR_SIZE+SPARE_SIZE)
#define SECTORS_PER_PAGE	1
#define	PAGES_PER_BLOCK		32						// 수정 가능
#define SECTORS_PER_BLOCK	(SECTORS_PER_PAGE*PAGES_PER_BLOCK)
#define	BLOCK_SIZE		(PAGE_SIZE*PAGES_PER_BLOCK)
#define	BLOCKS_PER_DEVICE	1024						// 수정 가능
#define	DEVICE_SIZE		(BLOCK_SIZE*BLOCKS_PER_DEVICE)
#define DATABLKS_PER_DEVICE	(BLOCKS_PER_DEVICE - 1)				// spare block로서 블록 하나를 사용
//
// 필요하면 상수를 추가해서 사용 가능함
//

//
// Flash file system 구현을 위한 상수 정의
//
#define MAX_FILE_NAME_SIZE	30	// 파일 이름의 최대 바이트 크기
// 아래의 두 상수는 수정 가능
#define MAX_FILE_SIZE		20	// 파일을 구성하는 최대 sector의 수
#define MAX_FILE_NUM		20	// 플래시 메모리에 저장할 수 있는 최대 파일의 수

//

//
// flash memory의 spare area를 다루기 위한 구조체
//
typedef struct
{
	int lsn;			// page에 데이터를 저장할 때 spare area에 lsn도 같이 저장함
	char dummy[SPARE_SIZE - 4];
} SpareData;

//
// FTL이 관리하는 address mapping table의 각 entry 구조체로서, 각 entry는 (lbn, pbn) 쌍으로 이루어지지만,
// 굳이 lbn을 둘 필요는 없음
//
typedef struct
{
//	int lbn;			// not necessary
	int pbn;
} BlkMapTblEntry;

//
// FTL이 관리하는 address mapping table을 위한 구조체
//
typedef struct
{
	BlkMapTblEntry entry[DATABLKS_PER_DEVICE];
} BlkMapTbl;



typedef struct
{
	char fname[MAX_FILE_NAME_SIZE];
	int fsize;		// 파일의 크기로 byte 단위로 표현됨
	int curFilePos;		// file position은 byte offset으로 표현하며, byte offset은 0부터 시작
	int numSectors;		// 파일을 구성하는 sector의 수
	int sectors[MAX_FILE_SIZE];	// 파일을 구성하는 sector의 집합
} FlashFileTblEntry;

typedef struct
{
	int numFiles;		// 현재까지 생성된 전체 파일의 수
	int nextEmptyLsn;	// empty sector 중 첫 번째 lsn을 가리키며 sector 할당 시 사용됨
	FlashFileTblEntry entry[MAX_FILE_NUM];
} FlashFileTbl;

int spareblock=1023;
#endif
