/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.*;
import java.net.ConnectException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.lang.Thread;
import javafx.application.Application;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontPosture;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

public class sclient extends Application {
    // Main app extends application because has gui
    // Gui is provided by javafx
    private Socket clientSocket; // Socket this program will connect to
    private PrintWriter out; // Out of this program. To write to socket
    private Scanner in; // In of this program. To get from socket
    public boolean clientStarted = false;
    public boolean clientOn = true;
    // You can set these variables
    public boolean printToConsole = false; // Prints stream to console too. It is for debugging. Or if javafx fails.
    public int FPS_MAX_DELAY = 50; // 1000/20 = 50 or 1sec/fps = k ms System is smart this is fps max value if fps
                                   // is low system won't delay. How ever don't set target fps too high it will
                                   // stutter.

    public void startConnection(String ip, int port) throws IOException {
        // Connection starter method
        clientSocket = new Socket(ip, port); // Getting a socket with given ip and port
        out = new PrintWriter(clientSocket.getOutputStream(), true); // Setting up in and out of new socket
        in = new Scanner(clientSocket.getInputStream());
    }

    public void stopConnection() throws IOException {
        clientOn = false; // Stops background task
        in.close(); // Closes socket and socket in and out
        out.close();
        clientSocket.close();
        // System.out.println("Stopped connection."); // Prints the status
        // Commented, you can open if you want.
    }

