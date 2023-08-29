

// TODO: tty interface
typedef struct {
    char* contents; // TODO: support colors
    
} tty;

// A video driver or terminal emulator must support rendering the tty, and giving it inputs
// The console driver can choose to buffer inputs