#include"GramMatch.h"

//�������:��ʼ����ʽ�Ĺ��򼯺ϣ���Ҫƥ����ַ���
//����:
//���� �ķ������򴮣���ƥ�䴮
//�����ƥ�䴮ƥ�䵽���򴮵Ĵ�д��ĸ�򽫴�д��ĸ��ȷչ��
//�����ƥ�䴮ƥ�䵽���򴮵�Сд��ĸ��ɹ��Ͳ�����ʧ���򷵻�false
bool GramMatch::TD_match(Gram gram,string match,const char *str)
{
    cout<<match<<"\t"<<str<<endl;
    if((match.size()==0 || match == "$") && *str == '\0')
        return true;
    for(char ch : match)
    {
        if(Gram::isNoEndSym(ch))
        {
            bool hasnull = false;
            for(Rule r : gram.getRule(ch))  //չ������,Ѱ����ȷ��·��
            {
               for(char r_ch : get_first(gram,r))
               {
                   if(r_ch == *str) // ��ƥ��չ����Ĺ���
                   {
                       match.erase(0,1);
                       match.insert(0,r.right());
                       cout<<"չ���Ƶ�ʽ:"<<r.left()<<"->"<<r.right()<<endl;
                       return TD_match(gram,match,str);
                   }
               }
               set<char> temp = get_first(gram,r);
               if(temp.find('$') != temp.end())
                    hasnull = true;
            }
            if(hasnull)     //�Կմ�����ս��
            {
                match.erase(0,1);
                match.insert(0,"$");
                cout<<"չ���Ƶ�ʽ"<<ch<<"->$"<<endl;
                return TD_match(gram,match,str);
            }
            else
                break;  //����չ��ʽ�������㣬�˳�
        }
        else if(Gram::isEndSym(ch))
        {
            if(*str == ch)
            {
                match.erase(0,1);
                str++;
                cout<<"����"<<endl;
                return TD_match(gram,match,str);
            }
            else
                return false;
        }
        else if(ch == '$')
        {
            if(*str=='\0')
            {
                return true;
            }
            else
                continue;
        }
    }

    return false;
}

//�ڲ���������,����ѰԤ�������
Rule GramMatch::Pretable(Gram gram,char NoEndSym,char EndSym)
{
    const vector<pair<Rule,set<char>>> selectvec = get_selectvec(gram);
    for(pair<Rule,set<char>> select : selectvec)
    {
        if(select.first.left() == NoEndSym)
        {
            if(select.second.find(EndSym) != select.second.end()) //�ü����и÷���
            {
                return select.first;
            }
        }
    }

    return Rule();
}
//�ڲ�����,�Զ�����-Ԥ��������
bool GramMatch::Pretable_match(Gram gram,const char *str)
{
    char *temp = (char*)malloc(strlen(str)+2);
    strcpy(temp,str);
    str = strcat(temp,"#"); // ��ԭʼ�ַ��������Զ����� #

    stack<char> mstack;  //ƥ��ջ
    mstack.push('#');
    mstack.push(gram.getStartSym());

    GramIO::showstack(mstack);
    cout<<"\tƥ��\t";
    cout<<"\t"<<str<<endl;

    while(!mstack.empty())
    {
        if(Gram::isNoEndSym(mstack.top()))
        {
            Rule rule = Pretable(gram,mstack.top(),*str);
            if(rule.isempty())
            {
                return false;
            }

            mstack.pop();
            for(int i = rule.right().size()-1; i >= 0 ;i--)
            {
                if(rule.right()[i] == '$')
                    continue;
                mstack.push(rule.right()[i]);
            }

                GramIO::showstack(mstack);
                cout<<"\tԤ��"<<rule.left()<<"->"<<rule.right()<<"\t";
                cout<<"\t"<<str<<endl;
        }
        else if(Gram::isEndSym(mstack.top()))
        {
            if(*str == mstack.top())
            {
                mstack.pop();
                str++;

                GramIO::showstack(mstack);
                cout<<"\t����\t";
                cout<<"\t"<<str<<endl;

            }
            else
            {
                return false;
            }
        }
        else if(mstack.top() == '#')
        {
            if(*str == mstack.top())
            {
                mstack.pop();
                str++;

                GramIO::showstack(mstack);
                cout<<"\t"<<str<<endl;

            }
            else
                return false;
        }
    }

    return true;
}

