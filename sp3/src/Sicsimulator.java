
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;

import sp.project2.interfacepack.SicSimulator;

public class Sicsimulator implements SicSimulator{
    boolean finish = false;
    HashMap<Integer,opcode> table;
    String log;
    private static Sicsimulator instance = new Sicsimulator();
    public Sicsimulator() {
        log = "";
        table = new HashMap<>();
    }
    public static Sicsimulator getInstance(){
        return instance;
    }
    //메모리 작업 등 실질적인 초기화 작업을 수행
    //각 명령어가 저장되어있는 inst.data파일을 읽고 저장
    @Override
    public void initialize(File instFile){
        try {
            BufferedReader br = new BufferedReader(new FileReader("./inst.dat"));
            while(true){
                String line = br.readLine();
                if(line==null)break;
                String[] arr = line.split(" ");
                table.put(Integer.parseInt(arr[2].substring(2), 16), new opcode(arr[0], arr[1].charAt(0) - '0', Integer.parseInt(arr[2].substring(2), 16)));
//                System.out.println(arr[0]+" "+(arr[1].charAt(0)-'0')+" "+Integer.parseInt(arr[2].substring(2),16));
            }
            br.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    //하나의 명령어만 수행한다. 해당 명령어가 수행되고 난 값의 변화를
    //보여주고, 다음 명령어를 포인팅
    @Override
    public void oneStep(){
        int pc = ResourceManager.getInstance().getRegister(8)*2;//pc is 8
        if(finish){
            pc=0;
            return;
        }
        byte[] bytes = ResourceManager.getInstance().getMemory(pc, 3);
        System.out.println(new String (bytes));
        int op = Integer.parseInt(new String(bytes), 16);
        int opcheck = op/16/4*4;
        if(table.containsKey(opcheck)){
            boolean use_pc = false;
            int format = table.get(opcheck).getFormat();
            if (format==2) {
                ResourceManager.getInstance().setTarget_addr((pc+format*2)/2);
                switch (table.get(opcheck).getName()) {
                    case "CLEAR":
                        bytes = ResourceManager.getInstance().getMemory(pc+2,1);
                        ResourceManager.getInstance().setRegister(bytes[0]-'0',0);
                        log = "해당하는 Register를 0으로 초기화";
                        break;
                    case "COMPR":
                        log = "register 2개를 비교해서 condition 을 바꾼다.";
                        bytes = ResourceManager.getInstance().getMemory(pc+2,2);
                        ResourceManager.getInstance().setRegister(9,ResourceManager.getInstance().getRegister(bytes[0]-'0')-ResourceManager.getInstance().getRegister(bytes[1]-'0'));
                        break;
                    case "TIXR":
                        log = "X를 1증가시키고 register를 비교해서 condition을 바꾼다.";
                        bytes = ResourceManager.getInstance().getMemory(pc+2,1);
                        ResourceManager.getInstance().setRegister(1, ResourceManager.getInstance().getRegister(1) + 1);
                        ResourceManager.getInstance().setRegister(9,ResourceManager.getInstance().getRegister(1)-ResourceManager.getInstance().getRegister(bytes[0]-'0'));
                        break;
                    default:
                        break;
                }
            }
            else {
                boolean index=false;
                boolean imid = false;
                boolean indirect = false;
                int nixbpe = Integer.parseInt(new String(bytes),16);
                if(nixbpe%2==1) format++;
                if(nixbpe/8%2==1) index=true;
                if(nixbpe/16%4==1) imid=true;
                if(nixbpe/16%4==2) indirect = true;
                ResourceManager.getInstance().setTarget_addr((pc+format*2)/2);
                int data=0;
                byte[] b;
                switch (table.get(opcheck).getName()){
                    case "COMP" :
                        log = "A register와 비교해서 condition을 바꾼다.";
                        if(imid) data = Integer.parseInt(new String(ResourceManager.getInstance().getMemory(pc+3,3)),16);
                        ResourceManager.getInstance().setRegister(9,ResourceManager.getInstance().getRegister(0)-data);
                        break;
                    case "J" :
                        log = "PC를 바꾼다.";
                        use_pc=true;
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16);
                        if(data/16/16/8==1){
                            data--;
                            data = data^0xFFF;
                            data = -data;
                        }
//                        System.out.println(new String(b) + " "+data+" "+ResourceManager.getInstance().getTarget_addr());

                        if(!indirect) {
                            data += ResourceManager.getInstance().getTarget_addr();
                        }
                        pc = data;
                        pc*=2;
                        System.out.println("pc :"+ pc);
                        if(pc==0) finish=true;
                        break;
                    case "JEQ" :
                        log ="Condition이 0일 경우 점프 한다.";
                        if(ResourceManager.getInstance().getRegister(9)!=0) break;
                        use_pc=true;
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16);
                        if(data/16/16/8==1){
                            data--;
                            data = data^0xFFF;
                            data = -data;
                        }
                        data += ResourceManager.getInstance().getTarget_addr();
                        pc = data;
                        pc*=2;
                        break;
                    case "JLT" :
                        log ="Condition이 0보다 작을 경우 점프 한다.";
                        if(ResourceManager.getInstance().getRegister(9)>=0) break;
                        use_pc=true;
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16);
                        if(data/16/16/8==1){
                            data--;
                            data = data^0xFFF;
                            data = -data;
                        }
                        data +=  ResourceManager.getInstance().getTarget_addr();
                        pc = data;
                        pc*=2;
                        break;
                    case "JSUB" :
                        log ="Sub routine으로 이동한다. l register에 돌아올 주소를 저장한다.";
                        use_pc=true;
                        b =ResourceManager.getInstance().getMemory(pc + 3, 5);
                        data = Integer.parseInt(new String(b), 16);
                        pc = data;
                        pc*=2;
                        ResourceManager.getInstance().setRegister(2, ResourceManager.getInstance().getTarget_addr());
                        ResourceManager.getInstance().set_header(ResourceManager.getInstance().getESTAB(ResourceManager.getInstance().getESTABindex_Ctrsec(pc)));
                        break;
                    case "LDA" :
                        log = "Register A에 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        if(imid) {
                            ResourceManager.getInstance().setRegister(0,Integer.parseInt(new String(b),16));
                            break;
                        }
                        data = Integer.parseInt(new String(b),16) + ResourceManager.getInstance().getTarget_addr();
                        b =ResourceManager.getInstance().getMemory(data*2,6);
                        ResourceManager.getInstance().setRegister(0,Integer.parseInt(new String(b),16));
                        break;
                    case "LDCH" :
                        log = "character를 A register에 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc + 3, 5);
                        data = Integer.parseInt(new String(b), 16);
                        b = ResourceManager.getInstance().getMemory((data + ResourceManager.getInstance().getRegister(1)) * 2, 2);
                        ResourceManager.getInstance().setRegister(0, Integer.parseInt(new String(b), 16));
                        break;
                    case "RD" :
                        log = "device로부터 1byte 읽어온다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16) + ResourceManager.getInstance().getTarget_addr();
                        b = ResourceManager.getInstance().getMemory(data*2,2);
                        ResourceManager.getInstance().initialDevice(new String(b));
                        try {
                            byte input = ResourceManager.getInstance().ReadDevice(new String (b) , ResourceManager.getInstance().getRegister(1));
                            ResourceManager.getInstance().setRegister(0,input);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case "RSUB" :
                        log = "다시 main routine으로 돌아간다.";
                        use_pc=true;
                        ResourceManager.getInstance().setRegister(8,ResourceManager.getInstance().getRegister(2));
                        pc = ResourceManager.getInstance().getRegister(2)*2;
                        ResourceManager.getInstance().set_header(ResourceManager.getInstance().getESTAB(ResourceManager.getInstance().getESTABindex_Ctrsec(0)));
                        break;
                    case "STA" ://set memory byte element 갯수 allocate
                        log = "A register에 있는 값을 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16) + ResourceManager.getInstance().getTarget_addr();
                        ResourceManager.getInstance().setMemory(data*2,String.format("%06x",ResourceManager.getInstance().getRegister(0)).getBytes(),6);
                        break;
                    case "STCH" :
                        log = "A register에 있는 character를 저장시킨다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,5);
                        data = Integer.parseInt(new String(b),16)+ResourceManager.getInstance().getRegister(1);
                        ResourceManager.getInstance().setMemory(data*2,String.format("%02x", ResourceManager.getInstance().getRegister(0)).getBytes(), 2);
                        break;
                    case "STL" :
                        log = "L register에 있는 값을 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16) + ResourceManager.getInstance().getTarget_addr();
                        ResourceManager.getInstance().setMemory(data*2,String.format("%06x", ResourceManager.getInstance().getRegister(2)).getBytes(), 6);
//                        ResourceManager.getInstance().print_memory();
                        break;
                    case "STX" :
                        log = "X register에 있는 값을 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,5);
                        data = Integer.parseInt(new String(b), 16);
                        ResourceManager.getInstance().setMemory(data*2,String.format("%06x", ResourceManager.getInstance().getRegister(1)).getBytes(), 6);
                        break;
                    case "TD" :
                        log = "device가 사용할 준비가 되었는지 test해본다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b), 16) + ResourceManager.getInstance().getTarget_addr();
                        ResourceManager.getInstance().setRegister(9,1);
                        ResourceManager.getInstance().initialDevice(new String(ResourceManager.getInstance().getMemory(data*2,2)));
                        break;
                    case "WD" :
                        log = "A register에 있는 값을 device에 1byte 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,3);
                        data = Integer.parseInt(new String(b),16) + ResourceManager.getInstance().getTarget_addr();
                        b = ResourceManager.getInstance().getMemory(data*2,2);
                        ResourceManager.getInstance().initialDevice(new String(b));
                        try {
                            ResourceManager.getInstance().writeDevice(new String (b) , (byte)(ResourceManager.getInstance().getRegister(0)));
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        break;
                    case "LDT" :
                        log = "T register에 값을 저장한다.";
                        b =ResourceManager.getInstance().getMemory(pc+3,format*2-3);
                        if(format==3){
                            data = Integer.parseInt(new String(b), 16) + ResourceManager.getInstance().getTarget_addr();
                        }
                        else{
                            data = Integer.parseInt(new String(b), 16);
                        }
                        data = Integer.parseInt(new String(ResourceManager.getInstance().getMemory(data*2,6)),16);
                        ResourceManager.getInstance().setRegister(5,data);
                        break;

                }
            }
            if(!use_pc) pc+=format*2;
        }
        addLog();
        ResourceManager.getInstance().affectVisualSimulator();
        ResourceManager.getInstance().setRegister(8,pc/2);
    }
    //남은 명령어를 모두 수행하는 메소드
    @Override
    public void allStep(){
        while(true){
            oneStep();
            if(finish)break;
        }
    }
    //실행한 결과를 로그에 추가하는 메소드
    @Override
    public void addLog(){
        ResourceManager.getInstance().addlog(log);
    }
    public opcode getinstruction(int op){
        return table.get(op);
    }
}
class opcode{
    private String name;
    private int format,opcode;
    public opcode(String name, int format, int opcode){
        this.name = name;
        this.format = format;
        this.opcode = opcode;
    }
    public int getFormat(){return format;}
    public int getOpcode(){return opcode;}
    public String getName(){return name;}
}