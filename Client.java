import java.io.*;
import java.net.*;

public class Client {
    public static void main(String[] args) {
        String host = "127.0.0.1"; // Localhost (same machine)
        int port = 5000;

        try (Socket socket = new Socket(host, port)) {
            BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));
            BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter output = new PrintWriter(socket.getOutputStream(), true);

            System.out.println("Connected to server. Type messages (type 'exit' to quit):");

            String message;
            while (true) 
            {
                System.out.print("You: ");
                message = userInput.readLine();
                output.println(message);

                if (message.equalsIgnoreCase("exit")) 
                {
                    break;
                }

                String response = input.readLine();
                System.out.println(response);
            }

            socket.close();

        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }
    }
}