// �Ե����ϵ�ƥ�䷽��
bool GramMatch::DT_match(Gram gram,const char *str)
{
    char *temp = (char*)malloc(strlen(str)+2);
    strcpy(temp,str);
    str = strcat(temp,"#"); // ��ԭʼ�ַ��������Զ����� #

    stack<char> mstack;
    mstack.push('#');

    GramIO::showstack(mstack);
    cout<<"\tƥ��\t";
    cout<<"\t"<<str<<endl;

    while(true)
    {
        if(Priority(gram,mstack.top(),*str) == '>')  //��Լ
        {
            if(Priority(gram,mstack.top(),'$') == '<')
            {
                mstack.push(gram.getNoEndSym("$"));
                GramIO::showstack(mstack);
                cout<<"\t��Լ:"<<"$"<<"->"<<gram.getNoEndSym("$");
                cout<<"\t"<<str<<endl;
                continue;
            }
            string cur_str = "";         // �����洢��ǰ��Լ��
            char temp;                  // ��ʱ�ַ�
            temp = mstack.top();

            cur_str.insert(0,1,temp);
            mstack.pop();
            while(Priority(gram,mstack.top(),cur_str[0]) != '<')
            {
                cur_str.insert(0,1,mstack.top());
                mstack.pop();
            }
            temp = gram.getNoEndSym(cur_str);  //��ȡ��

            if(Gram::isNoEndSym(temp))
            {
                mstack.push(temp);

                GramIO::showstack(mstack);
                cout<<"\t��Լ:"<<cur_str<<"->"<<temp;
                cout<<"\t"<<str<<endl;

                if(*str == '#' && mstack.size() == 2 && mstack.top() == gram.getStartSym())
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            mstack.push(*str);  //�ƽ�
            str++;

            GramIO::showstack(mstack);
            cout<<"\t�ƽ�";
            cout<<"\t"<<str<<endl;
        }
    }
}


//���ȼ��Ƚ�,�Ƚ��ķ��������������ŵ����ȼ�
char GramMatch::Priority(Gram gram,char A,char B)
{
    map<pair<char,char>,char> PTable = Priority_Table(gram);
    if(A == '#' && B!='#')
        return '<';
    else if(B == '#' && A!='#')
        return '>';
    else if(A == '#' && B == '#')
        return '=';

    pair<char,char> ret(A,B);
    if(PTable.find(ret) == PTable.end())
        return ' ';
    else
        return PTable[ret];
}

//�����Ե�����ƥ�䣬Ѱ���ķ��з��ս���Ƶ�ʽ���ܳ��ֵ�ͷ�����ţ������������ս���򷵻�����ļ��ϡ�
set<char> GramMatch::SymHead(Gram gram,char start)
{
    set<char> ret;
    if(Gram::isNoEndSym(start))
    {
        stack<char> left_stack;
        left_stack.push(start);
        while(!left_stack.empty())
        {
            char temp = left_stack.top();
            left_stack.pop();
            for(Rule rule : gram.getRule(temp))
            {
                if(Gram::isNoEndSym(rule.right()[0]) && ret.find(rule.right()[0]) == ret.end())
                    left_stack.push(rule.right()[0]);

                ret.insert(rule.right()[0]);
            }
        }
    }
    else if(Gram::isEndSym(start))
    {
        ret.insert(start);
    }

    return ret;
}

//�����Ե�����ƥ�䣬Ѱ���ķ��з��ս�����Ƶ�ʽ�Ŀ��ܳ��ֵ����һ�����ţ������������ս���򷵻�����ļ���
set<char> GramMatch::Symtail(Gram gram,char start)
{
    set<char> ret;
    if(Gram::isNoEndSym(start))
    {
        stack<char> left_stack;
        left_stack.push(start);
        while(!left_stack.empty())
        {
            char temp = left_stack.top();
            left_stack.pop();
            for(Rule rule : gram.getRule(temp))
            {
                const unsigned lastindex = rule.right().size()-1;
                if(Gram::isNoEndSym(rule.right()[lastindex]) && ret.find(rule.right()[lastindex]) == ret.end())
                {
                    left_stack.push(rule.right()[lastindex]);
                }
                ret.insert(rule.right()[lastindex]);
            }
        }
    }
    else if(Gram::isEndSym(start))
    {
        ret.insert(start);
    }

    return ret;
}

// ��ȡ���������First��
set<char> GramMatch::get_first(Gram gram,Rule rule)   // wait....
{
  //  cout<<"��"<<rule.left()<<"->"<<rule.right()<<endl;
    set<char> ret;
    if(Gram::isEndSym(rule.right()[0]))
        ret.insert(rule.right()[0]);
    else if(Gram::isNoEndSym(rule.right()[0]))
    {
        unsigned int index = 0;
        for(char ch : rule.right())
        {
            if(Gram::isNoEndSym(ch))
            {
                if(ch == rule.left())   //��ݹ�
                    continue;
                for(Rule rule : gram.getRule(ch))
                {
                    set<char> temp = get_first(gram,rule);
                    ret.insert(temp.begin(),temp.end());
                }
                if(ret.find('$') == ret.end()) //�����ս����First����û�� �գ���ù����First�����ǵ�ǰ��ret
                {
                    break;
                }
                else if(index != rule.right().size()-1) //��,���ս���������һ�������� �� ��ɾ���� �գ����ұ����ù�����һ���ַ�
                {
                    ret.erase(ret.find('$'));
                }
            }
            else if(Gram::isEndSym(ch))
            {
                ret.insert(ch);
                break;
            }
            index++;
        }
    }
    else if(rule.right()=="$")
    {
        ret.insert('$');
    }

    return ret;
}

// ��ȡ�������ս����Follow��

//����Ҫ��д
//���̣�
// �������ǿ�ʼ����ʽ �� set ��ֻ���� $
// ���򣬱������в���ʽ
// ���� A -> ��B�� , ����First(��)��û��{$}��First(��)����Follow(B)������First(��)��Follow(A)������Follow(B)
// ���� A ->��B �� Follow(A) ���� Follow(B)
set<char> GramMatch::get_follow(Gram gram,char left)
{
    set<char> ret;
    if(left == gram.getStartSym())
        ret.insert('#');

    for(Rule rule : gram.getRule())
    {
        unsigned int index = 0;
        for(char ch : rule.right())
        {
//       cout<<"��ǰ��"<<left<<"������"<<rule.right()<<","<<rule.right().size()<<","<<index<<endl;
            if(ch == left && index != rule.right().size()-1)
            {
                if(Gram::isEndSym(rule.right()[index+1]))
                {
                    ret.insert(rule.right()[index+1]);
                    break;
                }
                else if(Gram::isNoEndSym(rule.right()[index+1]))
                {
                    for(Rule frule : gram.getRule(rule.right()[index+1]))
                    {
                        set<char> temp = get_first(gram,frule);
                        ret.insert(temp.begin(),temp.end());
                    }
                    if(ret.find('$') != ret.end())
                    {
                        ret.erase('$');
                        if(index+1 == rule.right().size()-1) //���һ�����ս��
                        {
                            set<char> temp = get_follow(gram,rule.left());
                            ret.insert(temp.begin(),temp.end());
                        }
                    }
                    else
                        break;
                }
            }
            else if(ch == left && index == rule.right().size()-1)
            {
                if(rule.left() == left)   //��ֹ�ҵݹ�
                  continue;
                set<char> temp = get_follow(gram,rule.left());
                ret.insert(temp.begin(),temp.end());
            }
            index++;
        }
    }

    return ret;
}

bool GramMatch::match_precheck(const char *str)
{
    while(*str != '\0')
    {
        if(Gram::isEndSym(*str))
        {
            str++;
        }
        else
            return false;
    }
    return true;
}
/**********  ������private����,������public����**************/
/*
�����ķ���ÿ�������First������vector��
*/
vector<pair<Rule,set<char>>> GramMatch::get_firstvec(Gram gram)
{
    vector<pair<Rule,set<char>>> ret;
    for(Rule rule : gram.getRule())
    {
        pair<Rule,set<char>> cur;
        set<char> cur_set;
        cur.first = rule;
        cur_set = get_first(gram,rule);
        cur.second = cur_set;
        ret.push_back(cur);
    }
    return ret;
}

vector<pair<char,set<char>>> GramMatch::get_followvec(Gram gram)
{
    vector<pair<char,set<char>>> ret;
    for(char ch: gram.getNoEndSym())
    {
//        cout<<"���е�"<<ch<<endl;
        pair<char,set<char>> cur;
        set<char> cur_set;
        cur.first = ch;
        cur_set = get_follow(gram,ch);
        cur.second = cur_set;
        ret.push_back(cur);
    }
    return ret;
}

vector<pair<Rule,set<char>>> GramMatch::get_selectvec(Gram gram)
{
//  cout<<"in get_selectvec()"<<endl;
    vector<pair<Rule,set<char>>> ret;
    for(Rule rule : gram.getRule())
    {
//      cout<<"get_select:"<<rule.left()<<"->"<<rule.right()<<endl;
        pair<Rule,set<char>> cur_pair;
        cur_pair.first = rule;

        set<char> first = get_first(gram,rule);
        if(first.find('$') != first.end())  //First�����ڿ�
        {
            set<char> follow = get_follow(gram,rule.left());
            cur_pair.second = first;
            cur_pair.second.erase('$');
            cur_pair.second.insert(follow.begin(),follow.end());
        }
        else    //First�������ڿ�
        {
            cur_pair.second = first;
        }

        ret.push_back(cur_pair);
    }
    return ret;
}

//�ж��Ƿ����LL1
bool GramMatch::LL1_check(Gram gram)
{
    if(LeftRecursion_check(gram))  //��ݹ鲻��LL1
        return false;
//    cout<<"in LL1_check()"<<endl;
    vector<pair<Rule,set<char>>> select = get_selectvec(gram);
    for(char NoEndSym : gram.getNoEndSym())
    {
//        cout<<"LL1_check:"<<NoEndSym<<endl;
        unsigned int count = 0;
        set<char> Nset;
        for(pair<Rule,set<char>> p : select)
        {
            if(p.first.left() == NoEndSym)
            {
                count += p.second.size();
                Nset.insert(p.second.begin(),p.second.end());
            }
        }
        if(count != Nset.size())
            return false;
    }

    return true;
}

// �ж��Ƿ������ݹ�
bool GramMatch::LeftRecursion_check(Gram gram)
{
    for(Rule rule : gram.getRule())
    {
        for(char ch : rule.right())
        {
            if(Gram::isNoEndSym(ch))
            {
                if(ch == rule.left())
                    return true;   //����ݹ�
                set<char> temp;
                for(Rule Irule : gram.getRule(ch))
                {
                    set<char> Itemp = get_first(gram,Irule);
                    temp.insert(Itemp.begin(),Itemp.end());
                }
                if(temp.find('$') == temp.end())
                    break;
                else
                    continue;
            }
            else if(Gram::isEndSym(ch))
            {
                break;
            }
        }
    }

    return false;
}


// �ж��Ƿ���ϼ����ȹ���
bool GramMatch::SimPri_check(Gram gram)
{
    try{
        Priority_Table(gram);
        unsigned count = 0;
        set<string> right;
        for(Rule rule : gram.getRule())
        {
            count++;
            right.insert(rule.right());
        }
        if(count != right.size())
            return false;
        else
            return true;
    }catch(GramExcept ex)
    {
        return false;
    }
}


//�ж��Ƿ�����������
bool GramMatch::OPG_check(Gram gram)
{
    return false;
}

// �����Ե�����ƥ�䣬�����ķ��Ƶ�ʽ�����ȼ���ϵ
map<pair<char,char>,char> GramMatch::Priority_Table(Gram gram)
{
    map<pair<char,char>,char> ret;
    for(Rule rule : gram.getRule())  // ��ÿ���Ƶ�ʽ��Ѱ�����ȹ�ϵ
    {
//        cout<<"������"<<rule.left()<<"->"<<rule.right()<<endl;
        if(rule.right().size() < 2)
            continue;

        for(unsigned i = 0; i <= rule.right().size()-2; i++) //ÿ�α����Ƚ�һ��
        {
            /*******  ���ڹ�ϵ ***************/
            pair<char,char> eq_pair(rule.right()[i],rule.right()[i+1]);
            if(ret.find(eq_pair) == ret.end())  //��һ����Ӹ��ķ������ȼ���ϵ
                ret[eq_pair] = '=';
            else
            {
                if(ret[eq_pair] != '=')  //��������������ȼ���ϵ���׳��쳣
                    throw GramExcept("���ķ����Ǽ������ķ�");
            }
          //cout<<"���ڹ�ϵ"<<eq_pair.first<<"="<<eq_pair.second<<endl;

            /******* С�ڹ�ϵ **************/
            if(Gram::isNoEndSym(rule.right()[i+1]))
            {
               set<char> lt_set = SymHead(gram,rule.right()[i+1]);
               for(char ch : lt_set)
               {
                    pair<char,char> lt_pair(rule.right()[i],ch);
                    if(ret.find(lt_pair) == ret.end())
                        ret[lt_pair] = '<';
                    else
                    {
                        if(ret[lt_pair] != '<')
                            throw GramExcept("���ķ����Ǽ������ķ�");
                    }
                //cout<<"С�ڹ�ϵ"<<lt_pair.first<<"<"<<lt_pair.second<<endl;
               }
            }

            /******* ���ڹ�ϵ ***********/
            if(Gram::isNoEndSym(rule.right()[i]))
            {
                set<char> gt_left_set = Symtail(gram,rule.right()[i]);
                set<char> gt_right_set = SymHead(gram,rule.right()[i+1]);
                for(char ch : gt_left_set)
                {
                    for(char inch : gt_right_set)
                    {
                        pair<char,char> gt_pair(ch,inch);
                        if(ret.find(gt_pair) == ret.end())
                            ret[gt_pair] = '>';
                        else
                        {
                            if(ret[gt_pair] != '>')
                                throw GramExcept("���ķ����Ǽ������ķ�");
                        }
                       //cout<<"���ڹ�ϵ"<<gt_pair.first<<">"<<gt_pair.second<<endl;
                    }
                }
            }
        }
    }

    return ret;
}

/****
  �Զ������ķ�ƥ�䣬Ҫ�����ķ��Լ�ƥ�䴮
  �ķ�������LL1�ķ�
****/
void GramMatch::topTodown(Gram gram,const char *str,int type)
{
    if(LL1_check(gram) && match_precheck(str))
    {
        bool sucess = false;
        if(type == 0)
        {
            char temp[2] = {gram.getStartSym(),0};
            if(TD_match(gram,string(temp),str))
                sucess = true;
        }
        else if(type == 1)
        {
            if(Pretable_match(gram,str))
                sucess = true;
        }
        if(sucess)
            cout<<"ƥ��ɹ�"<<endl;
        else
            cout<<"ƥ��ʧ��"<<endl;
    }
    else
    {
        throw GramExcept("���ķ�����LL1�ķ���ƥ�䴮��������,�޷�ʹ�ø÷���ƥ��");
    }
}

/****
  �Ե������ķ�ƥ�䣬Ҫ�����ķ��Լ�ƥ�䴮
  �ķ������� �������ķ�
****/
void GramMatch::downTotop(Gram gram,const char *str)
{
    if(SimPri_check(gram) && match_precheck(str))
    {
        if(DT_match(gram,str))
            cout<<"ƥ��ɹ�"<<endl;
        else
            cout<<"ƥ��ʧ��"<<endl;
    }
    else
    {
        throw GramExcept("���ķ����Ǽ������ķ���ƥ�䴮��������,�޷�ʹ�ø÷���ƥ��");
    }
}

