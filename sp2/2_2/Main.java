package sp2_2;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

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
	
	public int AXST(char s){
		if (s=='X')return 1;
		else if(s=='A')return 0;
		else if(s=='S')return 4;
		else return 5;
	}
	public void pass2(){
		int i=0,indexOfInst,section=0;
		for(;i<token_line;++i){
			System.out.print(String.format("%04X\t%s\t%s",token_table[i].addr,token_table[i].label,token_table[i].operator_));
			if(token_table[i].operator_.compareTo("RSUB")==0){
				System.out.print("\t");
			}
			else if(token_table[i].operand[0].length()>0){
				System.out.print(String.format("\t%s",token_table[i].operand[0]));
			}
			if(token_table[i].operand[1].length()>0){
				System.out.print(String.format(",%s",token_table[i].operand[1]));
			}
			if( token_table[i].operator_.compareTo("CSECT") == 0){
				section++;
			}
			int[] reg = new int[6];
			if(token_table[i].operator_.compareTo("CLEAR")==0){
				token_table[i].opcode="B4"+AXST(token_table[i].operand[0].charAt(0))+"0";
			}
			else if(token_table[i].operator_.compareTo("COMPR")==0){
				token_table[i].opcode = "A0"+AXST(token_table[i].operand[0].charAt(0))+""+AXST(token_table[i].operand[0].charAt(2));
			}
			else if(token_table[i].operator_.compareTo("TIXR")==0){
				token_table[i].opcode="B8"+AXST(token_table[i].operand[0].charAt(0))+"0";
			}
			else if((indexOfInst = search_opcode(token_table[i].operator_)) > -1){
				int symnum;
				if(token_table[i].operator_.compareTo("RSUB")==0){
					reg[0] = 1; reg[1] = 1; reg[2] = 0;
					reg[3] = 0; reg[4] = 0; reg[5] = 0;
					token_table[i].opcode+= String.format("4%X%X000",0xC+reg[0]*2+reg[1],reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
				}
				else if(token_table[i].operand[0].charAt(0)=='#'){//nixbpe
					reg[0] = 0; reg[1] = 1; reg[2] = 0;
					reg[3] = 0; reg[4] = 0; reg[5] = 0;
					String opr = token_table[i].operand[0].substring(1, token_table[i].operand[0].length());
					token_table[i].opcode=String.format("%X%X%X%03X", inst[indexOfInst].op/16,inst[indexOfInst].op%16+reg[0]*2+reg[1],reg[2]*8+reg[3]*4+reg[4]*2+reg[5],Integer.parseInt(opr));
				}else if(token_table[i].operator_.charAt(0)=='+'){
					reg[0] = 1; reg[1] = 1; reg[2] = 0;
					reg[3] = 0; reg[4] = 0; reg[5] = 1;
					if(token_table[i].operand[0].charAt(token_table[i].operand[0].length()-1)=='X')reg[2]=1;
					token_table[i].opcode = String.format("%X%X%X00000", inst[indexOfInst].op/16,inst[indexOfInst].op%16+reg[0]*2+reg[1],reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
					String [] starr = token_table[i].operand[0].split(",");
					Modi[section][modi_num[section]].name = starr[0];
					Modi[section][modi_num[section]].address = token_table[i].addr+1;
					Modi[section][modi_num[section]].flag = 1;
					Modi[section][modi_num[section]++].length = 5;
				}else if(token_table[i].operand[0].charAt(0)=='@'){
					reg[0] = 1; reg[1] = 0; reg[2] = 0;
					reg[3] = 0; reg[4] = 1; reg[5] = 0;
					String opr = token_table[i].operand[0].substring(1, token_table[i].operand[0].length());
					symnum = search_label(opr,section);
					token_table[i].opcode = String.format("%X%X%X%03X",inst[indexOfInst].op/16,inst[indexOfInst].op%16+reg[0]*2+reg[1],reg[2]*8+reg[3]*4+reg[4]*2+reg[5],(sym_table[section][symnum].addr - token_table[i + 1].addr)%4096);
				}else {
					reg[0] = 1; reg[1] = 1; reg[2] = 0;
					reg[3] = 0; reg[4] = 1; reg[5] = 0;
					token_table[i].opcode = String.format("%X%X%X",inst[indexOfInst].op/16,inst[indexOfInst].op%16+reg[0]*2+reg[1],reg[2]*8+reg[3]*4+reg[4]*2+reg[5]);
					if(token_table[i].operand[0].charAt(0)=='='){
						token_table[i].opcode+= String.format("%03X", liter[section].address-token_table[i+1].addr);
					}
					else{
						if((symnum = search_label(token_table[i].operand[0],section)) > -1){
							int res =sym_table[section][symnum].addr - token_table[i + 1].addr;
							if(res>=0){
								token_table[i].opcode+= String.format("%03X",res%0x1000);
							}
							else{
								res = res&0xFFF;
								token_table[i].opcode+= String.format("%03X",res%0x1000);
							}
						}
					}
				}
			}
			else if(token_table[i].operator_.compareTo("END")==0){
				System.out.print(String.format("\n%04X\t*\t%s\t",token_table[i].addr,liter[section].literal));
				int j=0;
				for(;j<(liter[section].literal.length()-4);++j){
					token_table[i].opcode+=liter[section].literal.charAt(3+j);
				}
			}
			else if(token_table[i].operator_.compareTo("BYTE")==0){
				int j=0;
				for(;j<(token_table[i].operand[0].length()-3);++j){
					token_table[i].opcode+=token_table[i].operand[0].charAt(2+j);
				}
			}
			else if(token_table[i].operator_.compareTo("WORD")==0){
				String buffer = token_table[i].operand[0];
				String[] buf = buffer.split("-");
				Modi[section][modi_num[section]].name=buf[0];
				Modi[section][modi_num[section]].address = token_table[i].addr;
				Modi[section][modi_num[section]].flag = 1;
				Modi[section][modi_num[section]++].length = 6;
				Modi[section][modi_num[section]].name=buf[1];
				Modi[section][modi_num[section]].address = token_table[i].addr;
				Modi[section][modi_num[section]].flag = 0;
				Modi[section][modi_num[section]++].length = 6;
				token_table[i].opcode = "000000";
			}
			else if(token_table[i].operator_.compareTo("LTORG")==0){
				System.out.print(String.format("\n%04X\t*\t\t%s",token_table[i].addr,liter[section].literal));
				int j=0;
				token_table[i].opcode="";
				for(;j<(liter[section].literal.length()-4);++j){
					token_table[i].opcode+= String.format("%X%X", liter[section].literal.charAt(3+j)/16,liter[section].literal.charAt(3+j)%16);
				}
			}
			System.out.print(String.format("\t%s\n",token_table[i].opcode));
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
	public void printOPCODE(){
		// TODO Auto-generated method stub
		PrintWriter pw;
		try {
			pw = new PrintWriter("./src/sp2_2/output.txt");
		int i=1,section=0;
		int ls=0,rs=0;
		pw.println(String.format("H%-6s000000%06X",sections[section],locctr[section]));
		for(;i<token_line;++i){
			if(token_table[i].operator_.compareTo("CSECT")==0){
				int j=0;
				if(section!=0){
					pw.print(String.format("%02X",rs-3));
					pw.print(ls+""+i);
					for(;ls<i;ls++){
						pw.print(token_table[ls].opcode);
					}
					pw.println();
					rs=0;
				}
				for(;j<modi_num[section];++j){
					pw.print(String.format("M%06X%02X",Modi[section][j].address,Modi[section][j].length));
					if(Modi[section][j].flag==1) 
						pw.print("+");
					else	
						pw.print("-");
					pw.println(Modi[section][j].name);
				}
				if(section==0){
					pw.println("E000000");
					pw.println();
				}
				else{
					pw.println("E");
					pw.println();
				}
				section++;
				pw.println(String.format("H%-6s000000%06X",sections[section],locctr[section]));
			}
			else if(token_table[i].operator_.compareTo("END")==0){
				pw.print(String.format("T000000%2X",rs+1));
				for(;ls<token_line;ls++){
					if(token_table[i].opcode.compareTo("")==0)continue;
					pw.print(token_table[ls].opcode);
				}
				pw.println();
				rs=0;
				int j=0;
				for(;j<modi_num[section];++j){
					pw.print(String.format("M%06X%02X",Modi[section][j].address,Modi[section][j].length));
					if(Modi[section][j].flag==1)
						pw.print("+");
					else
						pw.print("-");
					pw.println(Modi[section][j].name);
				}
				pw.print("E");
			}
			else if(token_table[i].operator_.compareTo("EXTDEF")==0){
				int j=0;
				pw.print("D");
				for(;j<extdef_num[section];++j){
					pw.print(String.format("%-6s%06X",extdef[section][j],sym_table[section][search_label(extdef[section][j],section)].addr));
				}
				pw.println();
			}
			else if(token_table[i].operator_.compareTo("EXTREF")==0){
				int j=0;
				pw.print("R");
				for(;j<Modifi_num[section];++j){
					pw.print(String.format("%-6s",Modifi[section][j]));
				}
				pw.println();
			}
			else if(token_table[i].operator_.compareTo("LTORG")==0){
				pw.print(String.format("%02X",rs-3));
				for(;ls<i;ls++){
					if(token_table[i].opcode.compareTo("")==0)continue;
					pw.print(token_table[ls].opcode);
				}
				ls++;
				pw.println();
				rs=0;
				pw.println(String.format("T%06X%02X%6s",token_table[i].addr,(token_table[i].opcode.length())/2,token_table[i].opcode));
			}
			else if(token_table[i].opcode.compareTo("")==0)continue;
			else{
				if(rs+(token_table[i].opcode.length())/2>29){
					pw.print(String.format("T000000%2X",rs));
					for(;ls<i;ls++){
						if(token_table[i].opcode.compareTo("")==0)continue;
						pw.print(token_table[ls].opcode);
					}
					pw.println();
					pw.print(String.format("T%06X",rs));
					rs = (token_table[i].opcode.length())/2;
				}
				rs+=(token_table[i].opcode.length())/2;
			}
		}
		pw.close();

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
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