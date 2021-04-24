#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<cstring>

using namespace std;
#define MAX_USER 10000
typedef enum{
EQUAL,
EXACT,
NONE
}expense_type;

class User
{
    int id;
    string name,email,mobile;
public:
   User(int id,string name,string email,string mobile)
   {
      this->id=id;
      this->name=name;
      this->email=email;
      this->mobile = mobile;
      
   }
     int getId()
    {
        return this->id;
    }
    string getName()
    {
        return this->name;
    }
    string getEmail()
    {
        return this->email;
    }
    string getMobile()
    {
        return this->mobile;
    }
    void setName(string newName)
    {
        this->name = newName;
    }
    void setEmail(string newEmail)
    {
        this->email = newEmail;
    }
    void setMobile(string mobile)
    {
        this->mobile=mobile;
    } 
};
class Expense
{
   int fromUid,toUid;
   expense_type expenseType;
   double amount;
   public:
    Expense(int fromUid,int toUid,string type,double amount)
    {
        this->fromUid=fromUid;
        this->toUid=toUid;
        this->amount=amount;
          if(type.compare("EQUAL"))
          {
              this->expenseType=EQUAL;

          }
          else if(type.compare("EXACT"))
          {
                this->expenseType=EXACT;
          }
    }
   string getexpenseType()  
   {
       if(this->expenseType==EQUAL)
         return "EQUAL";
        else if(this->expenseType==EXACT)
        return "EQUAL";
        return "Undefined";
   }           
  int getFromUid()
  {
      return this->fromUid;
  }
  int getToUid()
  {
      return this->toUid;
  }
  double getAmount()
  {
      return this->amount;
  }
  void setAmount(double amount)
  {
      this->amount=amount;
  }
};

class UserManager
{
    map<int,User*> usermap;
    UserManager()
    {
        usermap.clear();
    }
   static UserManager *usermanager; 
   public:
   static UserManager *getinstance()
   {
       if(!usermanager)
         usermanager =new UserManager;

     return usermanager;
   }
   void adduser(int id,string name,string email,string mobile)
   {
       if(usermap.find(id)!=usermap.end())
       {
           cout<<"User Already existed";
           return ;
       }
       User *user=new User(id,name,email,mobile);
       usermap[id]=user;

   }
   bool checkUser(int id)
   {
       if(usermap.find(id)==usermap.end())
       return false;
       return true;
         
   }
   User *getuser(int id)
   {
       if(checkUser(id))
        return usermap[id];
      return NULL;
        
   }

};
UserManager *UserManager::usermanager=0;

class ExpenseManager
{
    map<int,vector<Expense*>> expensegraph;
    
    
    ExpenseManager()
    {
      expensegraph.clear();
    } 
   static ExpenseManager *INSTANCE;
   
  public:
     static  ExpenseManager *getinstance()
      {
          if(!INSTANCE)
           INSTANCE = new ExpenseManager;

           return INSTANCE;
      }
     double roundoff(double amount)
     {
         double value =(int)(amount*100+0.5);
         return (double)value/100;
     }
     string addexpense(int from,double totalexpense,int *touid,int toUidsize,double *exact,string type)
     {
         UserManager *usermanager = usermanager->getinstance();
         if(totalexpense<=0)
         {
             return"Expense can not be negative";
         }
        double exact1[toUidsize];
         if(type.compare("EQUAL")==0)
         {
            double perpersonAmount=totalexpense/(toUidsize);
            toUidsize--;
            touid++;
            for(int i=0;i<toUidsize;i++)
              exact1[i]=roundoff(perpersonAmount);

         }
         if(type.compare("EXACT")==0)
         {
            //Validating the exact amounts
             double exactsum=0;
               for(int i=0;i<toUidsize;i++)
               { 
                    exactsum+=exact[i];
                    exact1[i]=exact[i];

               }
               if(exactsum!=totalexpense)
               {
                   return "Expense is not correct";
                   
               }
         }
            if(!usermanager->checkUser(from))
            {
                return "User does not exist";
                
            }
            vector<Expense*> te;
            if(expensegraph.find(from)!=expensegraph.end())
            {
                       te=expensegraph[from];
            }
            for(int i=0;i<toUidsize;i++)
            {
               
                 if(!usermanager->checkUser(touid[i]))  
                   {
                       return "User Does not exist";
                       
                   }
                int j,k;
                vector<Expense *> uid2;
                if(expensegraph.find(touid[i])!=expensegraph.end())
                {
                   uid2=expensegraph[touid[i]];

                }
                for( j=0;j<te.size();j++)
                {
                    if(te[j]->getToUid()==touid[i])
                    {
                       te[j]->setAmount(te[j]->getAmount()+exact1[i]); 
                       break;
                    }
                }
                for(k=0;k<uid2.size();k++)
                {
                    if(uid2[k]->getToUid()==from)
                    {
                        uid2[k]->setAmount(uid2[k]->getAmount()-exact1[i]);
                        break;
                    }
                }
                
                if(j==te.size())
                {
                    te.push_back(new Expense(from,touid[i],type,exact1[i]));
                    uid2.push_back(new Expense(touid[i],from,type,-exact1[i])); 
                }
                expensegraph[touid[i]]=uid2;    
            }
            expensegraph[from]=te;
             
             return "OK";
         }

