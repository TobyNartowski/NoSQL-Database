#include "menu.hpp"
#include "windows.hpp"

//Implementacja metod
void Menu::printMenuChoices(WINDOW *window, Choices::choices_t whichMenu,
                            unsigned int highlight, std::string databaseName)
{
    int x, y;
    getmaxyx(window, y, x);

    x = x/2 - 6;
    y = y/2 - ((Choices::choices[whichMenu]).size()/2);

    Windows::drawBasicWindow(databaseName, Choices::names[whichMenu]);
    for(unsigned int i = 0; i < Choices::choices[whichMenu].size(); i++){
        if(highlight == i){
            wattron(window, A_REVERSE);
            mvwprintw(window, y, x, "%s", Choices::choices[whichMenu][i].c_str());
            wattroff(window, A_REVERSE);
        }
        else{
            mvwprintw(window, y, x, "%s", Choices::choices[whichMenu][i].c_str());
        }
        y++;
    }
    wrefresh(window);
}

void Menu::printMenuChoices(WINDOW *window, const std::string *menuChoices, unsigned int size,
                            std::string windowName, std::string databaseName, unsigned int highlight)
{
    int x, y;
    getmaxyx(window, y, x);

    x = x/2 - 6;
    y = y/2 - (size/2);

    Windows::drawBasicWindow(databaseName, windowName);
    for(unsigned int i = 0; i < size; i++){
        if(highlight == i){
            wattron(window, A_REVERSE);
            mvwprintw(window, y, x, "%s", menuChoices[i].c_str());
            wattroff(window, A_REVERSE);
        }
        else{
            mvwprintw(window, y, x, "%s", menuChoices[i].c_str());
        }
        y++;
    }
    wrefresh(window);
}

int Menu::drawMenu(WINDOW *window, Choices::choices_t whichMenu, std::string databaseName)
{
    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    box(window, 0, 0);

    refresh();
    printMenuChoices(window, whichMenu, highlight, databaseName);

    while(true){
        buffer = wgetch(window);
        switch(buffer){
            case KEY_LEFT:
                return -1;
            case KEY_UP:
                if(highlight == 0)
                    highlight = Choices::choices[whichMenu].size()-1;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == Choices::choices[whichMenu].size()-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case KEY_RIGHT:
            case 10:
                choice = highlight;
                break;

            default:
                break;
        }
        printMenuChoices(window, whichMenu, highlight, databaseName);

        if(choice != -1)
            break;
    }
    return choice;
}

int Menu::drawMenu(WINDOW *window, const std::string menuChoices[],
                   unsigned int size, std::string windowName, std::string databaseName)
{

    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    box(window, 0, 0);

    refresh();
    printMenuChoices(window, menuChoices, size, windowName, databaseName, highlight);

    while(true){
        buffer = wgetch(window);
        switch(buffer){
            case KEY_LEFT:
                return -1;
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight == size-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case KEY_RIGHT:
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }
        printMenuChoices(window, menuChoices, size, windowName, databaseName, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

void Menu::printHorizontalMenuChoices(WINDOW *window, const std::string *menuChoices,
                                         unsigned int size, unsigned int shift,
                                         unsigned int highlight)
{
    int totalLength = 0;
    int x = getmaxx(window);

    for(unsigned int i = 0; i < size; i++)
        totalLength += menuChoices[i].length();

    for(unsigned int i = 0, horizontal = 0; i < size; i++){
        if(highlight == i){
            wattron(window, A_REVERSE);
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(window, A_REVERSE);
        }
        else{
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
        }
        horizontal += menuChoices[i].length() + 1;

    }
    wrefresh(window);
}

int Menu::drawHorizontalMenu(WINDOW *window, const std::string *menuChoices,
                                unsigned int size, unsigned int shift)
{
    unsigned int highlight = 0;
    int choice = -1;
    int buffer;

    printHorizontalMenuChoices(window, menuChoices, size, shift, highlight);

    while(true){
        buffer = wgetch(window);
        switch(buffer){
            case KEY_LEFT:
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if(highlight == size-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }
        printHorizontalMenuChoices(window, menuChoices, size, shift, highlight);

        if(choice != -1)
            break;
    }
    return choice;
}

void Menu::printMenuSelectable(WINDOW *window, const std::string *menuChoices,
                        unsigned int size, unsigned int shift,
                        unsigned int highlight, bool *selected)
{
    int totalLength = 0;
    int x = getmaxx(window);

    for(unsigned int i = 0; i < size; i++)
        totalLength += menuChoices[i].length();

    for(unsigned int i = 0, horizontal = 0; i < size; i++){
        if(highlight == i){
            wattron(window, A_REVERSE);
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(window, A_REVERSE);
        }
        else if(selected[i]){
            wattron(window, A_REVERSE);
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
            wattroff(window, A_REVERSE);
        }
        else{
            mvwprintw(window, shift, (x/2-(totalLength/2)+horizontal), "%s", menuChoices[i].c_str());
        }
        horizontal += menuChoices[i].length() + 1;

    }
    wrefresh(window);
}

bool *Menu::drawMenuSelectable(WINDOW *window, const std::string *menuChoices,
                               unsigned int size, unsigned int shift)
{
    unsigned int highlight = 0;
    int endSelecting = false;
    int buffer;
    bool *selected = new bool[size];

    for(unsigned int i = 0; i < size; i++)
        selected[i] = false;

    printMenuSelectable(window, menuChoices, size, shift, highlight, selected);

    while(true){
        buffer = wgetch(window);
        switch(buffer){
            case KEY_LEFT:
            case KEY_UP:
                if(highlight == 0)
                    highlight = size-1;
                else
                    highlight--;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if(highlight == size-1)
                    highlight = 0;
                else
                    highlight++;
                break;
            case ' ':
                selected[highlight] = !selected[highlight];
                break;
            case 10:
                endSelecting = true;
                break;
            default:
                break;
        }
        printMenuSelectable(window, menuChoices, size, shift, highlight, selected);

        if(endSelecting)
            break;
    }
    return selected;
}
