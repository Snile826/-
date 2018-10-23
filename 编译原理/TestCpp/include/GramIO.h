#ifndef GRAMIO_H
#define GRAMIO_H
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<typeinfo>
#include"GramExcept.h"
#include"Grammer.h"
#include"GramMatch.h"
#include<stdlib.h>
#include<conio.h>
#include<stack>

class GramIO{
public:
    //Input
    /*
    Gram����
    ���:������
    ʹ�ô�������������ж�ȡ�Ƶ�ʽ��ÿ������:
    A aA
    ���Ϊ�Ƶ�ʽ��,�ұ�Ϊ�Ƶ�ʽ�Ҳ�
    ���ǿ���̨����,����Ctrl+Zģ���ļ�������������
    */

    static Gram Input(istream &in);


    //Output
    static void OutputRule(Gram gram);      //����̨��ʽ������ķ����Ƶ�ʽ
    static void OutputGram(Gram gram);      //����̨��ʽ������ķ�
    static void OutputGramAttr(Gram gram);  //����̨��ʽ������ķ�����(����First��,Follow��,Select��,�ķ�����)
    static void OutputFirstvec(Gram gram);  //����̨��ʽ������ķ���First���б�
    static void OutputFollowvec(Gram gram); //����̨��ʽ������ķ���Follow���б�
    static void OutputSelectvec(Gram gram); //����̨��ʽ������ķ���Select���б�

    static void OutputPriTable(Gram gram);  //�����ȱ�
    static void OutputPreTable(Gram gram);  //Ԥ�������



    static std::string& trim(std::string &s);   //ȥ�ַ�����β�ո�

    static void showstack(stack<char> s);   //˳����ʾջ������
};

#endif


