/*
* 화일명 : my_assembler.c
* 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
* 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
*
*/

/*
*
* 프로그램의 헤더를 정의한다.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* my_assembler 프로그램의 의존적인 데이터들이 정의된 곳이다.
*/

#include "my_assembler.h"

/* -----------------------------------------------------------------------------------
* 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
* 매계 : 실행 파일, 어셈블리 파일
* 반환 : 성공 = 0, 실패 = < 0
* 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다.
*		   또한 중간파일을 생성하지 않는다.
* -----------------------------------------------------------------------------------
*/
char opcode[MAX_LINES];//기계어 코드를 저장하기 위한 배열

int main(int args, char *arg[])
{
	if (init_my_assembler()< 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}
	if (assem_pass1() < 0) {
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}
	if (assem_pass2() < 0) {
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}

	make_objectcode(NULL) ;
	make_objectcode("output") ;
}
/* -----------------------------------------------------------------------------------
* 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다.
* 매계 : 없음
* 반환 : 정상종료 = 0 , 에러 발생 = -1
* 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기
*		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
*		   구현하였다.
* -----------------------------------------------------------------------------------
*/

int init_my_assembler(void)
{
	int result;

	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/

static int assem_pass1(void)
{
	int i;
	for(i=0;i<line_num;++i){
		if(input_data[i][0]=='.')continue;//.으로 시작하는 주석부분을 건너뛴다.
		token_parsing(i);
	}
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/

static int assem_pass2(void)
{
	int i;
	for(i=0;i<token_line;++i){
		opcode[i]=search_opcode(token_table[i]->operator_);
	}
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을
*        생성하는 함수이다.
* 매계 : 기계어 목록 파일
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : 기계어 목록파일 형식은 다음과 같다.
*
*	===============================================================================
*		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
*	===============================================================================
*
* -----------------------------------------------------------------------------------
*/

int init_inst_file(char *inst_file)
{
	FILE *in_fp;
	int i=0;
	if ((in_fp = fopen("inst.dat", "r")) == NULL)
		return -1;
	else {
		char ch[10];
		while(fscanf(in_fp,"%s",ch)==1){
			inst[i].str = (char *)malloc(sizeof(ch));
			strcpy(inst[i].str,ch);
			fscanf(in_fp,"%s",ch);
			inst[i].format = atoi(ch);
			fscanf(in_fp,"%hhx",&inst[i].op);
			fscanf(in_fp,"%s",ch);
			inst[i].ops = atoi(ch);
			fscanf(in_fp,"%s",ch);
			// printf("%s %hhx %d %d\n", inst[i].str,inst[i].op,inst[i].format,inst[i].ops);
			i++;
		}
	}
	fclose(in_fp);
	inst_index=i;
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다.
* 매계 : 어셈블리할 소스파일명
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/


int init_input_file(char *input_file)
{
	FILE *in_fp;
	char buffer[1024];
	if ((in_fp = fopen("input.txt", "r")) == NULL)
		return -1;
	else{
		while(fgets(buffer,1024,in_fp)){
			input_data[line_num] = (char*)malloc(sizeof(char)*strlen(buffer)+1);
			strcpy(input_data[line_num++],buffer);
		// 	char * temp;
		// 	int lpos=0,rpos=0;
			// printf("%s", buffer);
		// 	while(buffer[rpos]!='\n'){
		// 		if(buffer[rpos]=='\t'){
		// 		}
		// 		rpos++;
		// 	}
		}
	}
	fclose(in_fp);
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다.
*        패스 1로 부터 호출된다.
* 매계 : 소스코드의 라인번호
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다.
* -----------------------------------------------------------------------------------
*/

int token_parsing(int index)
{
	char *token;
	const char s[2] = "\t";
	int i;
//	printf("%s",input_data[index]);
	token = strtok(input_data[index],s);
	// while(token!=NULL){
		// printf("%s\n",token);
		// token = strtok(NULL,s);
	// }
	// printf("\n");
	token_table[token_line] = malloc(sizeof(token_table[token_line]));
	if(input_data[index][0]=='\t'){//no label
		token_table[token_line]->label = (char *)malloc(sizeof(char)*4);
		strcpy(token_table[token_line]->label,"\t");
//		printf("%s",token_table[token_line]->label);
	}
	else{
		token_table[token_line]->label = (char *)malloc(sizeof(char)*strlen(token)+1);
		strcpy(token_table[token_line]->label,token);
		strcpy(sym_table[token_line].symbol,token);
//		printf("%s%d\n",token_table[token_line]->label,strlen(token_table[token_line]->label));
		token = strtok(NULL,s);
	}
	token_table[token_line]->operator_ = (char *)malloc(sizeof(char)*strlen(token)+1);
	strcpy(token_table[token_line]->operator_,token);
//	printf("%s\t",token_table[token_line]->operator_);
	token = strtok(NULL,s);

	if(strcmp(token_table[token_line]->operator_,"RSUB")!=0){
		token_table[token_line]->operand[0] = (char *)malloc(sizeof(char)*strlen(token)+1);
		if(token[strlen(token)-1]=='\n'){
			token[strlen(token)-1]='\0';
		}
		strcpy(token_table[token_line]->operand[0],token);
//		printf("%s\t",token_table[token_line]->operand[0]);
		token = strtok(NULL,s);
	}
	if(token==NULL){
		token_table[token_line]->comment = NULL;
//		printf("\n");
	}
	else{
		token_table[token_line]->comment = (char *)malloc(sizeof(char)*strlen(token)+1);
		if(token[strlen(token)-1]=='\n'){
			token[strlen(token)-1]='\0';
		}
		strcpy(token_table[token_line]->comment,token);
//		printf("%s\n",token_table[token_line]->comment);
	}

	token_line++;
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다.
* 매계 : 토큰 단위로 구분된 문자열
* 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/

int search_opcode(char *str)
{
	int i;
	for(i=0;i<inst_index;++i){
		if(strcmp(str,inst[i].str)==0){
			return i;
		}
	}
	return -1;
}
/* -----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/

void make_objectcode(char *file_name)
{
	if(file_name==NULL){
		int i=0;
		for(i=0;i<token_line;++i){
			if(strcmp(token_table[i]->label,"\t")==0){//no label
				printf("%s",token_table[i]->label);
			}
			else{
				printf("%s\t",sym_table[i].symbol);
			}
			printf("%s\t",token_table[i]->operator_);

			if(strcmp(token_table[i]->operator_,"RSUB")!=0){
				printf("%s\t",token_table[i]->operand[0]);
			}
			else{
				printf("\t");
			}
			if(opcode[i]!=-1){
				printf("%02hhX",inst[opcode[i]].op);
			}
			printf("\n");
		}
	}
	else{
		FILE *of;
		of = fopen(file_name,"w");
		int i=0;
		for(i=0;i<token_line;++i){
			if(strcmp(token_table[i]->label,"\t")==0){//no label
				fprintf(of,"%s",token_table[i]->label);
			}
			else{
				fprintf(of,"%s\t",sym_table[i].symbol);
			}
			fprintf(of,"%s\t",token_table[i]->operator_);

			if(strcmp(token_table[i]->operator_,"RSUB")!=0){
				fprintf(of,"%s\t",token_table[i]->operand[0]);
			}
			else{
				fprintf(of,"\t");
			}
			if(opcode[i]!=-1){
				fprintf(of,"%02hhX",inst[opcode[i]].op);
			}
			fprintf(of,"\n");
		}
		fclose(of);
	}
}
