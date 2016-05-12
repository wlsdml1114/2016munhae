
import java.net.*;
import java.io.*;

public class NakovChatServer
{
    public static final int LISTENING_PORT = 8080;

    public static void main(String[] args)
    {
        // Open server socket for listening
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(LISTENING_PORT);
            System.out.println("NakovChatServer started on port " + LISTENING_PORT);
        } catch (IOException se) {
            System.err.println("Can not start listening on port " + LISTENING_PORT);
            se.printStackTrace();
            System.exit(-1);
        }

        // Start ServerDispatcher thread
        ServerDispatcher serverDispatcher = new ServerDispatcher();
        serverDispatcher.start();

        // Accept and handle client connections
        while (true) {
            try {
                Socket socket = serverSocket.accept();
                ClientInfo clientInfo = new ClientInfo();
                clientInfo.mSocket = socket;
                ClientListener clientListener =
                        new ClientListener(clientInfo, serverDispatcher);
                ClientSender clientSender =
                        new ClientSender(clientInfo, serverDispatcher);
                clientInfo.mClientListener = clientListener;
                clientInfo.mClientSender = clientSender;
                clientListener.start();
                clientSender.start();
                serverDispatcher.addClient(clientInfo);
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
    }

}
