#include <panel.h>
#include <string.h>

//GLOBAL CONFIGURATION VARIABLES:

#define NUM_ROWS 5 //number of rows of items
#define NUM_COLS 5
#define NUM_WINS (NUM_ROWS * NUM_COLS)

const int win_height = 9; //number of lines for each window, including border
const int win_width = 19; //number of columns for each window, including border

const int y_offset = 1; //vertical spacing between windows and between top of screen and first window
const int x_offset = 2; //horizontal spacing between windows and between left side of screen and first window

const char* wordlist_file = ".tmplist"; //file path for wordlist
const char* checkboxfalse = "[ ]"; //unchecked checkbox
const char* checkboxtrue = "[X]"; //checked checkbox
const char* numbox = "[%c]"; //number box format

const int blurbsize = 255; //maximum length of the description for each space

//BOARD TYPEDEF

typedef struct {
    FILE* wordlist;
    WINDOW* windows[NUM_WINS];
    PANEL* panels[NUM_WINS];
    bool states[NUM_WINS];
} board;

//FUNCTION DEFINITIONS

void getinput(board gameboard); //get a character of input and toggle the appropriate state, then call checkbingo
bool checkbingo(board gameboard); //check if the user has a bingo
bool checkrows(board gameboard); //subset of checkbingo
bool checkcols(board gameboard);
bool checkdiags(board gameboard);
void refreshscreen(); //redraw the screen
void draw_x(board b, int wi);
void printblurb(WINDOW* w, FILE* file);

int main() {

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    board gameboard; //main structure to be passed around functions
    gameboard.wordlist = fopen(wordlist_file, "r"); //open the wordlist and assign it to the board

    //INITIALIZATION:
    
    int current_x = x_offset;
    int current_y = y_offset;
    int i,j,k;
    
    for (j = 0; j < NUM_ROWS; j++) {
        for (k = 0; k < NUM_COLS; k++) {
            i = j * NUM_COLS + k; //overall index in list for row j col k
            WINDOW* w = newwin(win_height, win_width, current_y, current_x);

            mvwprintw(w, win_height - 2, win_width - strlen(checkboxfalse) -1, checkboxfalse);

            char numboxtemp[5];
            sprintf(numboxtemp, numbox, i + 'A');
            mvwprintw(w, 1, 1, numboxtemp);

            gameboard.windows[i] = w;
            gameboard.panels[i] = new_panel(w);
            gameboard.states[i] = false;

            /*char* line[blurbsize];*/
            /*fgets(line, blurbsize, gameboard.wordlist);*/

            /*mvwprintw(w, 1, 1, "%s", line);*/

            printblurb(w, gameboard.wordlist);

            box(w,0,0);

            current_x += x_offset + win_width;
        }
        current_x = x_offset;
        current_y += y_offset + win_height;
    }

    update_panels();

    doupdate();

    getinput(gameboard);

    getch();

    endwin();

    printf("Goodbye!\n");

    return 0;

}

void refreshscreen() {
    update_panels();
    doupdate();
}

void getinput(board b) {
    int ch;
    int index;
    
    bool haswon = false;

    while (!haswon) {

        ch = getch();
        if (ch >= 97 && ch < 97 + NUM_WINS) {
            index = ch - 97;
        } else if (ch >= 65 && ch < 65 + NUM_WINS) {
            index = ch - 65;
        } else if (ch == 27) {
            break;
        } else {
            continue;
        }


        b.states[index] = !(b.states[index]);

        haswon = checkbingo(b);

        if (b.states[index]) {
            mvwprintw(b.windows[index], win_height - 2, win_width - strlen(checkboxtrue) - 1, checkboxtrue);
        } else {
            mvwprintw(b.windows[index], win_height - 2, win_width - strlen(checkboxfalse) - 1, checkboxfalse);
        }

        refreshscreen();

    }

    if (haswon) {
        printw("Congratulations! You've won!");
    }

}

bool checkbingo(board b) {
    bool r = checkrows(b);
    bool c = checkcols(b);
    bool d = checkdiags(b);
    /*if (r) {
        printw("won on a row");
    }
    if (c) {
        printw("won on a col");
    }
    if (d) {
        printw("won on a diag");
    }*/
    return (r || c || d);
}

