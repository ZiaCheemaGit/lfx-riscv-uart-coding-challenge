// linux environment
# include <fcntl.h>
# include <unistd.h>

// termios API
# include <termio.h>

// ERROR CODES
# define PORT_ATTR_GET_ERROR 101 // Something went wrong while getting port attributes
# define PORT_ATTR_SET_ERROR 102 // Something went wrong while setting port attributes
# define PORT_INIT_ERROR 103 // Could not open port, Maybe nothing connected yet

// uart port on Linux 
# define UART_PORT_PATH "/dev/ttyUSB0"

// port's termios attributes 
struct termios tty_USB0;               

// Port's global file descriptor 
int g_fd;    

// helper functions
static speed_t get_baudrate(int baud);

// uart
static int initialize_uart_interface();
static int set_uart_baud_rate(int baud_rate);

