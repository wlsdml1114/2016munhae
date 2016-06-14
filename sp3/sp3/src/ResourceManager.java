
import sp.project2.interfacepack.*;

import java.io.*;
import java.util.ArrayList;

public class ResourceManager implements sp.project2.interfacepack.ResourceManager {
    private static final int MEMORY_SIZE=10000;
    private static ResourceManager instance = new ResourceManager();
    byte[] memory;
    int start_address, length, addr_1st_inst, start_addr_in_mem, target_addr;
    int [] regi;
    String device;
    String program;
    ArrayList<ESTAB> estab;

    //메모리 영역을 초기화 하는 메소드
    public ResourceManager() {
        estab = new ArrayList<ESTAB>();
        initializeMemory();
        initializeRegister();
        initialDevice("");
    }

    public static ResourceManager getInstance() {
        return instance;
    }

    public void add_estab(ESTAB inst) {
        estab.add(inst);
    }

    @Override
    public void initializeMemory() {
        memory = new byte[MEMORY_SIZE];
    }

    //각 레지스터 값을 초기화 하는 메소드
    @Override
    public void initializeRegister() {
        regi = new int[10];
        start_address = 0;
        length = 0;
        addr_1st_inst = 0;
        start_addr_in_mem = 0;
        target_addr = 0;
    }

    //디바이스 접근에 대한 메소드
    //디바이스는 각 이름과 매칭되는 파일로 가정한다
    //(F1 이라는 디바이스를 읽으면 F1 이라는 파일에서 값을 읽는다.)
    //해당 디바이스(파일)를 사용 가능한 상태로 만드는 메소드
    @Override
    public void initialDevice(String devName) {
        device = devName;
    }

    //선택한 디바이스(파일)에 값을 쓰는 메소드. 파라미터는 변경 가능하다.
    @Override
    public void writeDevice(String devName, byte[] data, int size) {
    }
    public void writeDevice(String devName, byte data) throws IOException {
        PrintWriter pw = new PrintWriter(new FileWriter("./"+devName,true));
        pw.print(data);
        pw.close();
    }
    //선택한 디바이스(파일)에서 값을 읽는 메소드. 파라미터는 변경 가능하다.
    @Override
    public byte[] readDevice(String devName, int size) {
        return null;
    }
    public byte ReadDevice(String devName, int index) throws IOException {
        byte[] bytes = new byte[10000];
        FileInputStream input = new FileInputStream("./"+devName);
        input.read(bytes);
        input.close();
        return bytes[index];
    }

    //메모리 영역에 값을 쓰는 메소드
    @Override
    public void setMemory(int locate, byte[] data, int size) {
        for(int i=0;i<size;++i){
            memory[locate+i] = data[i];
        }
    }

    //레지스터에 값을 세팅하는 메소드. regNum은 레지스터 종류를 나타낸다.
    @Override
    public void setRegister(int regNum, int value) {
        regi[regNum]= value;
    }

    //메모리 영역에서 값을 읽어오는 메소드
    @Override
    public byte[] getMemory(int locate, int size) {
        byte[] bytes = new byte[size];
        for(int i=0;i<size;++i){
            bytes[i] = memory[locate+i];
        }
        return bytes;
    }

    //레지스터에서 값을 가져오는 메소드
    @Override
    public int getRegister(int regNum) {
        return regi[regNum];
    }

    //바뀐 값들을 GUI에 적용시키는 메소드
    @Override
    public void affectVisualSimulator() {
        VisualSimulator.getInstance().jt_A_dec.setText(Integer.toString(regi[0]));
        VisualSimulator.getInstance().jt_A_hex.setText(Integer.toString(regi[0],16));
        VisualSimulator.getInstance().jt_X_dec.setText(Integer.toString(regi[1]));
        VisualSimulator.getInstance().jt_X_hex.setText(Integer.toString(regi[1],16));
        VisualSimulator.getInstance().jt_L_dec.setText(Integer.toString(regi[2]));
        VisualSimulator.getInstance().jt_L_hex.setText(Integer.toString(regi[2],16));
        VisualSimulator.getInstance().jt_PC_dec.setText(Integer.toString(regi[8]));
        VisualSimulator.getInstance().jt_PC_hex.setText(Integer.toString(regi[8],16));
        VisualSimulator.getInstance().jt_SW.setText(Integer.toString(regi[9]));
        VisualSimulator.getInstance().jt_B_dec.setText(Integer.toString(regi[3]));
        VisualSimulator.getInstance().jt_B_hex.setText(Integer.toString(regi[3],16));
        VisualSimulator.getInstance().jt_S_dec.setText(Integer.toString(regi[4]));
        VisualSimulator.getInstance().jt_S_hex.setText(Integer.toString(regi[4],16));
        VisualSimulator.getInstance().jt_T_dec.setText(Integer.toString(regi[5]));
        VisualSimulator.getInstance().jt_T_hex.setText(Integer.toString(regi[5],16));
        VisualSimulator.getInstance().jt_F.setText(Integer.toString(regi[6]));
        VisualSimulator.getInstance().jt_start_address.setText(Integer.toString(start_address));
        VisualSimulator.getInstance().jt_length.setText(Integer.toString(length));
        VisualSimulator.getInstance().jt_addr_1st_inst.setText(Integer.toString(addr_1st_inst));
        VisualSimulator.getInstance().jt_start_addr_in_mem.setText(Integer.toString(start_addr_in_mem));
        VisualSimulator.getInstance().jt_target_address.setText(Integer.toString(target_addr));
        VisualSimulator.getInstance().jt_program_name.setText(program);
        VisualSimulator.getInstance().jt_device.setText(device);
    }

    public void set_program(String name){
        program = name;
    }

    public int getESTAB(String str){
        for(int i=0;i<estab.size();++i){
            if(estab.get(i).control_section.compareTo(str)==0||
                    estab.get(i).name.compareTo(str)==0){
                return estab.get(i).address;

            }
        }
        return -1;
    }
    public void print_memory(){
        for(int i=0;i<MEMORY_SIZE;++i){
            if(i%8==0)System.out.print(" ");
            if(i%32==0)System.out.println(String.format("%04X",i/2));
            System.out.print((char)memory[i]);
        }
    }
}

