package sp2_2;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import sp.interfacepack.XEToyAssemler1;

public class Main implements XEToyAssemler1{

	private static final int MAX_INST =256;
	private static final int MAX_LINES =5000;
	
	private static final int MAX_COLUMNS= 4;
	private static final int MAX_OPERAND =3;
	
	private static final int SECTOR_MAX =10;
	
	private static final int XXX = 1;
	private static final int AAA = 0;
	private static final int SSS = 4;
	private static final int TTT = 5;
	/*
	* 기계어 목록 파일로 부터 정보를 받아와 생성하는 기계어 변환 테이블이다.
	* 해당 기계어의 정보를 토큰으로 나눠 기록하고 있다.
//	*/
class inst_struct{
	String str;
	int op;
	int format;
	int ops;
	public inst_struct(){
		this.str = null; 
		this.op=0;
		this.format=0;
		this.ops=0;
	}
}
	public int inst_index;
	inst_struct [] inst = new inst_struct[MAX_INST];
//	/*
//	* 어셈블리 할 소스코드를 토큰 단위로 관리하는 테이블이다.
//	* 관리 정보는 소스 라인 단위로 관리되어진다.
//	*/
	String[] input_data = new String[MAX_LINES];
	static int line_num;
	int label_num;
	class token{
		String label,operator_,comment;
		String [] operand = new String[MAX_OPERAND];
		int addr;
		int length;
		String opcode;
		public token(){
			this.label="";
			this.operator_="";
			this.comment="";
			this.addr=0;
			this.length=0;
			this.opcode="";
			this.operand[0]="";
			this.operand[2]="";
			this.operand[1]="";
		}
	}
	
	token[] token_table = new token[MAX_LINES];
	static int token_line;
	
	String[][] Modifi = new String [SECTOR_MAX][MAX_LINES];
	static int[] Modifi_num = new int[SECTOR_MAX];

	class Modification_record{
		String name;
		int flag,length,address;
		public Modification_record(){
			this.name = null;
			this.flag = 0;
			this.length =0;
			this.address =0;
		}
	}
	
	Modification_record[][] Modi = new Modification_record[SECTOR_MAX][MAX_LINES];
	static int[] modi_num = new int[SECTOR_MAX];
//	/*
//	* 심볼을 관리하는 구조체이다.
//	* 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
//	*/
	class symbol_unit{
		String symbol;
		int addr;
		public symbol_unit(){
			this.symbol = null;
			this.addr=0;
		}
	}
	symbol_unit [][] sym_table = new symbol_unit[SECTOR_MAX][MAX_LINES];
	static int[] symbol_line = new int [SECTOR_MAX];
	
	String[][] extdef = new String[SECTOR_MAX][MAX_LINES];
	static int [] extdef_num = new int[SECTOR_MAX];
	
	class literal_unit{
		String literal;
		int length,address;
		public literal_unit(){
			this.literal=null;
			this.length=0;
			this.address=0;
		}
	}
	
	literal_unit[] liter = new literal_unit[SECTOR_MAX];
	
	static int [] locctr = new int [SECTOR_MAX];
	
	static int sectionIndex;
	
	String[] sections = new String[SECTOR_MAX];
	static char input_file;
	static char output_file;
	public Main(){
		for(int i=0;i<MAX_INST;++i){
			inst[i] = new inst_struct();
		}
		for(int i=0;i<MAX_LINES;++i){
			token_table[i] = new token();
		}
		for(int i=0;i<SECTOR_MAX;++i){
			liter[i] = new literal_unit();
			for(int j=0;j<MAX_LINES;++j){
				Modi[i][j] = new Modification_record();
				sym_table[i][j] = new symbol_unit();
			}
		}
	}

