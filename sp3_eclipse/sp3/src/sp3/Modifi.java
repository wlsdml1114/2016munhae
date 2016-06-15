package sp3;
/**
 * Created by jini on 16. 6. 11.
 */
public class Modifi {
    String symbol;
    int address;
    int length;
    char pm;
    public Modifi(int address, int length , char pm,String symbol){
        this.symbol = symbol;
        this.address = address;
        this.length = length;
        this.pm = pm;
    }
    @Override
    public String toString(){
        return symbol + " "+address+" "+length+" "+pm;
    }
}