    public static void main(String[] args) throws Exception {
        // Main just starts fx, the background threaded from there.
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        // The javafx function
        Parameters parameters = getParameters();
        // Program arguments are get
        List<String> allParameters = parameters.getRaw();
        // They are turned to String[]
        // That was how I coded before I coded javafx so
        // Needs to turn it old way
        // System.out.println(unnamedParameters);
        String[] args = allParameters.toArray(new String[0]);
        // System.out.println(args);
        // Program title is set
        primaryStage.setTitle("BBM342 Project 2 Kayla Akyüz 21726914");
        // The panel which 2 buttons and text are will go
        StackPane layout = new StackPane();
        // The text area, I used label. It was text area first but label looks better.
        // Streaming done to here
        Label label = new Label();
        // The client which will be used to run background tasks on
        sclient client = new sclient();
        // Background task is created
        Task dynamicTimeTask = new Task<Void>() {
            @Override
            protected Void call() throws Exception {
                // Initial values are -1
                // If anything goes wrong we can determine this way
                int port = -1;
                String address = "-1";
                int channel = -1;
                // Reading arguments
                int argCount = args.length;
                for (int i = 0; i < argCount; i++) {
                    // System.out.println(args[i]);
                    // Why double ? Because – (from project document) and - (normal middle line) are
                    // not equal. TA used a special line in document.
                    // Just to make sure
                    if (args[i].compareToIgnoreCase("–p") == 0) {
                        port = Integer.parseInt(args[i + 1]);
                    } else if (args[i].compareToIgnoreCase("-p") == 0) {
                        port = Integer.parseInt(args[i + 1]);
                    } else if (args[i].compareToIgnoreCase("–a") == 0) {
                        address = args[i + 1];
                    } else if (args[i].compareToIgnoreCase("-a") == 0) {
                        address = args[i + 1];
                    } else if (args[i].compareToIgnoreCase("–ch") == 0) {
                        channel = Integer.parseInt(args[i + 1]);
                    } else if (args[i].compareToIgnoreCase("-ch") == 0) {
                        channel = Integer.parseInt(args[i + 1]);
                    }
                }
                // Checking validity of arguments
                // I tried to code smart so I can catch if you make mistakes and warn you
                // But still please be careful.
                if (port == -1) {
                    System.out.println(
                            "Can not read port value from arguments make sure you use following configuration: ");
                    System.out.println("...    –p <port>");
                    System.out.println("Type port number (Ex:59898) instead of <port>, without angle brackets.");
                } else if (address.compareToIgnoreCase("-1") == 0) {
                    System.out.println(
                            "Can not read address value from arguments make sure you use following configuration: ");
                    System.out.println("...    –a <address>");
                    System.out.println(
                            "Type connection address (Ex:localhost) instead of <address>, without angle brackets.");
                } else if (channel == -1) {
                    System.out.println(
                            "Can not read channel value from arguments make sure you use following configuration: ");
                    System.out.println("...    –ch <channel>");
                    System.out.println("Type channel id (Ex:1) instead of <channel>, without angle brackets.");
                } else {
                    // Server message and respond initialized
                    String server_message;
                    int server_response = 0; // Used to determine what is the response
                    try {
                        // Client starts to connect given address and port
                        client.startConnection(address, port);
                        // After connection server is pinged automatically and greets us. Its greeting
                        // differs.
                        // We determine server status from its greeting
                        if (client.in.hasNextLine()) { // Waiting server to respond
                            // Reading server response
                            server_message = client.in.nextLine();
                            // Printing server message to console.
                            System.out.println(server_message);
                            if (server_message.compareToIgnoreCase("sorry we were about to close!") == 0) {
                                // This gets send if server was about to close.
                                // Server closes when a client tries to connect.
                                // It is like this:
                                // Server is closing but listening to port.
                                // Port gets connection but server checks and sees it was closing.
                                // This is because server listening to port is not interrupted while server is
                                // closing.
                                // That is because if there is someone watching they don't get kicked out.
                                // Next connection attempt will be denied.
                                // And current watcher will be terminated.
                                server_response = -1;
                                if (client.in.hasNextLine()) {
                                    System.out.println(client.in.nextLine());
                                }
                            } else if (server_message
                                    .compareToIgnoreCase("Greetings! This is the streaming service.") == 0) {
                                // Means server is on.
                                client.out.println(channel);
                                server_message = client.in.nextLine();
                                System.out.println(server_message);
                                if (server_message
                                        .compareToIgnoreCase("There is no available slot in this channel.") == 0) {
                                    // Server has 300 client capacity per channel. 900 total.
                                    // That is a lot I think after 100 per channel it is a lot.
                                    // Just in case it gets filled.
                                    // We get this response.
                                    server_response = -2;
                                } else if (server_message
                                        .compareToIgnoreCase("Please wait while we configure your connection.") == 0) {
                                    // Everything is going well.
                                    // After this server will configure and wait our ping.
                                    // And with our pings it will start sending data.
                                    server_response = 1;
                                }
                            } else if (server_message
                                    .compareToIgnoreCase("Sorry this channel is not open right now!") == 0) {
                                // This gets send if channel is not opened yet or errored and closed.
                                // Like producer filename is invalid and channel closed.
                                server_response = -3;
                                if (client.in.hasNextLine()) {
                                    System.out.println(client.in.nextLine());
                                }
                            } else if (server_message.compareToIgnoreCase(
                                    "Server had an error on a channel, try again later! (Probably will be closed!)") == 0) {
                                // This gets send if server had an error. Like the one above. And is in middle
                                // of handling it.
                                server_response = -4;
                            }
                        }

                        if (server_response > 0) {
                            // Here the TCP starts.
                            // This array list is buffer of client.
                            // Client buffer is not big. Because we stream live.
                            // Don't worry we still manage 20 fps per a lot of worker.
                            // And yes you can set the fps max and it will actually be 20 fps.
                            // Big buffer was not needed.
                            ArrayList<String> bufferArray = new ArrayList<String>();
                            // Our buffered reader. We read from this.
                            BufferedReader inComing = new BufferedReader(
                                    new InputStreamReader(client.clientSocket.getInputStream()));
                            // The incoming message is read in to this string.
                            String messageCame = null;
                            // This is example how we ping server.
                            // We obviously can send messages but it doesn't matter.
                            // I had implemented closing server from client etc but it was too unnecessary
                            // so I removed.
                            client.out.println(1);
                            // Streaming height.
                            // First value we get this is needed to process buffer etc.
                            // Nothing else needed.
                            // The frame data is processed by server.
                            // Frame width doesn't matter we display as much.
                            int streamHeight = -1;
                            // This is example to how we read message.
                            while (clientOn) {
                                // Wait for line to be available.
                                // Server always sends lines.
                                // Server always ends messages with \n
                                if ((messageCame = inComing.readLine()) != null) {
                                    // When received set the streamHeight.
                                    streamHeight = Integer.parseInt(messageCame);
                                }
                                if (streamHeight > 0) {
                                    // Break from loop.
                                    // It is not needed to be in a loop but I don't want to change.
                                    break;
                                }
                            }
                            // currentLen is a parameter to determine our buffer length.
                            int currentLen = 0;
                            // Again ping send to server this time server will start streaming to us.
                            client.out.println(1);
                            // Streaming loop.
                            // ClientOn sets false from javafx.
                            // That way we can stop.
                            // We can terminate too server will pick it up and clean socket.
                            bufferArray.add(""); // First line needs this to be aligned.
                            while (clientOn) {
                                // Start time is measured. This is used to syncing fps.
                                // Basically math is like this.
                                // We populate buffer:
                                // We sleep while we populating.
                                // If we lag behind:
                                // (TIMETOSLEEP - EXECUTIONTIME)/BUFFERLENGTH
                                // We dont sleep. Cuz we lagging. Possibly getting lower than 20 fps.
                                // If we have time to sleep we sleep. So data is displayed at 20 fps constant
                                // max.
                                // You can set it at beginning of program to display 30fps etc.
                                long startTime = System.nanoTime();
                                if ((messageCame = inComing.readLine()) == null) {
                                    // If message read is null we print current Buffer.
                                    // This is used to debugging.
                                    // Won't happen hopefully for you.
                                    System.out.println(currentLen);
                                }
                                // Message is added to buffer.
                                bufferArray.add(messageCame);
                                // We add new line after message.
                                // Actually all messages are lines but when we read and add it to array we
                                // remove new line.
                                // So it gets added back.
                                bufferArray.add(System.getProperty("line.separator"));
                                // Buffer len is increased since buffer is populated.
                                currentLen++;
                                // Now this below means buffer needs to be printed out.
                                // We don't use big buffer where we buffer data to couple of frames.
                                // It could be but we stream live anyways and 20fps syncing is done in a smart
                                // way.
                                // This works!
                                if (currentLen == streamHeight) {
                                    // If print to console is set we also print to console.
                                    // You can set it at the beginning.
                                    if (printToConsole) {
                                        // Used mostly for debugging.
                                        System.out.println(bufferArray.toString().replaceAll(",", "")
                                                .replaceAll("\\[", "").replaceAll("\\]", ""));

                                    }
                                    // The javafx label message is updated with in this method.
                                    // Method is hooked from javafx.
                                    // Buffer array return like [ , , ]
                                    // So we remove commas and brackets.
                                    updateMessage(bufferArray.toString().replaceAll(",", "").replaceAll("\\[", "")
                                            .replaceAll("\\]", ""));
                                    // Buffer is reset.
                                    currentLen = 0;
                                    // We ping server to send us new data, new frame.
                                    client.out.println(1);
                                    // Buffer is flushed.
                                    bufferArray.clear();
                                    bufferArray.add(""); // First line needs this to be aligned.
                                }
                                // End time of current operation is calculated.
                                long endTime = System.nanoTime();
                                long duration = (endTime - startTime);
                                // System.out.println((FPS_MAX_DELAY-(duration/1000000))/13);
                                // This is smart way to be live and sync fps.
                                // ((1SEC/FPSTARGET)-EXECTIME)/STREAM_HEIGHT
                                // If we are lagging behind we don't sleep.
                                // Other wise we cap to max fps.
                                Thread.sleep((((FPS_MAX_DELAY - (duration / 1000000)) / streamHeight >= 0)
                                        ? (FPS_MAX_DELAY - (duration / 1000000)) / streamHeight
                                        : 0));
                            }
                            // If we are here means javafx used our method to close us.
                            System.out.println("Closing client.");
                        }
                        // Closing connection.
                        client.stopConnection();
                    } catch (ConnectException a) {
                        // Catching connection issues.
                        System.out.println("Can not connect to the server at address: " + address + " on port: "
                                + String.valueOf(port));
                    } catch (IOException a) {
                        // If we are here means client is stopped.
                        System.out.println("Stopped client.");
                    } catch (NullPointerException e) {
                        System.out.println("NullException");
                    }
                }
                return null;
            }
        };
        // Rest of the code is gui.
        // I used javafx.
        // label is where we display the stream
        // It is hooked to background.
        label.textProperty().bind(dynamicTimeTask.messageProperty());
        // Must important line.
        // Took me hours to figure out.
        // Without setting font like this the stream is crooked.
        label.setFont(Font.font("Monospaced", FontWeight.BOLD, FontPosture.REGULAR, 10));
        // Background thread is created.
        Thread backgroundThread = new Thread(dynamicTimeTask);
        backgroundThread.setName("Background Thread");
        backgroundThread.setDaemon(true);
        // Button for closing client.
        Button closeButton = new Button();
        // Setting text to the button
        closeButton.setText("Close Client");
        // Setting the location of the button
        closeButton.setTranslateX(150);
        closeButton.setTranslateY(200);
        // Button to start connection.
        Button connectButton = new Button();
        // Setting text to the button
        connectButton.setText("Connect");
        // Setting the location of the button
        connectButton.setTranslateX(50);
        connectButton.setTranslateY(200);
        // Adding gui item to layout.
        layout.getChildren().add(label);
        layout.getChildren().add(connectButton);
        layout.getChildren().add(closeButton);
        // Setting theme colors.
        connectButton.setStyle("-fx-background-color:rgba(255,204,225,0.5)");
        closeButton.setStyle("-fx-background-color:rgba(255,204,225,0.5)");
        layout.setStyle("-fx-background-color:rgba(204,241,255,1)");
        // Connection button click event.
        connectButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                try {
                    if (!clientStarted) {
                        // Background Thread gets started
                        backgroundThread.start();
                        // This value is set so we don't start it again and again.
                        clientStarted = true;
                    } else {
                        System.out.println("Connection already started.");
                    }
                } catch (Exception exception) {
                    exception.printStackTrace();
                }
            }
        });
        // Close button click event.
        closeButton.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                try {
                    // First we stop background thread by calling it's method. If it started.
                    if (clientStarted) {
                        client.stopConnection();
                    }
                    // Getting gui scene.
                    Stage stage = (Stage) closeButton.getScene().getWindow();
                    // Closing gui.
                    stage.close();
                } catch (NullPointerException exceptionNullClosedBackThread) {
                    // Getting gui scene.
                    Stage stage = (Stage) closeButton.getScene().getWindow();
                    // Closing gui.
                    stage.close();
                } catch (IOException exceptionIO) {
                    exceptionIO.printStackTrace();
                }
            }
        });
        // Scene of gui is set. 500 width 500 height.
        Scene scene = new Scene(layout, 500, 500);
        // Setting primary stage for javafx.
        primaryStage.setScene(scene);
        // Starting gui.
        primaryStage.show();
    }
}
