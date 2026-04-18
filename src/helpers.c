# include <helpers.h>

// select BUAD RATE corresponding to 
// options in termios
// Default 115200 otherwise
static speed_t get_baudrate(int baud) {
    switch(baud) {
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        default: return B115200;
    }
}

// initialize UART Port and get file descriptor
static int initialize_uart_interface() {
    g_fd = open(UART_PORT_PATH, O_RDWR | O_NONBLOCK);

    if(g_fd < 0) {
        return PORT_INIT_ERROR;
    }

    if(tcgetattr(g_fd, &tty_USB0)) {
        return PORT_ATTR_GET_ERROR;
    }

    cfmakeraw(&tty_USB0);

    if(tcsetattr(g_fd, TCSANOW, &tty_USB0)) {
        return PORT_ATTR_SET_ERROR;
    }
    
    return 0;
    return 0;
}

// configure UART BAUD RATE
static int set_uart_baud_rate(int baud_rate) {
    speed_t baud_rate_sel = get_baudrate(baud_rate);
    cfsetispeed(&tty_USB0, baud_rate_sel);
    cfsetospeed(&tty_USB0, baud_rate_sel);
}

