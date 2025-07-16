#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <csignal>
#include <termios.h>
#include <unistd.h>

using namespace std;

const int FONT_HEIGHT = 8;
const int CHAR_COUNT = 11;
bool exit_requested = false;

vector<string> FONT = {
    " ██████\\  ",
    "███ __██\\ ",
    "████\\ ██ |",
    "██\\██\\██ |",
    "██ \\████ |",
    "██ |\\███ |",
    "\\██████  /",
    " \\______/ ",

    "   ██\\    ",
    " ████ |   ",
    " \\_██ |   ",
    "   ██ |   ",
    "   ██ |   ",
    "   ██ |   ",
    " ██████\\  ",
    " \\______| ",

    " ██████\\  ",
    "██  __██\\ ",
    "\\__/  ██ |",
    " ██████  |",
    "██  ____/ ",
    "██ |      ",
    "████████\\ ",
    "\\________|",

    " ██████\\  ",
    "██ ___██\\ ",
    "\\_/   ██ |",
    "  █████ / ",
    "  \\___██\\ ",
    "██\\   ██ |",
    "\\██████  |",
    " \\______/ ",

    "██\\   ██\\ ",
    "██ |  ██ |",
    "██ |  ██ |",
    "████████ |",
    "\\_____██ |",
    "      ██ |",
    "      ██ |",
    "      \\__|",

    "███████\\  ",
    "██  ____| ",
    "██ |      ",
    "███████\\  ",
    "\\_____██\\ ",
    "██\\   ██ |",
    "\\██████  |",
    " \\______/ ",

    " ██████\\  ",
    "██  __██\\ ",
    "██ /  \\__|",
    "███████\\  ",
    "██  __██\\ ",
    "██ /  ██ |",
    " ██████  |",
    " \\______/ ",

    "████████\\ ",
    "\\____██  |",
    "    ██  / ",
    "   ██  /  ",
    "  ██  /   ",
    " ██  /    ",
    "██  /     ",
    "\\__/      ",

    " ██████\\  ",
    "██  __██\\ ",
    "██ /  ██ |",
    " ██████  |",
    "██  __██< ",
    "██ /  ██ |",
    "\\██████  |",
    " \\______/ ",

    " ██████\\  ",
    "██  __██\\ ",
    "██ /  ██ |",
    "\\███████ |",
    " \\____██ |",
    "██\\   ██ |",
    "\\██████  |",
    " \\______/ ",

    "          ",
    "          ",
    "  ██\\     ",
    "  \\__|    ",
    "          ",
    "  ██\\     ",
    "  \\__|    ",
    "          "
};

struct termios orig_termios;

void reset_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    cout << "\033[?25h"; // show cursor
}

void sigint_handler(int) {
    exit_requested = true;
}

void disable_input_echo_and_cursor() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    cout << "\033[?25l"; // hide cursor
}

void print_ascii_clock(const string& time_str) {
    for (int line = 0; line < FONT_HEIGHT; ++line) {
        cout << "  ";
        for (char c : time_str) {
            int index = (c == ':' ? 10 : c - '0');
            if (index < 0 || index >= CHAR_COUNT) continue;
            cout << FONT[index * FONT_HEIGHT + line];
        }
        cout << '\n';
    }
}

string current_time() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    char buf[9];
    strftime(buf, sizeof(buf), "%H:%M:%S", ltm); // change to "%I:%M:%S" for 12-hour
    return string(buf);
}

int main() {
    signal(SIGINT, sigint_handler);
    atexit(reset_terminal);
    disable_input_echo_and_cursor();

    const float REFRESH_INTERVAL = 0.2f;

    while (!exit_requested) {
        string now = current_time();
        print_ascii_clock(now);
        this_thread::sleep_for(chrono::duration<float>(REFRESH_INTERVAL));
        cout << "\033[" << FONT_HEIGHT << "A\r"; // move up to redraw
    }

    return 0;
}
