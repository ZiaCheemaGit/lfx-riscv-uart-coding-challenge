# include <helpers.h>

// Implementations
char *uart_interface;
struct termios termios_interface_tty;               
int g_fd;  

// select BUAD RATE corresponding to 
// options in termios
// Default 115200 otherwise
speed_t get_baudrate(int baud) {
    switch(baud) {
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        default: return B115200;
    }
}

// initialize UART Port with provided configurations
// and get file descriptor for the initialized process
int initialize_uart_interface(int baud, int databits, char parity, int stopbits) {
    // Initialize 
    g_fd = open(uart_interface, O_RDWR | O_NOCTTY | O_NONBLOCK);

    if(g_fd < 0) {
        return PORT_INIT_ERROR;
    }

    if(tcgetattr(g_fd, &termios_interface_tty)) {
        return PORT_ATTR_GET_ERROR;
    }

    // set uart BAUD RATE 
    speed_t baud_rate = get_baudrate(baud);
    cfsetispeed(&termios_interface_tty, baud_rate); 
    cfsetospeed(&termios_interface_tty, baud_rate); 

    // set number of databits in payload
    termios_interface_tty.c_cflag &= ~CSIZE;
    switch (databits) {
        case 5:
            termios_interface_tty.c_cflag |= CS5;
            break;
        case 6:
            termios_interface_tty.c_cflag |= CS6;
            break;
        case 7:
            termios_interface_tty.c_cflag |= CS7;
            break;
        case 8:
            termios_interface_tty.c_cflag |= CS8;
            break;
        default:
            return PORT_ATTR_SET_ERROR;
    }

    // set parity type
    parity = toupper(parity);
    switch (parity) {
        case 'N':   // No parity
            termios_interface_tty.c_cflag &= ~PARENB;
            break;

        case 'E':   // Even parity
            termios_interface_tty.c_cflag |= PARENB;
            termios_interface_tty.c_cflag &= ~PARODD;
            break;

        case 'O':   // Odd parity
            termios_interface_tty.c_cflag |= PARENB;
            termios_interface_tty.c_cflag |= PARODD;
            break;

        default:
            return PORT_ATTR_SET_ERROR;
    }

    // set stopbits
    switch (stopbits) {
        case 1:
            termios_interface_tty.c_cflag &= ~CSTOPB;
            break;

        case 2:
            termios_interface_tty.c_cflag |= CSTOPB;
            break;

        default:
            return PORT_ATTR_SET_ERROR;
    }

    // Enable Receiver
    termios_interface_tty.c_cflag |= (CLOCAL | CREAD);

    // Apply all these configurations
    if(tcsetattr(g_fd, TCSANOW, &termios_interface_tty)) {
        return PORT_ATTR_SET_ERROR;
    }
    
    return 0;
}

// write message data to 
// selected tty* uart interface
int transmit_message(char *message) {
    if(write(g_fd, message, strlen(message))) {
        return 0;
    }
    return UART_WRITE_ERROR;
}

// Read data using time out
int receive_message_with_timeout(int timeout_sec) {
    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(g_fd, &readfds);

    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;

    int ret = select(g_fd + 1, &readfds, NULL, NULL, &timeout);
    if (ret == 0) { return SELECT_TIMEOUT_ERROR; }

    char buffer[256];

    int bytes = read(g_fd, buffer, sizeof(buffer));

    if (bytes > 0)
        write(STDOUT_FILENO, buffer, bytes);
}
