// Linux environment APIs
# include <fcntl.h>
# include <unistd.h>
# include <sys/time.h>

// C Language Headers
# include <ctype.h>
# include <string.h>

// termios API
# include <termios.h>

// API ERROR CODES
# define PORT_INIT_ERROR 103 // Could not open port, Maybe nothing connected yet
# define PORT_ATTR_GET_ERROR 101 // Something went wrong while getting port attributes
# define PORT_ATTR_SET_ERROR 102 // Something went wrong while setting port attributes
# define UART_WRITE_ERROR 104 // write() returned 0
# define READ_TIMEOUT_ERROR 105 // timeout while reading received data i.e. No data available to read 
# define SELECT_ERROR 106 // error occcured while execution of select()
# define READ_ERROR 107  // error occured while execution of read()

// Global Variables
extern char *uart_interface; // uart port device path on Linux  
extern int g_fd; // Port's global file descriptor
extern char buffer[256]; // buffer to store read data
extern int bytes; // Number of characters read to buffer

// UART API Feauture Set
int initialize_uart_interface(int baud, int databits, char parity, int stopbits);
int transmit_message(char *message);
int receive_message_with_timeout(int time_sec);

