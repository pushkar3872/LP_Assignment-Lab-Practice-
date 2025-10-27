import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        int port = 5000; // Port number
        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("Server started. Waiting for client...");

            Socket socket = serverSocket.accept(); // Wait for client to connect
            System.out.println("Client connected!");

            // Input and output streams
            BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter output = new PrintWriter(socket.getOutputStream(), true);

            // Communication
            String message;
            while ((message = input.readLine()) != null) {
                System.out.println("Client says: " + message);
                output.println("Server received: " + message);

                if (message.equalsIgnoreCase("exit")) {
                    System.out.println("Client disconnected.");
                    break;
                }
            }

            socket.close();
            serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
