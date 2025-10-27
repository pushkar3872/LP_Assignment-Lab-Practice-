import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        try {
            // Connect to server on localhost and port 5000
            Socket socket = new Socket("localhost", 5000);
            System.out.println("Connected to server!");

            // Input and output streams
            BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

            // Communication
            String msgToServer, msgFromServer;
            while (true) {
                System.out.print("You: ");
                msgToServer = input.readLine();
                out.println(msgToServer);

                msgFromServer = in.readLine();
                System.out.println("Server: " + msgFromServer);

                if (msgToServer.equalsIgnoreCase("bye")) break;
            }

            // Close connections
            input.close();
            in.close();
            out.close();
            socket.close();
            System.out.println("Connection closed.");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
