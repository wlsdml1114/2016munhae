
import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.io.*;
import java.net.*;
import java.util.ArrayList;

public class NakovChatClient
{
    public static final String SERVER_HOSTNAME = "localhost";
    public static final int SERVER_PORT = 8080;
    public static String name= "user";
    public static String message="";
    public static int msg_exist=0;
    public static ArrayList<String> arrayList = new ArrayList<String>();
    private static final Insets insets = new Insets(0, 0, 0, 0);


    private JFrame mainFrame;
//    private JLabel headerLabel;
//    private JLabel statusLabel;
    public static JTextArea textarea;
    private JPanel controlPanel;

    public NakovChatClient(){
        prepareGUI();
    }

    private void prepareGUI(){
        mainFrame = new JFrame("jini chatting");
        mainFrame.setSize(600, 380);
//        mainFrame.setBackground(new Color(0,0,255));
        mainFrame.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent windowEvent) {
                System.exit(0);
            }
        });
//        headerLabel = new JLabel("", JLabel.CENTER);
//        statusLabel = new JLabel("",JLabel.CENTER);
        textarea = new JTextArea(15,35);
        JScrollPane sp = new JScrollPane(textarea);
        sp.setPreferredSize(new Dimension(450,290));
        JPanel jp = new JPanel();
        jp.add(sp);
//        jp.add(textarea);
        jp.setPreferredSize(new Dimension(300, 300));
//        jp.setSize(300, 300);
        jp.setBackground(new Color(0,0,255,120));
//        statusLabel.setSize(350, 100);
        GridBagConstraints gc = new GridBagConstraints();
//        textarea.setText("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        controlPanel = new JPanel();
        controlPanel.setLayout(new FlowLayout());
        controlPanel.setBackground(new Color(0,0,255,120));
        mainFrame.add(jp, BorderLayout.CENTER);

        controlPanel.setPreferredSize(new Dimension(350,40));
        mainFrame.add(controlPanel, BorderLayout.SOUTH);

//        mainFrame.add(headerLabel);
//        addComponent(mainFrame, textarea, 0, 0, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH);
//        mainFrame.add(textarea,gc);
//        addComponent(mainFrame, controlPanel, 0, 1, 1, 1, GridBagConstraints.CENTER, GridBagConstraints.BOTH);
//        mainFrame.add(controlPanel,gc);
//        mainFrame.add(statusLabel);
        mainFrame.setVisible(true);
    }


    private static void addComponent(Container container, Component component, int gridx, int gridy,
                                     int gridwidth, int gridheight, int anchor, int fill) {
        GridBagConstraints gbc = new GridBagConstraints(gridx, gridy, gridwidth, gridheight, 1.0, 1.0,
                anchor, fill, insets, 0, 0);
        container.add(component, gbc);
    }

    private void showTextFieldDemo(){
//        headerLabel.setText("Control in action: JTextField");

        JLabel  namelabel= new JLabel("nick name: ", JLabel.RIGHT);
        final JTextField userText = new JTextField(6);
        JLabel  msglabel= new JLabel("msg: ", JLabel.CENTER);
        final JTextField mesg = new JTextField(30);

//        JButton loginButton = new JButton("Enter");
        mesg.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (!userText.getText().equals("")) {
                    name = userText.getText();
                }
                message = mesg.getText();
                mesg.setText("");
                msg_exist++;
//                statusLabel.setText(name+","+message);
            }
        });
        controlPanel.add(namelabel);
        controlPanel.add(userText);
        controlPanel.add(msglabel);
        controlPanel.add(mesg);
//        controlPanel.add(loginButton);
        mainFrame.setVisible(true);
    }

    public static void main(String[] args)
    {

//        for (int i=0;i<22;++i){
//            arrayList.add("");
//        }
        NakovChatClient  swingControlDemo = new NakovChatClient();
        swingControlDemo.showTextFieldDemo();
        BufferedReader in = null;
        PrintWriter out = null;
        try {
            // Connect to Nakov Chat Server
            Socket socket = new Socket(SERVER_HOSTNAME, SERVER_PORT);
            in = new BufferedReader(
                    new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(
                    new OutputStreamWriter(socket.getOutputStream()));
            System.out.println("Connected to server " +
                    SERVER_HOSTNAME + ":" + SERVER_PORT);
        } catch (IOException ioe) {
            System.err.println("Can not establish connection to " +
                    SERVER_HOSTNAME + ":" + SERVER_PORT);
            ioe.printStackTrace();
            System.exit(-1);
        }

        // Create and start Sender thread
        Sender sender = new Sender(out);
        sender.setDaemon(true);
        sender.start();

        try {
            // Read messages from the server and print them
            String message;
            while ((message=in.readLine()) != null) {
//                System.out.println(message);//textarea에 추가
                arrayList.add(message);
                String temp ="";
                for(int i = arrayList.size()-1;i>=0;--i) temp+=arrayList.get(i)+"\n";
                NakovChatClient.textarea.setText(temp);
            }
        } catch (IOException ioe) {
            System.err.println("Connection to server broken.");
            ioe.printStackTrace();
        }

    }
}

class Sender extends Thread
{
    private PrintWriter mOut;

    public Sender(PrintWriter aOut)
    {
        mOut = aOut;
    }

    /**
     * Until interrupted reads messages from the standard input (keyboard)
     * and sends them to the chat server through the socket.
     */
    public void run()
    {
        //            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        while (!isInterrupted()) {
//                String message = in.readLine();
            if(NakovChatClient.msg_exist!=0){
                mOut.println(NakovChatClient.message + "," + NakovChatClient.name);
                mOut.flush();
                NakovChatClient.msg_exist=0;
            }
        }
    }
}