#ifndef GRAMUI_H
#define GRAMUI_H

#define whiteFont_redBck  BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|BACKGROUND_INTENSITY|FOREGROUND_INTENSITY  //�Ա�־λ����������óɺ��ְ׵�
#define whiteFont FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY  //�Ա�־λ����������óɰ��ֺ��

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<conio.h>
#include <iostream>
#include"Grammer.h"
#include"GramIO.h"
#include"GramMatch.h"
#include<string>
#include<vector>

using namespace std;
// δ���
class MenuItem
{
public:
    MenuItem(string title,void*(*fun)(...));
    string getTitle();
};


class GramUI
{
public:
    GramUI();
    int UIMain();

private:
    int Menu();
    int MenuCall();
    void show();
    void help();

    bool issucesskey(char buffer);

    static void gotoxy(int y,int x);
    static void Hidecursor();
    static void Showcursor();

    Gram _gram;
    vector<string> menu_str;
    unsigned choose;
};

#endif // GRAMUI_H
