/*
* my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다.
*
*/
#define MAX_INST 256
#define MAX_LINES 5000

#define MAX_COLUMNS 4
#define MAX_OPERAND 3

#define SECTOR_MAX 10

#define XXX 1
#define AAA 0
#define SSS 4
#define TTT 5
/*
* 기계어 목록 파일로 부터 정보를 받아와 생성하는 기계어 변환 테이블이다.
* 해당 기계어의 정보를 토큰으로 나눠 기록하고 있다.
*/
struct inst_struct {
	char *str;
	unsigned char op;
	int format;
	int ops;
};
typedef struct inst_struct inst_struct;
inst_struct inst[MAX_INST];
int inst_index;
//
//struct my_opcode{
//	unsigned char op;
//	int n;
//	int i;
//	int x;
//	int b;
//	int p;
//	int e;
//	int dist;
//};
//struct my_opcode myop;
/*
* 어셈블리 할 소스코드를 토큰 단위로 관리하는 테이블이다.
* 관리 정보는 소스 라인 단위로 관리되어진다.
*/
char *input_data[MAX_LINES];
static int line_num;

int label_num;

struct token_unit {
	char *label;
	char *operator_;
	char *operand[MAX_OPERAND];
	char *comment;
	int addr;	//address
	int length;//use bytes
	char *opcode;
};

typedef struct token_unit token;
token token_table[MAX_LINES];
static int token_line;

char  Modifi[SECTOR_MAX][MAX_LINES][10];// for EXTREF
static int Modifi_num[SECTOR_MAX];


struct Modification_record{// for objectcode
	int flag;
	char *name;
	int length;
	int address;
};

struct Modification_record Modi[SECTOR_MAX][MAX_LINES];
static int modi_num[SECTOR_MAX];
/*
* 심볼을 관리하는 구조체이다.
* 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
*/
struct symbol_unit {
	char symbol[10];
	int addr;
};

typedef struct symbol_unit symbol;
symbol sym_table[SECTOR_MAX][MAX_LINES];
static int symbol_line[SECTOR_MAX];

char * extdef[SECTOR_MAX][MAX_LINES];
static int extdef_num[SECTOR_MAX];


struct literal_unit{
	char * literal;
	int length;
	int address;
};

struct literal_unit liter[SECTOR_MAX];

static int locctr[SECTOR_MAX];

static int sectionIndex;
char * sections[SECTOR_MAX];
//--------------

static char *input_file;
static char *output_file;

int init_my_assembler(void);
static int assem_pass1(void);
static int assem_pass2(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int search_opcode(char *str);
void make_objectcode(char *file_name);