	public void init_inst_file(String str){
		try {
			
			BufferedReader br = new BufferedReader(new FileReader("./src/sp2_2/inst.dat"));
			int i=0;
			while(true){
				String line = br.readLine();
				String [] arr;
				if(line==null)break;
				arr = line.split(" ");
				inst[i].str = arr[0];
				inst[i].format = Integer.parseInt(arr[1]);
				arr[2] = arr[2].substring(2, 4);
				inst[i].op = Integer.parseInt(arr[2],16);
				inst[i].ops = Integer.parseInt(arr[3]);
				i++;
			}

			inst_index=i;
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
//			e.printStackTrace();
			System.out.println("inst.dat file error");
		} catch (IOException e) {
			// TODO Auto-generated catch block
//			e.printStackTrace();
			System.out.println("IOException");
		}
	}
	public void init_input_file(String str){
		try {
			BufferedReader br = new BufferedReader(new FileReader("./src/sp2_2/input.txt"));
			while(true){
					String line = br.readLine();
					if(line==null)break;
					if(line.charAt(0)=='.')continue;
					input_data[line_num++] = line;
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
//			e.printStackTrace();
			System.out.println("input.txt file error");
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	@Override
	public void initialize() {
		// TODO Auto-generated method stub
		init_inst_file("inst.dat");
		init_input_file("input.txt");
	}
	public int token_parsing(int index){
		String [] arr;
//		System.out.println(input_data[index]);
		arr = input_data[index].split("\t");
		int i=0;
		if(input_data[index].charAt(0)=='.')return -1;
		if(input_data[index].charAt(0)=='\t')//no label
			token_table[token_line].label = "";
		else
			token_table[token_line].label = arr[0];
		token_table[token_line].operator_ = arr[1];
		if(arr.length<3)return 0;
		token_table[token_line].operand[0] = arr[2];
		return 0;
	}

	public int search_opcode(String str)
	{
		/* add your code here */
		int i=0;
		for(;i<inst_index;++i){
			if(str.charAt(0)=='+'&&str.compareTo("+"+inst[i].str)==0){
				return i;
			}
			else if(str.compareTo(inst[i].str)==0){
				return i;
			}
		}
		return -1;
	}

	public int search_label(String label_name,int section){
		int i=0;
		for(;i<symbol_line[section];++i){
			if(sym_table[section][i].symbol.compareTo(label_name)==0){
				return i;
			}
		}
		return -1;
	}
	public void pass1(){
		for(int i=0;i<line_num;++i){
			int result;
			if((result = token_parsing(i)) < 0)continue;
			if((token_table[token_line].operator_.compareTo("START")) == 0){
				locctr[sectionIndex] = Integer.parseInt(token_table[token_line].operand[0],16);
				sections[sectionIndex++] = token_table[token_line].label;
				token_table[token_line].addr= locctr[sectionIndex-1];
				token_table[token_line].length = 0;
			}
			else if(( token_table[token_line].operator_.compareTo("CSECT")) == 0){
				sections[sectionIndex++] = token_table[token_line].label;
				locctr[sectionIndex-1]=0;
				symbol_line[sectionIndex-1]=0;
				token_table[token_line].addr= locctr[sectionIndex-1];
				token_table[token_line].length = 0;
			}
			else if(( token_table[token_line].operator_.compareTo( "EXTDEF")) == 0){
				token_table[token_line].addr= 0;
				token_table[token_line].length = 0;
				String[] arr;
				arr = token_table[token_line].operand[0].split(",");
				for(int j=0;j<arr.length;++j){
					extdef[sectionIndex-1][extdef_num[sectionIndex-1]++] = arr[j];
				}
			}
			else if( (token_table[token_line].operator_.compareTo( "EXTREF")) == 0){
				token_table[token_line].addr= locctr[sectionIndex-1];
				token_table[token_line].length = 0;
				String[] arr;
				arr = token_table[token_line].operand[0].split(",");
				for(int j=0;j<arr.length;++j){
					Modifi[sectionIndex-1][Modifi_num[sectionIndex-1]++] = arr[j];
				}
			}
			else if( (token_table[token_line].operator_.compareTo( "LTORG" )) == 0 ){
				token_table[token_line].addr= locctr[sectionIndex-1];
				liter[sectionIndex-1].address = locctr[sectionIndex-1];
				if(liter[sectionIndex-1].literal.charAt(1)=='C'){
					locctr[sectionIndex-1]+=liter[sectionIndex-1].literal.length()-4;
				}else{
					locctr[sectionIndex-1]+=(liter[sectionIndex-1].literal.length()-4)*0.5;
				}
			}

			else if( (token_table[token_line].operator_.compareTo( "END" )) == 0){
				liter[sectionIndex-1].address = locctr[sectionIndex-1];
				token_table[token_line].addr= locctr[sectionIndex-1];
				locctr[sectionIndex-1]++;
			}
			else{
				int indexOfInst;
				token_table[token_line].addr = locctr[sectionIndex-1];
				if( token_table[token_line].label.compareTo("") != 0 ){
					int j=0,status=0;
					for(;j<symbol_line[sectionIndex-1];++j){
						if(sym_table[sectionIndex-1][j].symbol.compareTo(token_table[token_line].label)==0){
							status++;
						}
					}
					if(status==0){
						sym_table[sectionIndex-1][symbol_line[sectionIndex-1]].symbol=token_table[token_line].label;
						sym_table[sectionIndex-1][symbol_line[sectionIndex-1]++].addr = locctr[sectionIndex-1];
					}
				}
				// label 중복이면 에러, 아니면 심볼테이블에 추가
				if ( (indexOfInst = search_opcode(token_table[token_line].operator_)) > -1 ){
					if( token_table[token_line].operator_.charAt(0) == '+' ){
						locctr[sectionIndex-1]+=4;
					}
					else {
						locctr[sectionIndex-1]+=inst[indexOfInst].format;
					}
				}
				// opcode 를 인스트럭션 테이블에서 찾음
				//존재하면 일반 2형씩 또는 3형식에 따라 더함
				// + 가 있을 경우 4형식이므로 +4
				else if( token_table[token_line].operator_.compareTo( "WORD" ) == 0){
					locctr[sectionIndex-1]+=3;
				}
				// WORD 이면 +3
				else if( token_table[token_line].operator_.compareTo( "RESW" ) == 0){
					locctr[sectionIndex-1]+= Integer.parseInt(token_table[token_line].operand[0])*3;
				}
				// RESW 이면 + (3 * 갯수)
				else if(token_table[token_line].operator_.compareTo( "RESB" ) == 0){
					locctr[sectionIndex-1]+= Integer.parseInt(token_table[token_line].operand[0]);
				}
				// RESB 이면 + (1 * 갯수)
				else if( token_table[token_line].operator_.compareTo( "BYTE" ) == 0){
					if(token_table[token_line].operand[0].charAt(0)=='X'){
						locctr[sectionIndex-1] += (token_table[token_line].operand[0].length()-3)/2;
					}else{
						locctr[sectionIndex-1] += (token_table[token_line].operand[0].length()-4);
					}
				}
				// 캐릭터형이면 + (1 * 문자열갯수)
				// 16진수이면 + (0.5 * 숫자갯수)
				else if(token_table[token_line].operator_.compareTo("EQU")==0 ){;
					sym_table[sectionIndex-1][symbol_line[sectionIndex-1]].symbol=token_table[token_line].label;
					sym_table[sectionIndex-1][symbol_line[sectionIndex-1]++].addr = locctr[sectionIndex-1];
					if(token_table[token_line].operand[0].charAt(0)=='*'){
						token_table[token_line].addr = locctr[sectionIndex-1];
					}
					else{
						String [] ar = token_table[token_line].operand[0].split("-");
						int first = search_label(ar[0], sectionIndex-1);
						int second = search_label(ar[1], sectionIndex-1);
						token_table[token_line].addr = sym_table[sectionIndex-1][first].addr -sym_table[sectionIndex-1][second].addr; 
					}


				}
				else{
					return;
				}
				if(token_table[token_line].operand[0].length()>0){
					if(token_table[token_line].operand[0].charAt(0)=='='){
						liter[sectionIndex-1].literal=token_table[token_line].operand[0];
					}
				}
			}
//			System.out.println(Integer.toHexString(token_table[token_line].addr)+"\t"+token_table[token_line].label+"\t"+token_table[token_line].operator_+"\t"+token_table[token_line].operand[0]+"\t"+token_table[token_line].operand[1]);
			token_line++;
		}
	}
	public void pass2(){
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

			else if(strcmp(token_table[i].operator_,"TIXR")==0){
				sprintf(token_table[i].opcode, "%X", 0xB);
				sprintf(token_table[i].opcode+1, "%X", 0x8);
				sprintf(token_table[i].opcode+2, "%X", AXST(token_table[i].operand[0][0]));
				sprintf(token_table[i].opcode+3, "%X", 0);
			}
			else if((indexOfInst = search_opcode(token_table[i].operator_)) > -1){
				int symnum;
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


//					printf("%s\n%s\n%s\n%s\n%s\n",Modifi[0][0],Modifi[0][1],Modifi[1][0],Modifi[1][1],Modifi[2][1]);
				}else if(token_table[i].operand[0][0]=='@'){
					reg[0] = 1; reg[1] = 0; reg[2] = 0;
					reg[3] = 0; reg[4] = 1; reg[5] = 0;
					token_table[i].opcode = (char*)malloc(6);
					sprintf(token_table[i].opcode,"%X",inst[indexOfInst].op/16);
					sprintf(token_table[i].opcode+1,"%X",inst[indexOfInst].op%16+reg[0]*2+reg[1]);
					sprintf(token_table[i].opcode+2,"%X",reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
					if ((symnum = search_label(&token_table[i].operand[0][1],section)) > -1){
//						if ( >= 0) {
						sprintf(token_table[i].opcode+3,"%03X",(sym_table[section][symnum].addr - token_table[i + 1].addr)%0x1000);
//						}
					}
				}else {
					reg[0] = 1; reg[1] = 1; reg[2] = 0;
					reg[3] = 0; reg[4] = 1; reg[5] = 0;

					token_table[i].opcode = (char*)malloc(6);
					sprintf(token_table[i].opcode,"%X",inst[indexOfInst].op/16);
					sprintf(token_table[i].opcode+1,"%X",inst[indexOfInst].op%16+reg[0]*2+reg[1]);
					sprintf(token_table[i].opcode+2,"%X",reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
					if(token_table[i].operand[0][0]=='='){

						sprintf(token_table[i].opcode+3,"%03X",(liter[section].address-token_table[i+1].addr));
					}
					else{
						if((symnum = search_label(token_table[i].operand[0],section)) > -1){
							int res =sym_table[section][symnum].addr - token_table[i + 1].addr;
							if(res>=0){
								sprintf(token_table[i].opcode+3,"%03X",res%0x1000);
							}
							else{
								res = res&0xFFF;
								sprintf(token_table[i].opcode+3,"%03X",res%0x1000);
							}
						}
					}


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
				printf("\n%04X\t*\t%s\t",token_table[i].addr,liter[section].literal);
				token_table[i].opcode = (char*)malloc((strlen(liter[section].literal)-4));
				int j=0;
				for(;j<(strlen(liter[section].literal)-4);++j){
					sprintf(token_table[i].opcode+j, "%c", liter[section].literal[3+j]);
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

				token = strtok(NULL,s);
				Modi[section][modi_num[section]].name = (char *)malloc(sizeof(token));
				strcpy(Modi[section][modi_num[section]].name,token);
				Modi[section][modi_num[section]].address = token_table[i].addr;
				Modi[section][modi_num[section]].flag = 0;
				Modi[section][modi_num[section]++].length = 6;
				token_table[i].opcode = (char *)malloc(6);
				sprintf(token_table[i].opcode,"000000");//because both value is 0000;
//				printf("%d%s%d%x\n%d%s%d%x",Modi[section][0].flag,Modi[section][0].name,Modi[section][0].length,Modi[section][0].address,Modi[section][1].flag,Modi[section][1].name,Modi[section][1].length,Modi[section][1].address);
			}

			else if(strcmp(token_table[i].operator_,"LTORG")==0){
				printf("\n%04X\t*\t\t%s",token_table[i].addr,liter[section].literal);
				token_table[i].opcode = (char*)malloc((strlen(liter[section].literal)-4)*2);
				int j=0;
				for(;j<(strlen(liter[section].literal)-4);++j){
					sprintf(token_table[i].opcode+j*2, "%X", liter[section].literal[3+j]/16);
					sprintf(token_table[i].opcode+1+j*2, "%X", liter[section].literal[3+j]%16);
				}
			}

			printf("\t%s\n",token_table[i].opcode);
		}
		return;
	}
	@Override
	public void parseData(File arg) {
		// TODO Auto-generated method stub
		pass1();
		pass2();
	}
	@Override
	public void printOPCODE() {
		// TODO Auto-generated method stub
		
	}
	public void startt(){
		File arg = null;
		initialize();
		parseData(arg);
		printOPCODE();
	}
	public static void main(String args[]){
		new Main().startt();
	}
}