#include"GramIO.h"

Gram GramIO::Input(istream &in)
{
    system("cls");
     if(typeid(in) == typeid(cin))
        cout<<"�������ķ�:"<<endl;
    if(in.peek() == -1)
    {
        throw GramExcept("�ļ���ȡʧ��");
    }
    vector<Rule> rules;
    string buffer;
    string start;
    set<char> NoEndSym;
    set<char> EndSym;
    while(getline(in,buffer))  //ѭ���������ÿ��һ��
    {
        buffer = trim(buffer);
        if(buffer.substr(0,buffer.find(' ')).size() != 1)
            throw GramExcept("�Ƶ�ʽ�󲿶�ȡ�쳣!");
        rules.push_back(Rule(buffer.substr(0,buffer.find(' '))[0],buffer.substr(buffer.find(' ')+1,buffer.size())));

        for(char ch : buffer)
        {
            if(Gram::isNoEndSym(ch))
                NoEndSym.insert(ch);
            else if(Gram::isEndSym(ch))
                EndSym.insert(ch);
        }
    }

    start = rules[0].left();  //��һ�����ս��Ϊ��ʼ��

    if(!Gram::isNoEndSym(rules[0].left()))
        throw GramExcept("�ķ�������ʼ����ȡ�쳣!");

    Gram gram(NoEndSym,EndSym,rules,start[0]);
    cout<<"��ȡ�ɹ�!..�����������"<<endl;
    getch();
    return gram;
}

void GramIO::OutputRule(Gram gram)
{
    for(Rule rule: gram.getRule())
    {
        cout<<rule.left()<<"->"<<rule.right()<<endl;
    }
}

void GramIO::OutputGram(Gram gram)
{
    system("cls");
    if(gram.isempty())
    {
        cout<<"��ǰ�ķ�Ϊ��,��������ٲ鿴!"<<endl;
        getch();
        return;
    }
    cout<<"G["<<gram.getStartSym()<<"] = {"<<endl;
    cout<<"{";
    for(char ch:gram.getNoEndSym())
    {
        cout<<ch<<",";
    }
    if(gram.getNoEndSym().empty())
        cout<<"}"<<endl;
    else
        cout<<"\b}"<<endl;

    for(char ch:gram.getEndSym())
    {
        cout<<ch<<",";
    }
    if(gram.getEndSym().empty())
        cout<<"},"<<endl<<"{"<<endl;
    else
        cout<<"\b},"<<endl<<"{"<<endl;

    OutputRule(gram);
    cout<<"},"<<endl<<gram.getStartSym()<<" }"<<endl;
    getch();
}

void GramIO::OutputGramAttr(Gram gram)
{
    system("cls");
    if(gram.isempty())
    {
        cout<<"��ǰ�ķ�Ϊ��,��������ٲ鿴!"<<endl;
        getch();
        return;
    }
    OutputFirstvec(gram);
    OutputFollowvec(gram);
    OutputSelectvec(gram);
    OutputPriTable(gram);
    OutputPreTable(gram);
    cout<<endl<<"���Թؼ���:";
    if(GramMatch::LeftRecursion_check(gram))
        cout<<" ��ݹ�";
    if(GramMatch::LL1_check(gram))
        cout<<" LL1�ķ�";
    if(GramMatch::SimPri_check(gram))
        cout<<" �������ķ�";
    if(GramMatch::OPG_check(gram))
        cout<<" ����ķ�";
    cout<<endl;
    cout<<"..�����������"<<endl;
    getch();
}


void GramIO::OutputFirstvec(Gram gram)
{
    for(pair<Rule,set<char>> cur_pair : GramMatch::get_firstvec(gram))
    {
      cout<<"First("<<cur_pair.first.left()<<"->"<<cur_pair.first.right()<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputFollowvec(Gram gram)
{
    for(pair<char,set<char>> cur_pair : GramMatch::get_followvec(gram))
    {
      cout<<"Follow("<<cur_pair.first<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputSelectvec(Gram gram)
{
    for(pair<Rule,set<char>> cur_pair : GramMatch::get_selectvec(gram))
    {
      cout<<"Select("<<cur_pair.first.left()<<"->"<<cur_pair.first.right()<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputPriTable(Gram gram)  //�����ȱ�
{
    map<pair<char,char>,char> table = GramMatch::Priority_Table(gram);
    cout<<endl<<"�����ȱ�:"<<endl;
    vector<char> symvec;
    for(char ch : gram.getEndSym())
        symvec.push_back(ch);
    for(char ch : gram.getNoEndSym())
        symvec.push_back(ch);

    for(char rowch : symvec)  //��ӡ����
        cout<<"\t"<<rowch;
    cout<<endl;

    for(char colch : symvec)
    {
        cout<<colch;
        for(char rowch : symvec)
        {
            pair<char,char> temp_pair(colch,rowch);
            if(table.find(temp_pair) == table.end())
                cout<<"\t ";
            else
                cout<<"\t"<<table[temp_pair];
        }
        cout<<endl;
    }
}

void GramIO::OutputPreTable(Gram gram)  //Ԥ�������
{
    cout<<endl<<"Ԥ�������:"<<endl;
    for(char Endch : gram.getEndSym())
        cout<<"\t"<<Endch;
    cout<<endl;
    for(char NoEndch : gram.getNoEndSym())
    {
        cout<<NoEndch;
        for(char Endch : gram.getEndSym())
        {
            Rule rule = GramMatch::Pretable(gram,NoEndch,Endch);
            if(rule.isempty())
                cout<<"\t ";
            else
                cout<<"\t->"<<rule.right();
        }
        cout<<endl;
    }
}

std::string& GramIO::trim(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void GramIO::showstack(stack<char> s)
{
    stack<char> temp;
    while(!s.empty())
    {
        temp.push(s.top());
        s.pop();
    }
    while(!temp.empty())
    {
        cout<<temp.top();
        temp.pop();
    }
}



