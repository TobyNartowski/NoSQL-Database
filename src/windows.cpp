#include "windows.hpp"

#include <cstring>

WINDOW *Windows::mainWindow = nullptr;
WINDOW *Windows::infoWindow = nullptr;
WINDOW *Windows::errorWindow = nullptr;
WINDOW *Windows::choiceWindow = nullptr;

//Implementacje metod
void Windows::drawBasicWindow(std::string databaseName, std::string windowName)
{
    wclear(mainWindow);
    box(mainWindow, 0, 0);
    mvwprintw(mainWindow, 0, 3, "%s", windowName.c_str());
    mvwprintw(mainWindow, 0, getmaxx(mainWindow)-3-databaseName.length(),
              "%s", databaseName.c_str());
}

void Windows::drawInfoWindow()
{
    wclear(infoWindow);
    box(infoWindow, 0, 0);
    refresh();
    wrefresh(infoWindow);
}

void Windows::drawErrorWindow(std::string errorMessage)
{
    std::string enterText = "(ENTER, aby kontynuowac)";
    wclear(errorWindow);
    box(errorWindow, 0, 0);

    if(errorMessage.length() >= (unsigned)getmaxx(errorWindow)-2){
        std::string firstHalf, secondHalf;
        unsigned int cut = errorMessage.length()/2;
        while(errorMessage[cut] != ' ')
            cut++;

        firstHalf = errorMessage.substr(0, cut);
        secondHalf = errorMessage.substr(cut);
        mvwprintw(errorWindow, getmaxy(errorWindow)/2-1,
                 getmaxx(errorWindow)/2-(firstHalf.length()/2), firstHalf.c_str());
        mvwprintw(errorWindow, getmaxy(errorWindow)/2,
                 getmaxx(errorWindow)/2-(secondHalf.length()/2), secondHalf.c_str());
    }
    else{
        mvwprintw(errorWindow, getmaxy(errorWindow)/2,
                 getmaxx(errorWindow)/2-(errorMessage.length()/2), errorMessage.c_str());
    }

    mvwprintw(errorWindow, getmaxy(errorWindow)-2,
            getmaxx(errorWindow)/2-(enterText.length()/2), enterText.c_str());

    wrefresh(errorWindow);

    while(true){
        int getchBuffer = getch();
        if((getchBuffer == 10) || (getchBuffer == KEY_LEFT))
            break;
    }
    wclear(errorWindow);
}

bool Windows::drawChoiceWindow(std::string choiceQuestion)
{
    const std::string choices[2] = {"TAK", "NIE"};
    unsigned int highlight = 0;
    int choice = -1;
    int totalLength = 0;
    int buffer;

    wclear(choiceWindow);
    box(choiceWindow, 0, 0);

    if(choiceQuestion.length() >= (unsigned)getmaxx(choiceWindow)-2){
        std::string firstHalf, secondHalf;
        unsigned int cut = choiceQuestion.length()/2;
        while(choiceQuestion[cut] != ' ')
            cut++;

        firstHalf = choiceQuestion.substr(0, cut);
        secondHalf = choiceQuestion.substr(cut);
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3,
                     getmaxx(choiceWindow)/2-(firstHalf.length()/2), firstHalf.c_str());
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3+1,
                     getmaxx(choiceWindow)/2-(secondHalf.length()/2), secondHalf.c_str());
    }
    else{
        mvwprintw(choiceWindow, getmaxy(choiceWindow)/3+1,
                     getmaxx(choiceWindow)/2-(choiceQuestion.length()/2), choiceQuestion.c_str());
    }

    wrefresh(choiceWindow);

    int x = getmaxx(choiceWindow);

    for(unsigned int i = 0; i < 2; i++)
        totalLength += choices[i].length();

    for(unsigned int i = 0, horizontal = 0; i < 2; i++){
        if(highlight == i){
            wattron(choiceWindow, A_REVERSE);
            mvwprintw(choiceWindow, 2*getmaxy(choiceWindow)/3-1, (x/2-(totalLength/2)+horizontal), "%s", choices[i].c_str());
            wattroff(choiceWindow, A_REVERSE);
        }
        else{
            mvwprintw(choiceWindow, 2*getmaxy(choiceWindow)/3-1, (x/2-(totalLength/2)+horizontal), "%s", choices[i].c_str());
        }
        horizontal += choices[i].length() + 1;

    }
    wrefresh(choiceWindow);
    while(true){
        buffer = wgetch(choiceWindow);
        switch(buffer){
            case KEY_LEFT:
            case KEY_UP:
                if(highlight == 0)
                    highlight = 1;
                else
                    highlight--;
                break;
            case KEY_RIGHT:
            case KEY_DOWN:
                if(highlight == 1)
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
        int totalLength = 0;
        int x = getmaxx(choiceWindow);

        for(unsigned int i = 0; i < 2; i++)
            totalLength += choices[i].length();

        for(unsigned int i = 0, horizontal = 0; i < 2; i++){
            if(highlight == i){
                wattron(choiceWindow, A_REVERSE);
                mvwprintw(choiceWindow, 2*getmaxy(choiceWindow)/3-1, (x/2-(totalLength/2)+horizontal), "%s", choices[i].c_str());
                wattroff(choiceWindow, A_REVERSE);
            }
            else{
                mvwprintw(choiceWindow, 2*getmaxy(choiceWindow)/3-1, (x/2-(totalLength/2)+horizontal), "%s", choices[i].c_str());
            }
            horizontal += choices[i].length() + 1;

        }
        wrefresh(choiceWindow);
        if(choice != -1)
            break;
    }

    wclear(choiceWindow);

    if(!choice)
        return true;
    else
        return false;
}

void Windows::printInfo(std::string info)
{
    drawInfoWindow();
    mvwprintw(infoWindow, 1, 2, "%s", info.c_str());
    wrefresh(infoWindow);
}

std::string Windows::drawInputWindow(WINDOW *window, std::string name,
                                     unsigned int shift, std::string defaultText)
{
    char buffer[32];
    std::string stringBuffer;
    const int maxx = getmaxx(window);

    mvwprintw(window, shift, (maxx-strlen(name.c_str()))/2+1, "%s", name.c_str());
    mvwprintw(window, shift+1, (maxx/2+17), ":");
    mvwprintw(window, shift+1, (maxx/2-17), "                                  ");
    mvwprintw(window, shift+1, (maxx/2-16), ":");
    wrefresh(window);
    if(defaultText != ""){
        mvwprintw(window, shift+1, (maxx/2-15), defaultText.c_str());
        return defaultText;
    }

    echo();
    curs_set(1);

    wgetnstr(window, buffer, 32);

    noecho();
    curs_set(0);

    stringBuffer = buffer;
    return stringBuffer;
}
