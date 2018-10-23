#include "GramUI.h"

GramUI::GramUI()
{
    choose = 1;
    menu_str =
    {
        "1.�ӿ���̨�����ķ�",
        "2.���ļ������ķ�",
        "3.��ʾ��ǰ�ķ�����",
        "4.��ʾ��ǰ�ķ�����",
        "5.�Զ�����-���ط�ƥ���ַ���",
        "6.�Զ�����-Ԥ���ƥ���ַ���",
        "7.�Ե�����-������ƥ���ַ���",
        "8.����",
        "9.�˳�"
    };
}

int GramUI::UIMain()
{
    Menu();
    return 0;
}

int GramUI::Menu()
{
    while(true)
    {
        show();
        char buffer = getch();
        if(!issucesskey(buffer))
        {
            buffer = getch();
        }
        switch(buffer)
        {
        case 'w':
        case 'W':
        case 72:
        {
            if(choose > 1)
                choose--;
            else
                choose = menu_str.size();
            break;
        }
        case 's':
        case 'S':
        case 80:
        {
            if(choose < menu_str.size())
                choose++;
            else
                choose = 1;
            break;
        }
        case 13:  //�س�
        {
            try{
                MenuCall();
            }catch(GramExcept e)
            {
                cout<<e.what();
                getch();
            }
        }
        default:
            cin.clear();
            break;
        }
    }
}


/***
        "1.�ӿ���̨�����ķ�",
        "2.���ļ������ķ�",
        "3.��ʾ��ǰ�ķ�����",
        "4.��ʾ��ǰ�ķ�����",
        "5.�Զ�����-���ط�ƥ���ַ���",
        "6.�Զ�����-Ԥ���ƥ���ַ���",
        "7.�Ե�����-������ƥ���ַ���",
        "8.����",
        "9.�˳�"
***/
int GramUI::MenuCall()
{
    Showcursor();
    switch(choose)
    {
        case 1:
        {
            _gram = GramIO::Input(cin);
            break;
        }
        case 2:
        {
            ifstream in;		  //������ָ��in
            in.open("file.txt");  //������ķ�ʽ���ļ� file.txt
            _gram = GramIO::Input(in);
            break;
        }
        case 3:
        {
            GramIO::OutputGram(_gram);
            break;
        }
        case 4:
        {
            GramIO::OutputGramAttr(_gram);
            break;
        }
        case 5:
        {
            system("cls");
            cout<<"�������ַ���:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            cout<<"�����Զ�����-���ط�ƥ�����:"<<endl;
            GramMatch::topTodown(_gram,temp,0);
            getch();
            free(temp);
            break;
        }
         case 6:
        {
            system("cls");
            cout<<"�������ַ���:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            GramMatch::topTodown(_gram,temp,1);
            getch();
            free(temp);
            break;
        }
         case 7:
        {
            system("cls");
            cout<<"�������ַ���:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            GramMatch::downTotop(_gram,temp);
            getch();
            free(temp);
            break;
        }
         case 8:
        {
            help();
            break;
        }
        case 9:
        {
            exit(0);
            break;
        }
        default:
            cin.clear();
    }
    return 0;
}

void GramUI::show()
{
    Hidecursor();
    system("cls");
    unsigned row = 3;
    unsigned col = 15;
    unsigned i = 1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont);//���ð��ֺڱ���

    for(string cur_str : menu_str)
    {
        gotoxy(row,col);
        if(i == choose)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont_redBck);//���ð��ֺ챳��
        cout<<cur_str<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont);//���ð��ֺڱ���
        i++;
        row+=2;
    }
}

void GramUI::help()
{
    int row = 5;
    vector<string> help_str =
    {
        "1.�Ƶ�ʽ�еĿ���'$'���",
        "2.ÿ������һ���Ƶ�ʽ,�󲿺��Ҳ��ɿո����",
        "3.���ļ�β�ַ���������Ľ���(�ļ�β�ַ�������������������Ctrl+Zģ����ַ��Ҹ��ַ�Ӧ����������ͷ)",
        "4.�Ե�һ�����������Ϊ��ʼ����ʽ.",
        "5.���ս��ֻ��Ϊ��д��ĸ���ս��ֻ��ΪСд��ĸ�����������Լ�����",
        "6.�˵������¼�����W/S�����ƹ���ѡ�񣬻س������빦��",
        "7.���ļ���������ù���Ĭ�ϴ���Ŀ/����ͬĿ¼�ļ�'file'.txt�ж�ȡ"
    };
    system("cls");
    gotoxy(3,20);
    cout<<"��      ��"<<endl;
    for(string str : help_str)
    {
        gotoxy(row,10);
        cout<<str;
        row++;
    }

    getch();
}

bool GramUI::issucesskey(char buffer)
{
    string SucessKey = "WwSsPH\r\340";
    for(char ch : SucessKey)
    {
        if(buffer == ch)
            return true;
    }

    return false;
}


void GramUI::gotoxy(int y,int x)
{
    COORD pos;
    pos.X=x;
    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void GramUI::Hidecursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
    CursorInfo.bVisible = false; //���ؿ���̨���
    SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}

void GramUI::Showcursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = true;
    SetConsoleCursorInfo(handle, &CursorInfo);
}
