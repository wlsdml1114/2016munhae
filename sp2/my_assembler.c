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
//	printf("%X\t%X\t%X",locctr[0],locctr[1],locctr[2]);
//	int i,j;
//	for(i=0;i<sectionIndex;++i){
//		for(j=0;j<symbol_line[i];++j){
//			printf("%04hhX\t%s\n",sym_table[i][j].addr,sym_table[i][j].symbol);
//		}
//		printf("\n");
//	}

	//make_output("output") ;
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
	int i=0;
	for(;i<line_num;++i){
		int result;
		if ((result = token_parsing(i)) < 0){//output is less then 0 means this sentence start with .
			continue;
		}
//		printf("%d\n",search_opcode(token_table[token_line].operator_));
		if( strcmp(token_table[token_line].operator_, "START") == 0){
			locctr[sectionIndex] = (int)strtol(token_table[token_line].operand[0],NULL,16);
			sections[sectionIndex] = (char *)malloc(strlen(token_table[token_line].label));
			strcpy(sections[sectionIndex++], token_table[token_line].label);
//			printf("%04hhX  %s\n",locctr[sectionIndex-1],sections[sectionIndex-1]);
			token_table[token_line].addr= locctr[sectionIndex-1];
			token_table[token_line].length = 0;
		}
		else if( strcmp(token_table[token_line].operator_, "CSECT") == 0){
			sections[sectionIndex] = (char *)malloc(strlen(token_table[token_line].label));
			strcpy(sections[sectionIndex++], token_table[token_line].label);
			locctr[sectionIndex-1]=0;
			literal_number[sectionIndex-1]=0;
			symbol_line[sectionIndex-1]=0;
//			printf("%04hhX  %s\n",locctr[sectionIndex-1],sections[sectionIndex-1]);
			token_table[token_line].addr= locctr[sectionIndex-1];
			token_table[token_line].length = 0;
		}
		else if( strcmp(token_table[token_line].operator_, "EXTDEF") == 0){
			token_table[token_line].addr= 0;
			token_table[token_line].length = 0;
		}
		else if( strcmp(token_table[token_line].operator_, "EXTREF") == 0){
			token_table[token_line].addr= locctr[sectionIndex-1];
			token_table[token_line].length = 0;
			char *token;
			const char s[2] = ",";
			char buffer[100];
			strcpy(buffer,token_table[token_line].operand[0]);
			token = strtok(buffer,s);
			int i=0;
			do{
//				sym_table[sectionIndex-1][symbol_line].symbol = (char *)malloc(sizeof(strlen(token)));
				token_table[token_line].operand[i] = malloc(sizeof(token));
				strcpy(token_table[token_line].operand[i++],token);
				strcpy(Modifi[sectionIndex-1][Modifi_num[sectionIndex-1]++],token);
//				strcpy(sym_table[sectionIndex-1][symbol_line[sectionIndex-1]].symbol,token);
//				sym_table[sectionIndex-1][symbol_line[sectionIndex-1]++].addr = 0;
				token = strtok(NULL,s);
			}
			while(token!=NULL);
		}
		else if( strcmp(token_table[token_line].operator_, "LTORG" ) == 0 ){
			token_table[token_line].addr= locctr[sectionIndex-1];
			if(liter[sectionIndex-1][literal_number[sectionIndex-1]-1].literal[1]=='C'){
				locctr[sectionIndex-1]+=strlen(liter[sectionIndex-1][literal_number[sectionIndex-1]-1].literal)-4;
			}else{
				locctr[sectionIndex-1]+=(strlen(liter[sectionIndex-1][literal_number[sectionIndex-1]-1].literal)-4)*0.5;
			}
		}

		else if( strcmp(token_table[token_line].operator_, "END" ) == 0){
			token_table[token_line].addr= locctr[sectionIndex-1];
			locctr[sectionIndex-1]++;
		}

		else{
			int indexOfInst;
			token_table[token_line].addr = locctr[sectionIndex-1];
			if( strcmp(token_table[token_line].label, "") != 0 ){
				int j=0,status=0;
				for(;j<symbol_line[sectionIndex-1];++j){
					if(strcmp(sym_table[sectionIndex-1][j].symbol,token_table[token_line].label)==0){
						status++;
					}
				}
				if(status==0){
//					sym_table[sectionIndex-1][symbol_line].symbol = (char *)malloc(sizeof(strlen(token_table[token_line].label)));
					strcpy(sym_table[sectionIndex-1][symbol_line[sectionIndex-1]].symbol,token_table[token_line].label);
					sym_table[sectionIndex-1][symbol_line[sectionIndex-1]++].addr = locctr[sectionIndex-1];
				}
			}
			// label 중복이면 에러, 아니면 심볼테이블에 추가
			if ( (indexOfInst = search_opcode(token_table[token_line].operator_)) > -1 ){
				if( token_table[token_line].operator_[0] == '+' ){
					locctr[sectionIndex-1]+=4;
				}
				else {
					locctr[sectionIndex-1]+=inst[indexOfInst].format;
				}
			}
			// opcode 를 인스트럭션 테이블에서 찾음
			//존재하면 일반 2형씩 또는 3형식에 따라 더함
			// + 가 있을 경우 4형식이므로 +4
			else if( strcmp(token_table[token_line].operator_, "WORD" ) == 0){
				locctr[sectionIndex-1]+=3;
			}
			// WORD 이면 +3
			else if( strcmp(token_table[token_line].operator_, "RESW" ) == 0){
				locctr[sectionIndex-1]+= atoi(token_table[token_line].operand[0])*3;
			}
			// RESW 이면 + (3 * 갯수)
			else if( strcmp(token_table[token_line].operator_, "RESB" ) == 0){

				locctr[sectionIndex-1]+= atoi(token_table[token_line].operand[0]);
			}
			// RESB 이면 + (1 * 갯수)
			else if( strcmp(token_table[token_line].operator_, "BYTE" ) == 0){
				if(token_table[token_line].operand[0][0]=='X'){
					locctr[sectionIndex-1] += (strlen(token_table[token_line].operand[0])-4)*0.5;
					locctr[sectionIndex-1]++;
				}else{
					locctr[sectionIndex-1] += (strlen(token_table[token_line].operand[0])-4);
				}
			}
			// 캐릭터형이면 + (1 * 문자열갯수)
			// 16진수이면 + (0.5 * 숫자갯수)
			else if(strcmp(token_table[token_line].operator_, "EQU")==0 ){
				token_table[token_line].addr = locctr[sectionIndex-1];
				if(strcmp(token_table[token_line].label, "MAXLEN")==0){

					token_table[token_line].addr = 4096;
				}


			}
			else{
				return -1;
			}
			if(token_table[token_line].operand[0][0]=='='){
				liter[sectionIndex-1][literal_number[sectionIndex-1]].literal = (char *)malloc(sizeof(strlen(token_table[token_line].operand[0])));
				strcpy(liter[sectionIndex-1][literal_number[sectionIndex-1]++].literal,token_table[token_line].operand[0]);
			}
		}
//		printf("%04X\t%s\t%s\t%s\t%s\n",token_table[token_line].addr,token_table[token_line].label,token_table[token_line].operator_,token_table[token_line].operand[0],token_table[token_line].operand[1]);
		token_line++;
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
int AXST(char s){
	if (s=='X')return 1;
	else if(s=='A')return 0;
	else if(s=='S')return 4;
	else return 5;
}
static int assem_pass2(void)
{
	int i=0,indexOfInst,section=0;
	for(;i<token_line;++i){
		printf("%04X\t%s\t%s",token_table[i].addr,token_table[i].label,token_table[i].operator_);
		if(strcmp(token_table[i].operator_,"RSUB")==0){
			printf("\t");
		}
		else if(token_table[i].operand[0]!=NULL){
			printf("\t%s",token_table[i].operand[0]);
		}
		if(token_table[i].operand[1]!=NULL){
			printf(",%s",token_table[i].operand[1]);
		}
		if( strcmp(token_table[i].operator_, "CSECT") == 0){
			section++;
		}
		token_table[i].opcode= (char *)malloc(token_table[i].length*2);
		char reg[6];
		if(strcmp(token_table[i].operator_,"CLEAR")==0){
			sprintf(token_table[i].opcode, "%X", 0xB);
			sprintf(token_table[i].opcode+1, "%X", 0x4);
			sprintf(token_table[i].opcode+2, "%X", AXST(token_table[i].operand[0][0]));
			sprintf(token_table[i].opcode+3, "%X", 0);
		}

		else if(strcmp(token_table[i].operator_,"COMPR")==0){
			sprintf(token_table[i].opcode, "%X", 0xA);
			sprintf(token_table[i].opcode+1, "%X", 0x0);
			sprintf(token_table[i].opcode+2, "%X", AXST(token_table[i].operand[0][0]));
			sprintf(token_table[i].opcode+3, "%X", AXST(token_table[i].operand[0][2]));
		}

		if(strcmp(token_table[i].operator_,"TIXR")==0){
			sprintf(token_table[i].opcode, "%X", 0xB);
			sprintf(token_table[i].opcode+1, "%X", 0x8);
			sprintf(token_table[i].opcode+2, "%X", AXST(token_table[i].operand[0][0]));
			sprintf(token_table[i].opcode+3, "%X", 0);
		}
		else if((indexOfInst = search_opcode(token_table[i].operator_)) > -1){
			if(token_table[i].operand[0][0]=='#'){//nixbpe
				reg[0] = 0; reg[1] = 1; reg[2] = 0;
				reg[3] = 0; reg[4] = 0; reg[5] = 0;


				token_table[i].opcode = (char*)malloc(6);
				sprintf(token_table[i].opcode,"%X",inst[indexOfInst].op/16);
				sprintf(token_table[i].opcode+1,"%X",inst[indexOfInst].op%16+reg[0]*2+reg[1]);
				sprintf(token_table[i].opcode+2,"%X",reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
				sprintf(token_table[i].opcode+3,"%03X",atoi(&token_table[i].operand[0][1]));

			}else if(token_table[i].operator_[0]=='+'){
				reg[0] = 1; reg[1] = 1; reg[2] = 0;
				reg[3] = 0; reg[4] = 0; reg[5] = 1;
				if(token_table[i].operand[0][strlen(token_table[i].operand[0])-1]=='X'){
					reg[2]=1;
				}
				token_table[i].opcode = (char*)malloc(8);
				sprintf(token_table[i].opcode,"%X",inst[indexOfInst].op/16);
				sprintf(token_table[i].opcode+1,"%X",inst[indexOfInst].op%16+reg[0]*2+reg[1]);
				sprintf(token_table[i].opcode+2,"%X",reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
				sprintf(token_table[i].opcode+3,"00000");

				char *token;
				const char s[2] = ",";
				token = strtok(token_table[i].operand[0],s);
				Modi[section][modi_num[section]].name = (char *)malloc(sizeof(token));
				strcpy(Modi[section][modi_num[section]].name,token);
				Modi[section][modi_num[section]].address = token_table[i].addr+1;
				Modi[section][modi_num[section]].flag = 1;
				Modi[section][modi_num[section]++].length = 5;


//				printf("%s\n%s\n%s\n%s\n%s\n",Modifi[0][0],Modifi[0][1],Modifi[1][0],Modifi[1][1],Modifi[2][1]);
			}else if(token_table[i].operand[0][0]=='@'){
				reg[0] = 1; reg[1] = 0; reg[2] = 0;
				reg[3] = 0; reg[4] = 1; reg[5] = 0;
			}else {
				reg[0] = 1; reg[1] = 1; reg[2] = 0;
				reg[3] = 0; reg[4] = 1; reg[5] = 0;
			}

			if(strcmp(token_table[i].operator_,"RSUB")==0){
				reg[0] = 1; reg[1] = 1; reg[2] = 0;
				reg[3] = 0; reg[4] = 0; reg[5] = 0;
				token_table[i].opcode = (char*)malloc(6);
				sprintf(token_table[i].opcode,"%X",0x4);
				sprintf(token_table[i].opcode+1,"%X",0xC+reg[0]*2+reg[1]);
				sprintf(token_table[i].opcode+2,"%X",reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
				sprintf(token_table[i].opcode+3,"000");
			}


		}


		else if(strcmp(token_table[i].operator_,"END")==0){
			printf("\n%04X\t*\t%s",token_table[i].addr,liter[section][0].literal);
			token_table[i].opcode = (char*)malloc((strlen(liter[section][0].literal)-4));
			int j=0;
			for(;j<(strlen(liter[section][0].literal)-4);++j){
				sprintf(token_table[i].opcode+j, "%c", liter[section][0].literal[3+j]);
			}
		}

		else if(strcmp(token_table[i].operator_,"BYTE")==0){
			int j=0;
			for(;j<(strlen(token_table[i].operand[0])-3);++j){
				sprintf(token_table[i].opcode+j, "%c", token_table[i].operand[0][2+j]);
			}
		}
		else if(strcmp(token_table[i].operator_,"WORD")==0){
			char *token;
			char buffer[1024];
			strcpy(buffer,token_table[i].operand[0]);
			const char s[2] = "-";
			token = strtok(buffer,s);
			Modi[section][modi_num[section]].name = (char *)malloc(sizeof(token));
			strcpy(Modi[section][modi_num[section]].name,token);
			Modi[section][modi_num[section]].address = token_table[i].addr;
			Modi[section][modi_num[section]].flag = 1;
			Modi[section][modi_num[section]++].length = 6;

			token = strtok(buffer,s);
			Modi[section][modi_num[section]].name = (char *)malloc(sizeof(token));
			strcpy(Modi[section][modi_num[section]].name,token);
			Modi[section][modi_num[section]].address = token_table[i].addr;
			Modi[section][modi_num[section]].flag = 0;
			Modi[section][modi_num[section]++].length = 6;
			token_table[i].opcode = (char *)malloc(6);
			sprintf(token_table[i].opcode,"000000");//because both value is 0000;
//			printf("%d%s%d%x\n%d%s%d%x",Modi[section][0].flag,Modi[section][0].name,Modi[section][0].length,Modi[section][0].address,Modi[section][1].flag,Modi[section][1].name,Modi[section][1].length,Modi[section][1].address);
		}

		else if(strcmp(token_table[i].operator_,"LTORG")==0){
			printf("\n%04X\t*\t\t%s",token_table[i].addr,liter[section][0].literal);
			token_table[i].opcode = (char*)malloc((strlen(liter[section][0].literal)-4)*2);
			int j=0;
			for(;j<(strlen(liter[section][0].literal)-4);++j){
				sprintf(token_table[i].opcode+j*2, "%X", liter[section][0].literal[3+j]/16);
				sprintf(token_table[i].opcode+1+j*2, "%X", liter[section][0].literal[3+j]%16);
			}
		}

		printf("\t%s\n",token_table[i].opcode);
	}
	make_objectcode("output.txt");
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
//			printf("%s %hhx %d %d\n", inst[i].str,inst[i].op,inst[i].format,inst[i].ops);
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
	if(input_data[index][0]=='.')return -1;
	token = strtok(input_data[index],s);
	// while(token!=NULL){
		// printf("%s\n",token);
		// token = strtok(NULL,s);
	// }
	// printf("\n");
	if(input_data[index][0]=='\t'){//no label
		token_table[token_line].label = (char *)malloc(sizeof(char)*4);
		strcpy(token_table[token_line].label,"");
//			printf("%s",token_table[token_line].label);
	}
	else{
		token_table[token_line].label = (char *)malloc(sizeof(char)*strlen(token)+1);
		strcpy(token_table[token_line].label,token);
//			printf("%s",token_table[token_line].label);
		token = strtok(NULL,s);
	}



	token_table[token_line].operator_ = (char *)malloc(sizeof(char)*strlen(token)+1);
	if(token[strlen(token)-1]=='\n'){
		token[strlen(token)-1] = '\0';
	}
	strcpy(token_table[token_line].operator_,token);
//		printf(".%s\t.",token_table[token_line].operator_);
	token = strtok(NULL,s);

	if(token==NULL){
		return 0;
	}

	token_table[token_line].operand[0] = (char *)malloc(sizeof(char)*strlen(token)+1);
	if(token[strlen(token)-1]=='\n'){
		token[strlen(token)-1]='\0';
	}
	strcpy(token_table[token_line].operand[0],token);
//	printf("%s",token_table[token_line].operand[0]);
//
//	if(strcmp(token_table[token_line].operator_,"RSUB")!=0
//			||strcmp(token_table[token_line].operator_,"LTORG")!=0){
//		token = strtok(NULL,s);
//	}
//	if(token==NULL){
//		token_table[token_line].comment = NULL;
//			printf("\n");
//	}
//	else{
//		token_table[token_line].comment = (char *)malloc(sizeof(char)*strlen(token)+1);
//		if(token[strlen(token)-1]=='\n'){
//			token[strlen(token)-1]='\0';
//		}
//		strcpy(token_table[token_line].comment,token);
//			printf("%s\n",token_table[token_line].comment);
//	}
//
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
	/* add your code here */
	int i=0;
	for(;i<inst_index;++i){
		if(str[0]=='+'&&strcmp(&str[1],inst[i].str)==0){
			return i;
		}
		else if(strcmp(str,inst[i].str)==0){
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
	/* add your code here */

}

