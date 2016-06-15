
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class VisualSimulator implements sp.project2.interfacepack.VisualSimulator{
    private static VisualSimulator instance = new VisualSimulator();

    public static final int textlength= 10;

    File file;

    JTextField jt_program_name = new JTextField(textlength);
    JTextField jt_start_address = new JTextField(textlength);
    JTextField jt_length = new JTextField(textlength);
    JTextField jt_A_dec = new JTextField(textlength);
    JTextField jt_A_hex = new JTextField(textlength);
    JTextField jt_X_dec = new JTextField(textlength);
    JTextField jt_X_hex = new JTextField(textlength);
    JTextField jt_L_dec = new JTextField(textlength);
    JTextField jt_L_hex = new JTextField(textlength);
    JTextField jt_PC_dec = new JTextField(textlength);
    JTextField jt_PC_hex = new JTextField(textlength);
    JTextField jt_SW = new JTextField(textlength*2+1);
    JTextField jt_B_dec = new JTextField(textlength);
    JTextField jt_B_hex = new JTextField(textlength);
    JTextField jt_S_dec = new JTextField(textlength);
    JTextField jt_S_hex = new JTextField(textlength);
    JTextField jt_T_dec = new JTextField(textlength);
    JTextField jt_T_hex = new JTextField(textlength);
    JTextField jt_F = new JTextField(textlength*2+1);
    JTextField jt_addr_1st_inst = new JTextField(textlength);
    JTextField jt_start_addr_in_mem = new JTextField(textlength);
    JTextField jt_target_address = new JTextField(textlength);
    JTextField jt_device = new JTextField(textlength);

    JTextArea txtLog = new JTextArea();
    JTextArea Logarea = new JTextArea();
    public static VisualSimulator getInstance(){
        return instance;
    }

    private VisualSimulator() {
        JPanel mainPanel = new JPanel();
        JPanel Ljpanel = new JPanel();
        Ljpanel.setLayout(new GridLayout(0,1));
        JTextField jt_filename = new JTextField(textlength);
        JFrame jFrame = new JFrame();
        JPanel jpanel = new JPanel();
        jpanel.add(new JLabel("FileName"));
        jpanel.add(jt_filename);
        JButton jb_fileopen = new JButton("Open");
        jb_fileopen.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                fileChooser.setCurrentDirectory(new File("./"));
                int result = fileChooser.showOpenDialog(jFrame);
                if (result == JFileChooser.APPROVE_OPTION) {
                    file = fileChooser.getSelectedFile();
                    jt_filename.setText(file.getName());

//                    SicLoader.getInstance().load(file);
                    initialize();
                }
            }
        });
        jpanel.add(jb_fileopen);
        Ljpanel.add(jpanel);

        Ljpanel.add(new JLabel("H(Header Record)"));

        JPanel program_name = new JPanel();
        program_name.add(new JLabel("Program Name     "));
        program_name.add(jt_program_name);
        program_name.add(new JLabel("                                 "));
        Ljpanel.add(program_name);

        JPanel start_address = new JPanel();
        start_address.add(new JLabel("Start Address       "));
        start_address.add(jt_start_address);
        start_address.add(new JLabel("                                 "));
        Ljpanel.add(start_address);

        JPanel JLength = new JPanel();
        JLength.add(new JLabel("Length                    "));
        JLength.add(jt_length);
        JLength.add(new JLabel("                                 "));
        Ljpanel.add(JLength);

        Ljpanel.add(new JLabel("Register"));

        JPanel Dec_HEx = new JPanel();
        Dec_HEx.add(new JLabel("                                          "));
        Dec_HEx.add(new JLabel("DEC                              "));
        Dec_HEx.add(new JLabel("HEX                              "));
        Ljpanel.add(Dec_HEx);

        JPanel A = new JPanel();
        A.add(new JLabel("A(#0)                            "));
        A.add(jt_A_dec);
        A.add(jt_A_hex);
        Ljpanel.add(A);

        JPanel X = new JPanel();
        X.add(new JLabel("X(#1)                            "));
        X.add(jt_X_dec);
        X.add(jt_X_hex);
        Ljpanel.add(X);

        JPanel L = new JPanel();
        L.add(new JLabel("L(#2)                            "));
        L.add(jt_L_dec);
        L.add(jt_L_hex);
        Ljpanel.add(L);

        JPanel PC = new JPanel();
        PC.add(new JLabel("PC(#8)                         "));
        PC.add(jt_PC_dec);
        PC.add(jt_PC_hex);
        Ljpanel.add(PC);

        JPanel Sw = new JPanel();
        Sw.add(new JLabel("SW(#9)                         "));
        Sw.add(jt_SW);
        Ljpanel.add(Sw);

        Ljpanel.add(new JLabel("Register"));

        JPanel Dec_HEx2 = new JPanel();
        Dec_HEx2.add(new JLabel("                                          "));
        Dec_HEx2.add(new JLabel("DEC                              "));
        Dec_HEx2.add(new JLabel("HEX                              "));
        Ljpanel.add(Dec_HEx2);

        JPanel B = new JPanel();
        B.add(new JLabel("B(#3)                            "));
        B.add(jt_B_dec);
        B.add(jt_B_hex);
        Ljpanel.add(B);

        JPanel S = new JPanel();
        S.add(new JLabel("S(#4)                            "));
        S.add(jt_S_dec);
        S.add(jt_S_hex);
        Ljpanel.add(S);

        JPanel T = new JPanel();
        T.add(new JLabel("T(#5)                            "));
        T.add(jt_T_dec);
        T.add(jt_T_hex);
        Ljpanel.add(T);

        JPanel F = new JPanel();
        F.add(new JLabel("F(#6)                            "));
        F.add(jt_F);
        Ljpanel.add(F);

        mainPanel.add(Ljpanel);

        JPanel Rjpanel = new JPanel();
        Rjpanel.setLayout(new GridLayout(0,1));

        Rjpanel.add(new JLabel("                                                         Log"));

        Rjpanel.add(new JLabel("E(End Record)"));

        JPanel Addr_1st_inst = new JPanel();
        Addr_1st_inst.add(new JLabel("Addr 1th inst"));
        Addr_1st_inst.add(jt_addr_1st_inst);
        Rjpanel.add(Addr_1st_inst);


        JPanel start_addr_in_mem = new JPanel();
        start_addr_in_mem.add(new JLabel("Start addr in mem"));
        start_addr_in_mem.add(jt_start_addr_in_mem);
        Rjpanel.add(start_addr_in_mem);

        JPanel target_address = new JPanel();
        target_address.add(new JLabel("Target addr            "));
        target_address.add(jt_target_address);
        Rjpanel.add(target_address);

        JPanel using_device = new JPanel();
        using_device.add(new JLabel("사용중인 장치       "));
        using_device.add(jt_device);
        Rjpanel.add(using_device);

        JPanel buttons = new JPanel();
        JButton ones = new JButton("실행(1 step)");
        ones.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                oneStep();
            }
        });
        buttons.add(ones);

        JButton alls = new JButton("실행(all)");
        alls.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                allStep();
            }
        });
        buttons.add(alls);

        JButton finish = new JButton("종료");
        finish.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(ActionEvent e) {
                jFrame.dispose();
            }
        });
        buttons.add(finish);
        Rjpanel.add(buttons);

        Rjpanel.add(new JLabel("Instructions"));

        JPanel textarea = new JPanel();
        JScrollPane scrollPane = new JScrollPane(txtLog);
        scrollPane.setPreferredSize(new Dimension(120,60));
        textarea.add(scrollPane);
        Rjpanel.add(textarea);


        mainPanel.add(Rjpanel);
        JPanel allpanel = new JPanel();
        allpanel.setLayout(new GridLayout(0,1));
        allpanel.add(mainPanel);

        JPanel logpanel = new JPanel();
        JScrollPane sp = new JScrollPane(Logarea);
        sp.setPreferredSize(new Dimension(350,600));
        mainPanel.add(sp);
        allpanel.add(logpanel);

        jFrame.add(allpanel);
        jFrame.setLayout(new FlowLayout());
        jFrame.setSize(1200,700);
        jFrame.setVisible(true);

        jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    }
    public static void main(String args[]){
        //그냥 존재
    }
    //남은 명령어를 모두 실행하는 메소드로써 SIC 시뮬레이터에 작업을 전달
    @Override
    public void allStep() {
        Sicsimulator.getInstance().allStep();
        // TODO Auto-generated method stub

    }
    //각각 필요한 오브젝트를 생성해주어 실행가능 상태로 만듬
    @Override
    public void initialize() {
        Sicsimulator.getInstance().initialize(null);
        SicLoader.getInstance().load(file);
    }
    //하나의 명령어만 실행하는 메소드로써 SIC 시뮬레이터에게 작업을 전달
    @Override
    public void oneStep() {
        Sicsimulator.getInstance().oneStep();
        // TODO Auto-generated method stub

    }
    //이외에 GUI 관련 메소드들(set, view 등은 자유구현)

}