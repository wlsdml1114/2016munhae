import java.io.*;

/**
 * Created by jini on 16. 3. 31.
 */
public class PL {
    public static final int EOF = -1;
    public static final int LETTER = 0;
    public static final int DIGIT = 1;
    public static final int UNKNOWN = 99;
    public static final int INT_LIT = 10;
    public static final int IDENT =11;
    public static final int RESERVED =12;
    public static final int ASSIGN_OP =20;
    public static final int ADD_OP =21;
    public static final int SUB_OP =22;
    public static final int MULT_OP =23;
    public static final int DIV_OP =24;
    public static final int LEFT_PAREN =25;
    public static final int RIGHT_PAREN =26;
    public static final int SEMICOLON =27;
    public static final int EQUAL =28;
    public static final int COMMA =29;
    public static final int FOR_CODE =30;
    public static final int IF_CODE =31;
    public static final int ELSE_CODE =32;
    public static final int WHILE_CODE =33;
    public static final int DO_CODE =34;
    public static final int INT_CODE =35;
    public static final int FLOAT_CODE =36;
    public static final int SWITCH_CODE =37;
    public static final int BIG_LEFT_PAREN =41;
    public static final int BIG_RIGHT_PAREN =42;
    public static final int RETURN =39;
    public static final int DOT =43;
    public static final int SHOP =44;
    public static final int LEFT_BRACKET =45;
    public static final int RIGHT_BRACKET =46;
    public static final int INCLUDE =38;
    public static final int MAIN =40;
    static int charClass;
    char nextChar;
    String lexeme = "";
    int lexLen;
    int nextToken;
    String reserved[] = {"for","if","else","while","do","int","float","switch","include","return","main"};
    static BufferedReader reader;
    public static void main(String[] args){
        PL pl = new PL();
        pl.start();
    }

    public void start(){
        try {
            File file = new File("a.c");
            FileReader fileReader = new FileReader(file);
            reader = new BufferedReader(fileReader);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        getChar();
        do {
            if(nextToken==EOF){
                return;
            }
            lex();
        } while (nextToken != EOF);
    }


    int lookup(char ch) {
        switch (ch) {
            case '{':
                addChar();
                nextToken = BIG_LEFT_PAREN;
                break;
            case '}':
                addChar();
                nextToken = BIG_RIGHT_PAREN;
                break;
            case '(':
                addChar();
                nextToken = LEFT_PAREN;
                break;
            case ')':
                addChar();
                nextToken = RIGHT_PAREN;
                break;
            case '+':
                addChar();
                nextToken = ADD_OP;
                break;
            case '-':
                addChar();
                nextToken = SUB_OP;
                break;
            case '*':
                addChar();
                nextToken = MULT_OP;
                break;
            case '/':
                addChar();
                nextToken = DIV_OP;
                break;
            case ';':
                addChar();
                nextToken = SEMICOLON;
                break;
            case '=':
                addChar();
                nextToken = EQUAL;
                break;
            case ',':
                addChar();
                nextToken = COMMA;
                break;
            case '.':
                addChar();
                nextToken = DOT;
                break;
            case '#':
                addChar();
                nextToken = SHOP;
                break;
            case '<':
                addChar();
                nextToken = LEFT_BRACKET;
                break;
            case '>':
                addChar();
                nextToken = RIGHT_BRACKET;
                break;
            default:
                addChar();
                nextToken = EOF;
                break;
        }
        return nextToken;
    }

    int lex() {
        lexeme="";
        getNonBlank();
        switch (charClass) {
                /* Parse identifiers */
            case LETTER:
                addChar();
                getChar();
                while (charClass == LETTER || charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                for(int i=0;i<11;++i){
                    if(reserved[i].equals(lexeme)==true){
                        nextToken=30+i;
                        break;
                    }
                    else{
                        nextToken=IDENT;
                    }
                }
                break;
                        /* Parse integer literals */
            case DIGIT:
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = INT_LIT;
                break;
                        /* Parentheses and operators */
            case UNKNOWN:
                lookup(nextChar);
                getChar();
                break;
                        /* EOF */
            case EOF:
                nextToken = EOF;
                lexeme="EOF";
                break;
        } /* End of switch */
        System.out.println("Next token is: " + nextToken + ", Next lexeme is " + lexeme);
        return nextToken;
    } /* End of function lex */

    public void addChar() {
        if (lexLen <= 98) {
            lexeme+=nextChar;
            lexLen++;
        }
        else
            System.out.print("Error - lexeme is too long \n");
    }
    /*****************************************************/
/* getChar - a function to get the next character of
   input and determine its character class */
    public void getChar() {
        int r;
        try {
            r = reader.read();
            if (r != -1) {
                nextChar = (char)r;
                if (isalpha(nextChar))
                    charClass = LETTER;
                else if (isdigit(nextChar))
                    charClass = DIGIT;
                else charClass = UNKNOWN;
            }
            else
                charClass = EOF;
            if(r==-1){
                System.out.println("Next token is: " + "-1" + ", Next lexeme is " + "EOF");
                System.exit(1);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void getNonBlank() {
        while (isspace(nextChar))
            getChar();
    }

    private boolean isspace(char nextChar) {
        if(nextChar==' ')return true;
        if(nextChar=='\n')return true;
        if(nextChar=='\t')return true;
        return false;
    }

    private boolean isdigit(char nextChar) {
        if(nextChar>='0'&&nextChar<='9'){
            return true;
        }
        return false;

    }

    private boolean isalpha(char nextChar) {
        if(nextChar>='a'&&nextChar<='z'){
            return true;
        }
        else if(nextChar>='A'&&nextChar<='Z'){
            return true;
        }
        return false;

    }
}
