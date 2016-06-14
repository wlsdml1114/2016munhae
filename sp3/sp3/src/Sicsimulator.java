
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;

import sp.project2.interfacepack.SicSimulator;

public class Sicsimulator implements SicSimulator{
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
        int pc = ResourceManager.getInstance().getRegister(8);//pc is 8
        byte[] bytes = ResourceManager.getInstance().getMemory(pc, 2);
        int op = Integer.parseInt(new String(bytes),16);

    }
    //남은 명령어를 모두 수행하는 메소드
    @Override
    public void allStep(){

    }
    //실행한 결과를 로그에 추가하는 메소드
    @Override
    public void addLog(){

    }
}
class opcode{
    String name;
    int format,opcode;
    public opcode(String name, int format, int opcode){
        this.name = name;
        this.format = format;
        this.opcode = opcode;
    }
}