bool checkrows(board b) {
    int i;
    for (i = 0; i < NUM_ROWS; i++) {
        int j = i * NUM_COLS;

        bool rowval = true;

        for (int k = 0; k < NUM_COLS; k++) {
            if (!b.states[j+k]) {
                rowval = false;
            }
        }

        if (rowval) {
            for (int k = 0; k < NUM_COLS; k++) {
                draw_x(b, j+k);
            }
            return true;
        }

    }
    return false;
}

bool checkcols(board b) {
    int i,j;
    for (i = 0; i < NUM_COLS; i++) {
        bool colval = true;
        for (j = 0; j < NUM_ROWS; j++) {
            if (!b.states[i + (j * NUM_ROWS)]) {
                colval = false;
            }
        }
        if (colval) {
            for (j = 0; j < NUM_ROWS; j++) {
                draw_x(b, i + (j * NUM_ROWS));
            }
            return true;
        }
    }
    return false;
}

bool checkdiags(board b) {
    int i;
    bool diagval1 = true;
    bool diagval2 = true;
    for (i = 0; i < NUM_COLS; i++) {
        if (!b.states[i + (i * NUM_ROWS)]) {
            diagval1 = false;
        }
        if (!b.states[(i + 1) * (NUM_ROWS - 1)]) {
            diagval2 = false;
        }
    }
    if (diagval1) {
        for (i = 0; i < NUM_COLS; i++) {
            draw_x(b, i + (i * NUM_ROWS));
        }
        return true;
    }
    if (diagval2) {
        for (i = 0; i < NUM_COLS; i++) {
            draw_x(b, (i + 1) * (NUM_ROWS - 1));
        }
        return true;
    }
    return false;
} 

void draw_x(board b, int wi) {
    WINDOW* w = b.windows[wi];
    int x_center = win_width / 2;
    int y_center = win_height / 2;
    unsigned char block[2] = {65, 0};
    /*sprintf(block, "%s", 219);*/
    for (int i = -2; i <= 2; i++) {
        /*mvwprintw(w, y_center + i, x_center + i, "%s", block);*/
        /*mvwprintw(w, y_center + i, x_center - i, "%s", block);*/
        mvwaddch(w, y_center + i, x_center + i, ACS_CKBOARD);
        mvwaddch(w, y_center + i, x_center + i - 1, ACS_CKBOARD);
        mvwaddch(w, y_center + i, x_center + i + 1, ACS_CKBOARD);
        mvwaddch(w, y_center + i, x_center - i, ACS_CKBOARD);
        mvwaddch(w, y_center + i, x_center - i + 1, ACS_CKBOARD);
        mvwaddch(w, y_center + i, x_center - i - 1, ACS_CKBOARD);
    }
}

void printblurb(WINDOW* w, FILE* f) {
    char blurb[blurbsize]; //this is the full line, unwrapped
    fgets(blurb, blurbsize, f); //read in the line from the file
    char* p = blurb; //pointer to represent where we are in the blurb file
    char* lastspace = blurb; //where was the last space in the blurb file?
    char* temp = blurb; //this one is used for writing the blurb file to the line to be written
    char line[win_width - 1]; //this is the line that gets written to the screen, it is reset each time it wraps
    int l = 0; //the length of the current line
    int i; //iterator
    int linenr = 2; //what line to print output to
    while (*p != '\0' && linenr < win_height - 2) { //make sure the string hasn't ended
        if (*p == ' ') lastspace = p;  //if we encounter a space, mark it for later.
        if (++l >= win_width - 2) { //add 1 to length and if it is larger than acceptable, copy blurb up to lastspace into line, and then print line to the screen.
            i = 0;
            if (*temp == ' ') temp++;
            while (temp < lastspace) {
                line[i] = *temp; //copy blurb over to line
                i++; //and increment both values
                temp++;
            }
            line[i] = '\0';
            mvwaddch(w, 0, 1, blurb[0]);
            mvwprintw(w, linenr++, (win_width - strlen(line)) / 2, line); //now that line is copied in, print it to the screen
            l = 0; //and reset the length to zero
            p = lastspace; //pick up where we left off
        }
        p++; //move on to the next character and keep going
    }
    i = 0;
    if (*temp == ' ') temp++;
    while (temp < p) {
        line[i] = *temp;
        i++;
        temp++;
    }
    line[i] = '\0';
    mvwaddch(w, 0, 1, blurb[0]);
    mvwprintw(w, linenr++, (win_width - strlen(line)) / 2, line);
}
