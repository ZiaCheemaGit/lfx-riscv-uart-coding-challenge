// Custom helper API
# include <helpers.h>

// standard APIs
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>

// End the Interface Process 
// if Ctrl+C or Ctrl+Z is pressed
void handle_exit(int sig) {
    printf("\nInterrupted by Signal: %d. Closing UART and Exiting Program\n\n", sig);
    if (g_fd >= 0) {
        int error_code = close(g_fd);
        if(error_code){
            printf("Unable to Terminate Proccess: %i\nError: %i\n", g_fd, error_code);
        }
    }
    exit(0);
}

// Read by timeout Mechanisim and then  
// print read data to terminal
void print_read_data(int timeout) {
    int timeout_seconds = timeout;
    receive_message_with_timeout(timeout_seconds);
    printf("Data Read: ");
    for (int i = 0; i < bytes; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    // Program Usage
    if(argc != 7) {
        printf("\nUsage: %s <uart_interface> <baudrate> <databits> <parity> <stopbits> <timeout>\n\n", argv[0]);
        printf("uart_interface: Uart Port path on Linux File System(E.g. /dev/ttyUSB0)\n");
        printf("baudrate: UART Baud Rate for both TX and RX (9600, 115200, e.t.c.)\n");
        printf("databits: Number of databits in payload(5 - 8)\n");
        printf("parity: Parity Type(N, E, O) - No parity, Even parity, Odd parity\n");
        printf("stopbits: Number of Stop Bits (1 - 2)\n");
        printf("timeout: Number of seconds to wait before printing received data\n\n");
        return -1;
    }


    signal(SIGINT, handle_exit);  // catch Ctrl+C
    signal(SIGTSTP, handle_exit); // catch Ctrl+Z

    // UART Configurations
    uart_interface = argv[1]; // Uart Port path on Linux File System(E.g. /dev/ttyUSB0)
    int baud = atoi(argv[2]); // UART Baud Rate for both TX and RX (E.g. 9600, 115200, e.t.c.)
    int databits = atoi(argv[3]); // Number of databits in payload (E.g. 6, 7 - Min: 5 && Max: 8)
    char parity = argv[4][0]; // Parity Type(E.g. N, E, O) - No parity, Even parity, Odd parity
    int stopbits = atoi(argv[5]); // Number of Stop Bits 1 0r 2
    int timeout = atoi(argv[6]); // Number of seconds to wait before printing received data


    // Initialize & Configure UART Interface
    int error_code = initialize_uart_interface(baud, databits, parity, stopbits);
    if(error_code) {
        printf("Unable to Initialize: %s\nError: %i\n", uart_interface, error_code);
        return error_code;
    }

    
    // Transmit test message
    char *test_message = "UART test message\n";
    error_code = transmit_message(test_message);
    if (error_code) {
        close(g_fd);
        return error_code;
    }
    tcdrain(g_fd);
    printf("Test message transmitted successfully\n");


    // Non-blocking read
    while(1) {
        print_read_data(timeout);
    }    


    return 0;
}

