import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        try {
            // Create server socket at port 5000
            ServerSocket serverSocket = new ServerSocket(5000);
            System.out.println("Server started. Waiting for client...");

            // Accept client connection
            Socket socket = serverSocket.accept();
            System.out.println("Client connected!");

            // Input and output streams
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

            // Communication
            String msgFromClient;
            while ((msgFromClient = in.readLine()) != null) {
                System.out.println("Client: " + msgFromClient);
                if (msgFromClient.equalsIgnoreCase("bye")) {
                    out.println("Goodbye!");
                    break;
                }
                out.println("Server received: " + msgFromClient);
            }

            // Close connections
            in.close();
            out.close();
            socket.close();
            serverSocket.close();
            System.out.println("Server closed.");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