       void show(int uid)
       {
         
         UserManager *usermanager = usermanager->getinstance();
         if(!usermanager->checkUser(uid))
         {
             cout<<"User does not exist";
             return ;
         }
        vector<Expense *> te;
        if(expensegraph.find(uid)!=expensegraph.end())
        {
        	//cout<<"inside find"<<endl;
            te=expensegraph[uid];
            if(te.size()==0)
             {
                 cout<<"No Balances"<<endl;
                 return;
             }
             for(int i=0;i<te.size();i++)
             {
                 if(te[i]->getAmount()>0)
                 {
                     cout<<te[i]->getToUid()<<" owes "<<uid<<" : "<<te[i]->getAmount()<<endl;
                 }
                 else if(te[i]->getAmount()<0)
                 {
                     cout<<uid<<" Owes "<<te[i]->getToUid()<<" : "<<-te[i]->getAmount()<<endl;
                 }
             }
        }
        else
        {
            cout<<"No balances"<<endl;
        }

            cout<<endl;
       }  
       
      void show()
      {
         
          vector<Expense *> te;
          map<int,vector<Expense *>> :: iterator it;
          if(expensegraph.size()==0)
          {
              cout<<"No Balances"<<endl;
              return ;
          }
          for(it=expensegraph.begin();it!=expensegraph.end();++it)
          {
              te=it->second;
              if(te.size()==0)
             {
                 cout<<"No Balances"<<endl;
                 return;
             }
             for(int i=0;i<te.size();i++)
             {
                 if(te[i]->getAmount()>0)
                 {
                     cout<<te[i]->getToUid()<<" owes "<<it->first<<" : "<<te[i]->getAmount()<<endl;
                 }
                
             }
          }
          cout<<endl;

      } 

};

ExpenseManager *ExpenseManager::INSTANCE=0; 
int main()
{

   UserManager *usermanager = usermanager->getinstance();
    usermanager->adduser(1,"manish","manish@gmail.com","1234567890");
    usermanager->adduser(2,"ankush","ankush@gmail.com","2345678901");
    usermanager->adduser(3,"anil","anil@gmail.com","567890123");
    usermanager->adduser(4,"sunil","sunil@gmail.com","3456789012");
    ExpenseManager *em= em->getinstance();
  string command;
  string type;
  double totalexpense;
  int fromuser;
  int touser[1000];
  int tousersize;
  double exact[1000];
  int showuser;
    while(1)
    {
        cin>>command;

        if(command.compare("EXPENSE")==0)
        {
          cin>>fromuser;
          cin>>totalexpense;
          cin>>tousersize;
          for(int i=0;i<tousersize;i++)
          {
              cin>>touser[i];
          }
          cin>>type;
          if(type.compare("EXACT")==0)
          {
              for(int i=0;i<tousersize;i++)
              {
                  cin>>exact[i];
              }
              cout<<em->addexpense(fromuser,totalexpense,touser,tousersize,exact,type)<<endl;
          }
          else if(type.compare("EQUAL")==0)
          {
              cout<<em->addexpense(fromuser,totalexpense,touser,tousersize,NULL,type)<<endl;
          }

          else
          {
              cout<<"type not supported"<<endl;
          }
          

        }

        else if(command.compare("SHOW")==0)
        {
              em->show();
        }
        else if(command.compare("SHOWUSER")==0)
        {
             cin>>showuser;
             em->show(showuser);

        }
        else if(command.compare("EXIT")==0)
        {
            break;
            
        }
        else
        {
            cout<<"command not supported"<<endl;
        }
        
        
    }
    
    
    return 0;
}
