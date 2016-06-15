
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class SicLoader implements sp.project2.interfacepack.SicLoader{
    int cur_section_startaddress=0,start_address=0,length=0;
    ArrayList<Modifi> modi = new ArrayList<Modifi>();
    private static SicLoader instance = new SicLoader();
    //파일로부터 목적코드를 읽어와 메모리에 로드
    public static SicLoader getInstance(){return instance;}
    private SicLoader() {
    }

    @Override
    public void load(File objFile){
        try {
            Scanner scanner = new Scanner(objFile);
            while(scanner.hasNextLine()){
                String line = scanner.nextLine();
                if(line.length()==0)continue;
                readLine(line);
            }
            ResourceManager.getInstance().affectVisualSimulator();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        modifi();
        ResourceManager.getInstance().affectVisualSimulator();
        ResourceManager.getInstance().print_memory();
    }
    //목적코드의 한 줄을 읽고, 각 헤더(H, T, M 등)에 맞는 기능을 수행하여
    //각 메모리 및 명령어 리스트를 초기화 한다.
    @Override
    public void readLine(String line){
        switch(line.charAt(0)){
            case 'H':
                handleheader(line.substring(1));
                break;
            case 'D':
                handleEdef(line.substring(1));
                break;
            case 'R':
                handleEref(line.substring(1));
                break;
            case 'T':
                handleText(line.substring(1));
                break;
            case 'M':
                handleModi(line.substring(1));
                break;
            case 'E':
                handleEnd(line.substring(1));
                break;
            default:
                break;
        }
    }
    public void handleheader(String line){
        String str = line.substring(0, 6);
        str = str.replaceAll("\\s","");
        cur_section_startaddress += Integer.parseInt(line.substring(6, 12),16)*2;
        length = Integer.parseInt(line.substring(12),16)*2;
        if (cur_section_startaddress==0){
            ResourceManager.getInstance().set_program(str,length);
        }
        ResourceManager.getInstance().add_estab(new ESTAB(str, cur_section_startaddress, length));
//        System.out.println(str+" "+cur_section_startaddress+" "+length);

    }
    public void handleEdef(String line){
        String str;
        int addr;
        while(line.length()!=0){
            str = line.substring(0, 6);
            addr = Integer.parseInt(line.substring(6, 12), 16)*2;
            line = line.substring(12);
            ResourceManager.getInstance().add_estab(new ESTAB(str, cur_section_startaddress + addr));
//            System.out.println(str+" "+(cur_section_startaddress+addr));
        }
    }
    public void handleEref(String line){
    // do nothing
    }
    public void handleText(String line){
        int startAddress = Integer.parseInt(line.substring(0,6),16)*2;
        int leng = Integer.parseInt(line.substring(6,8),16)*2;
        byte[] bytes = line.substring(8).getBytes();
        ResourceManager.getInstance().setMemory(cur_section_startaddress+startAddress,bytes,leng);
        String str = new String(bytes);
        for(int i=0;i<str.length()/2-2;++i){
            int opcod = Integer.parseInt(str.substring(i * 2, i * 2 + 3), 16);
            System.out.print(str.substring(i * 2, i * 2 + 3)+" "+opcod/16/4*4+" ");
            if(!Sicsimulator.getInstance().table.containsKey(opcod/16/4*4)) break;
            System.out.print("cde");
            int format = Sicsimulator.getInstance().getinstruction(opcod/16/4*4).getFormat();
            if (format ==2){
                ResourceManager.getInstance().addinstruction(str.substring(i*2, i*2 + 4));
                i+=1;
            }else{
                if(opcod%2==1){
                    ResourceManager.getInstance().addinstruction(str.substring(i*2, i*2 + 8));
                    i+=3;
                }
                else {
                    ResourceManager.getInstance().addinstruction(str.substring(i*2, i*2 + 6));
                    i+=2;
                }
            }
        }
    }
    public void handleModi(String line){
        modi.add(new Modifi(Integer.parseInt(line.substring(0,6),16)*2+cur_section_startaddress,Integer.parseInt(line.substring(6,8)),line.charAt(8),line.substring(9)));
    }
    public void handleEnd(String line){
        cur_section_startaddress+=length;
    }
    public void modifi(){
        for(int j=0;j<modi.size();++j){
//            System.out.println(modi.get(j));
            ESTAB estab = ResourceManager.getInstance().getESTAB(ResourceManager.getInstance().getESTABindex(modi.get(j).symbol));
            int adr = estab.address/2;
            int cur_add = modi.get(j).address;
            char c = modi.get(j).pm;
            if(modi.get(j).length==5)cur_add++;
            byte[] bytes = ResourceManager.getInstance().getMemory(cur_add,modi.get(j).length);
//            System.out.print(cur_add+" "+adr+" "+c+" ");
//            for(int i=0;i<bytes.length;++i){
//                System.out.print(bytes[i]);
//            }
//            System.out.print(" ");
            int res;
            if(c=='+'){
                res = Integer.parseInt(new String(bytes),16)+adr;
            }else{
                res = Integer.parseInt(new String(bytes),16)-adr;
            }
//            System.out.print(res + " ");
            String output = String.format("%06X",res);
            int k=5;
            for(int i=modi.get(j).length-1;i>=0;--i){
                bytes[i] = (byte)output.charAt(k--);
            }
//            for(int i=0;i<bytes.length;++i){
//                System.out.print(bytes[i]+" ");
//            }
//            System.out.println();

            ResourceManager.getInstance().setMemory(cur_add, bytes, bytes.length);
        }
    }
}
