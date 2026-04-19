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
# define SELECT_TIMEOUT_ERROR 105 // timeout while reading received data via select() 

// // uart port on Linux 
extern char *uart_interface;

// port's termios attributes 
extern struct termios termios_interface_tty;               

// Port's global file descriptor 
extern int g_fd;    

// helper functions
speed_t get_baudrate(int baud);

// uart
int initialize_uart_interface(int baud, int databits, char parity, int stopbits);
int transmit_message(char *message);
char *receive_message();

