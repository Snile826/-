#ifndef MATCHGRAM_H
#define MATCHGRAM_H
#include"Grammer.h"
#include"GramExcept.h"
#include"GramIO.h"
#include<vector>
#include<stack>
#include<map>
#include<cstring>
#include<stdlib.h>
#include <algorithm>

using namespace std;
class GramMatch
{
private:
    //�ڲ�����,�Զ�����-����ƥ�䷨.
    //�������:��ʼ����ʽ�Ĺ��򼯺ϣ���Ҫƥ����ַ���
    static bool TD_match(Gram gram,string match,const char *str);

    //�ڲ�����,�Զ�����-Ԥ���ƥ�䷨
    static bool Pretable_match(Gram gram,const char *str);

    //�ڲ�����,�Ե����ϼ����ȷ�ƥ��.
    static bool DT_match(Gram gram,const char *str);


    static char Priority(Gram gram,char A,char B);   //��������,���ȼ��Ƚ�,�Ƚ��ķ��������������ŵ����ȼ�

    static set<char> SymHead(Gram gram,char start);  //���ڸ����Ե�����ƥ��,��ȡĳ���ŵ��Ƶ�ʽͷ�����ż���,����������ս��,�򷵻�����ļ���
    static set<char> Symtail(Gram gram,char start);  //���ڸ����Ե�����ƥ��,��ȡĳ���ŵ��Ƶ�ʽβ�����ż���,����������ս��,�򷵻�����ļ���

    static set<char> get_first(Gram gram,Rule rule);     //��������,��ȡ�ķ���ĳ���Ƶ�ʽ��First��
    static set<char> get_follow(Gram gram,char left);    //��������,��ȡ�ķ���ĳ�����ս����Follow��

    static bool match_precheck(const char *str);  //ƥ��ǰ���
public:
    static bool LL1_check(Gram gram);           //LL1�ķ����
    static bool LeftRecursion_check(Gram gram); //��ݹ���
    static bool SimPri_check(Gram gram);        //�������ķ����
    static bool OPG_check(Gram gram);  //����ķ����

    static vector<pair<Rule,set<char>>> get_firstvec(Gram gram);  // ÿ���Ƶ�ʽ��first����vector
    static vector<pair<char,set<char>>> get_followvec(Gram gram); // ÿ�����ս����follow����vector
    static vector<pair<Rule,set<char>>> get_selectvec(Gram gram); // ÿ���Ƶ�ʽ��select����vector
    static map<pair<char,char>,char> Priority_Table(Gram gram);   //�����ķ�,�����ȼ���,��������map��
    static Rule Pretable(Gram gram,char NoEndSym,char EndSym);    //ѰԤ�������

    static void topTodown(Gram gram,const char *str,int type);  //�Զ�����ƥ��
    static void downTotop(Gram gram,const char *str);           //�Ե�����ƥ��
};

#endif
