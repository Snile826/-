#ifndef GRAMMER_H
#define GRAMMER_H

#include"GramExcept.h"
#include<iostream>
#include<set>
#include<vector>
using namespace std;


/******
�Ƶ�ʽ��
    ʹ��char�洢��,ʹ��string�洢�Ҳ����������
    �Ķ�������ֱ����ȡ�Ƶ�ʽ������,һ����ΪGram
    ���һ����ʹ��ʹ��
    GramIO�����໹�������ر�ĵķ�ʽ������ķ�
******/
class Rule{
private:
    /*
    �洢ԭʼ�Ƶ�ʽ
    ����ʹ��pair<char,string>������ֻ�ܴ洢���ͣ������ķ��Ƶ�ʽ
    */
    pair<char,string> _gram;
public:
    /*
    Ĭ�Ϲ��캯�������ڹ�����Ƶ�ʽ
    Ĭ�Ͽ��Ƶ�ʽ�洢����Ϊ 0->""
    */
    Rule();
    /*
    ���캯�������ڹ����Ƶ�ʽ
    ��Σ��Ƶ�ʽ�󲿣��Ƶ�ʽ�Ҳ�
    */
    Rule(char left,string right);

    //�����ж������Ƶ�ʽ�Ƿ����
    bool operator ==(const Rule &r)const;

    //�����ж��Ƶ�ʽ��ascii����С
    bool operator <(const Rule &r)const;

    Rule& operator =(const Rule &r);

    // �����Ƶ�ʽ��
    void setleft(char s);
    // �����Ƶ�ʽ�Ҳ�
    void setright(string s);
    // �����Ƶ�ʽ��
    char left()const;
    // �����Ƶ�ʽ�Ҳ�
    string right()const;
    // �ж��Ƶ�ʽ�Ƿ�Ϊ'��'
    bool isempty()const;
};

/******
�ķ���
    ʹ��char�洢���ս�����ս��,�������Ķ����
    ����ֱ���жϸ��ķ�������,��ȡ�ķ�������.ʹ��
    GramIO�����໹�������ر�ĵķ�ʽ������ķ�
******/
class Gram{
private:
    set<char> _NoEndSym;   //���ս������
    set<char> _EndSym;     //�ս������
    vector<Rule> _Rules;   //�Ƶ�ʽ����
    char _StartSym;        //��ʼ��

public:
    Gram();
    Gram(set<char> a,set<char> b,vector<Rule> c,char d); //Gram���캯��

    char getStartSym()const;                //��ȡ�ķ�����ʼ��
    set<char> getNoEndSym()const;           //��ȡ�ķ��ķ��ս������
    set<char> getEndSym()const;             //��ȡ�ķ����ս������
    vector<Rule> getRule()const;            //��ȡ�ķ���ȫ���Ƶ�ʽ
    vector<Rule> getRule(char Symbol)const; //��ȡĳ�����ս���������Ƶ�ʽ

    static bool isNoEndSym(char ch);    //�ж��ַ��Ƿ��Ƿ��ս��,����ֻ����Χ�ж�
    static bool isEndSym(char ch);      //�ж��ַ��Ƿ����ս��,����ֻ����Χ�ж�

    char getNoEndSym(string right)const;    //�����Ƶ�ʽ���Ҳ���ȡ����,������Ҳ�û�����򷵻� #
    bool isempty()const;                    //�ж��ķ��߼����Ƿ�Ϊ��
};


#endif
