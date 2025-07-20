#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <csignal>
#include <termios.h>
#include <unistd.h>

#include "clock.hpp"

using namespace std;

// Box drawing characters
const char SYMBOL_HORIZONTAL[]   = "\u2500"; // ─
const char SYMBOL_VERTICAL[]     = "\u2502"; // │
const char SYMBOL_TOP_LEFT[]     = "\u250C"; // ┌
const char SYMBOL_TOP_RIGHT[]    = "\u2510"; // ┐
const char SYMBOL_BOTTOM_LEFT[]  = "\u2514"; // └
const char SYMBOL_BOTTOM_RIGHT[] = "\u2518"; // ┘
const char SYMBOL_T_LEFT[]       = "\u251C"; // ├
const char SYMBOL_T_RIGHT[]      = "\u2524"; // ┤
const char SYMBOL_T_TOP[]        = "\u252C"; // ┬
const char SYMBOL_T_BOTTOM[]     = "\u2534"; // ┴
const char SYMBOL_INTERSECT[]    = "\u253C"; // ┼


// Double line box drawing characters
const char SYMBOL_DOUBLE_HORIZONTAL[]   = "\u2550"; // ═
const char SYMBOL_DOUBLE_VERTICAL[]     = "\u2551"; // ║
const char SYMBOL_DOUBLE_TOP_LEFT[]     = "\u2554"; // ╔
const char SYMBOL_DOUBLE_TOP_RIGHT[]    = "\u2557"; // ╗
const char SYMBOL_DOUBLE_BOTTOM_LEFT[]  = "\u255A"; // ╚    
const char SYMBOL_DOUBLE_BOTTOM_RIGHT[] = "\u255D"; // ╝
const char SYMBOL_DOUBLE_T_LEFT[]       = "\u2560"; // ╠
const char SYMBOL_DOUBLE_T_RIGHT[]      = "\u2563"; // ╣
const char SYMBOL_DOUBLE_T_TOP[]        = "\u2566"; // ╦
const char SYMBOL_DOUBLE_T_BOTTOM[]     = "\u2569"; // ╩
const char SYMBOL_DOUBLE_INTERSECT[]    = "\u256C"; // ╬

const char BLOCK_FULL[]   = "\u2588"; // █
const char BLOCK_HALF[]   = "\u2592"; // ▒


// ANSI color codes
#define RED     "\033[91m"
#define GREEN   "\033[92m"
#define BLUE    "\033[94m"
#define RESET   "\033[0m"

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

void hide_cursor() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    cout << "\033[?25l"; // hide cursor
}

string current_date() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%a, %d %b %Y", ltm);
    return string(buf);
}

string current_time() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    char buf[9];
    strftime(buf, sizeof(buf), "%H:%M:%S", ltm);
    return string(buf);
}

void print_ascii_clock(const string& time_str, bool blink_colon) {
    string date = current_date();
    const int char_width = 10;
    const int total_width = 2 + time_str.size() * char_width;

    // Top border
    cout << SYMBOL_DOUBLE_TOP_LEFT;
    for (int i = 0; i < total_width; ++i) cout << SYMBOL_DOUBLE_HORIZONTAL;
    cout << SYMBOL_DOUBLE_TOP_RIGHT << "\n";

    // Date row
    cout << SYMBOL_DOUBLE_VERTICAL << "  \033[1m" << date << "\033[0m";
    int padding = total_width - date.length() - 2;
    for (int i = 0; i < padding; ++i) cout << " ";
    cout << SYMBOL_DOUBLE_VERTICAL << "\n";

    // Spacer
    cout << SYMBOL_DOUBLE_VERTICAL;
    for (int i = 0; i < total_width; ++i) cout << " ";
    cout << SYMBOL_DOUBLE_VERTICAL << "\n";

    // ASCII time
    for (int line = 0; line < FONT_HEIGHT; ++line) {
        cout << SYMBOL_DOUBLE_VERTICAL << "  ";
        for (int i = 0; i < time_str.size(); ++i) {
            char c = time_str[i];
            int index;

            if (c == ':') {
                bool show_colon = true;
                if (blink_colon) {
                    time_t now = time(nullptr);
                    show_colon = (now % 2 == 0);
                }

                if (!show_colon) {
                    cout << string(char_width, ' ');
                    continue;
                }

                index = 10; // colon is at FONT[10 * FONT_HEIGHT + line]
            } else if (c >= '0' && c <= '9') {
                index = c - '0';
            }

            if (index != -1) {
                string color = (i < 2) ? RED : (i < 5 ? GREEN : BLUE);
                cout << color << FONT[index * FONT_HEIGHT + line] << RESET;
            } else {
                cout << string(char_width, ' '); // fallback, shouldn't happen
            }
        }
        cout << SYMBOL_DOUBLE_VERTICAL << "\n";
    }

    // Spacer
    cout << SYMBOL_DOUBLE_VERTICAL;
    for (int i = 0; i < total_width; ++i) cout << " ";
    cout << SYMBOL_DOUBLE_VERTICAL << "\n";

    // Bottom border
    cout << SYMBOL_DOUBLE_BOTTOM_LEFT;
    for (int i = 0; i < total_width; ++i) cout << SYMBOL_DOUBLE_HORIZONTAL;
    cout << SYMBOL_DOUBLE_BOTTOM_RIGHT << "\n";
}

int main() {
    signal(SIGINT, sigint_handler);
    atexit(reset_terminal);
    hide_cursor();

    bool blink = true;

    while (!exit_requested) {
        string now = current_time();
        print_ascii_clock(now, blink);
        this_thread::sleep_for(chrono::seconds(1));
        blink = !blink; // Toggle colon each second
        cout << "\033[" << (FONT_HEIGHT + 5) << "A\r"; // move cursor up
    }

    return 0;
}