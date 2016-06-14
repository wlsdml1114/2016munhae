
public class ESTAB{
    String control_section;
    String name;
    int address;
    int length;
    public ESTAB(){
        control_section=null;
        name=null;
        address=0;
        length=0;
    }
    public ESTAB(String name, int address){
        this.name = name;
        this.control_section="";
        this.address=address;
    }
    public ESTAB(String control,int address,int length){
        this.control_section = control;
        this.name="";
        this.address=address;
        this.length = length;
    }
}