#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

int load_content(char buffer[BUFFER_SIZE][BUFFER_SIZE]);
void print_matching(char buffer[BUFFER_SIZE][BUFFER_SIZE], int buflen,
                    char *word);
void put_input(char *word);

int rows = 0, cols = 0;

int main()
{

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  raw();

  clear();

  char buffer[BUFFER_SIZE][BUFFER_SIZE] = {0};
  int bufcount = load_content(buffer);

  char c;
  char word[BUFFER_SIZE] = {0};
  int word_counter = 0;
  while ((c = getch()) != 'q')
  {
    if (c == '1')
    {

      getmaxyx(stdscr, rows, cols);

      word_counter = 0;
      word[word_counter] = '\0';

      clear();
      print_matching(buffer, bufcount, word);

      put_input(word);

      continue;
    }
    if (c == '\b' || c == 127 || c == KEY_BACKSPACE)
    {
      word_counter--;
      word[word_counter] = '\0';

      clear();
      print_matching(buffer, bufcount, word);

      put_input(word);
    }
    if (word_counter >= 127)
    {
      printf("word buffer is full\n");
      continue;
    }

    clear();
    word[word_counter++] = c;
    word[word_counter] = '\0';
    print_matching(buffer, bufcount, word);
    put_input(word);
  }

  clear();
  endwin();

  return 0;
}

int load_content(char buffer[BUFFER_SIZE][BUFFER_SIZE])
{
  FILE *ls = popen("ls ~/", "r");

  int count = 0;
  char temp[BUFFER_SIZE];
  while (fgets(temp, sizeof(temp), ls) != NULL)
  {
    temp[sizeof(temp) - 1] = '\0';
    *strchr(temp, '\n') = '\0';
    strncpy(buffer[count], temp, sizeof(buffer[count]) - 1);
    count++;
  }

  pclose(ls);

  return count;
}

void print_matching(char buffer[BUFFER_SIZE][BUFFER_SIZE], int buflen,
                    char *word)
{
  int matches = 0;

  move(0, 0);

  for (int i = 0; i < buflen; i++)
  {
    if (strstr(buffer[i], word) != NULL)
    {
      matches++;
      printw("%s\n", buffer[i]);
    }
  }

  if (matches == 0)
  {
    getmaxyx(stdscr, rows, cols);
    mvprintw(rows - 2, 0, "[NO OUTPUT]\n");
  }
}

void put_input(char *word)
{
  getmaxyx(stdscr, rows, cols);

  move(rows - 1, 0);
  clrtoeol();

  mvprintw(rows - 1, 0, "> %s", word);
}
