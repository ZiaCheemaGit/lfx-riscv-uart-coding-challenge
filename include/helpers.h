// linux environment
# include <fcntl.h>
# include <unistd.h>
# include <ctype.h>
# include <sys/time.h>
# include <string.h>

// termios API
# include <termios.h>

// ERROR CODES
# define PORT_INIT_ERROR 103 // Could not open port, Maybe nothing connected yet
# define PORT_ATTR_GET_ERROR 101 // Something went wrong while getting port attributes
# define PORT_ATTR_SET_ERROR 102 // Something went wrong while setting port attributes
# define UART_WRITE_ERROR 104 // write() returned 0
# define READ_TIMEOUT_ERROR 105 // timeout while reading received data i.e. No data available to read 
# define SELECT_ERROR 106 // error occcured while execution of select()
# define READ_ERROR 107  // error occured while execution of read()

extern char *uart_interface; // uart port on Linux  
extern int g_fd; // Port's global file descriptor
extern char buffer[256]; // buffer to store read data
extern int bytes; // characters read to buffer

// helper functions
speed_t get_baudrate(int baud);

// uart
int initialize_uart_interface(int baud, int databits, char parity, int stopbits);
int transmit_message(char *message);
int receive_message_with_timeout(int time_sec);

