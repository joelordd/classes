// Audrey Cards 005980230
// CSE 330 Summer 2018
// Dr. Murphy 


#include <iostream>
#include "unistd.h"

using namespace std;


class Time
{
   public:
   Time(int h, int m);
   int gethours() const;
   int getminutes() const;
   void addminute();
   void subtractminute();
   void addhour();
   void subtracthour();
   bool operator==(Time t);
   void operator++(); //prefix
   void operator++(int); //postfix
   void operator--(); //prefix
   void operator--(int); //postfix
   private :
   int hours;
   int minutes;
};

Time::Time(int h, int m)
{ hours=h;
  minutes=m;
}

int Time::gethours() const
{ return hours;
}


int Time::getminutes() const
{ return minutes;
}

bool Time::operator==(Time t)
{
    if (this->gethours() != t.gethours()) return false;
   if (this->getminutes() != t.getminutes()) return false;
   return true;
 }

 void Time::operator++()
 {
   this -> addminute();
 }

 void Time::operator++(int)
 {
   this -> addminute();
}

 void Time::addminute()
 { minutes++;
   if (minutes == 60 )
     {
       minutes=0;
       hours++;
       }
    if (hours == 13)
       hours=1;
 }

 void Time::operator--()
 {
         this -> subtractminute();
 }

 void Time::subtractminute()
 { minutes--;
         if (minutes == -1 )
         {
                 minutes=59;
                 hours--;
         }
         if (hours == 0)
                 hours=12;
 }
 
 void Time::addhour()
 { hours++;
        if(hours == 13 )
        {
                hours=1;
        }
 }

void Time::subtracthour()
 { hours--;
        if(hours == 0 )
        {
                hours=12;
        }
 }

 int main()
 {
 
    bool stop = false;
    Time Morning= Time(6,0);
    
    while(!stop) {
        if(Morning.getminutes() == 0)
            cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << "0" << endl;
        else if(Morning.getminutes() > 0 && Morning.getminutes() <= 9)
                cout << "The time is: " << Morning.gethours() << ":0" << Morning.getminutes() << endl;
        else
            cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << endl;
        
        int choose = 0;
        cout << "What do you want to do?\n";
        cout << "1. Add minutes\n";
        cout << "2. Subtract minutes\n";
        cout << "3. Add hours\n";
        cout << "4. Subtract hours\n";
        
        cin >> choose;
    
        if(choose == 1) {
            int x = 0;
            cout << "How many minutes do you want to add?\n";
            cin >> x;
            
            for(; x > 0; x--) {
                Morning.addminute();
            }
            
            //Added the else if, if the number is 1-9 it will output a zero in front of the number.
            //The '&&' is important, it means the number MUST be between 1-9 for the statement to be true.
            if(Morning.getminutes() == 0)
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << "0" << endl;
            else if(Morning.getminutes() > 0 && Morning.getminutes() <= 9)
                cout << "The time is: " << Morning.gethours() << ":0" << Morning.getminutes() << endl;
            else
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << endl;
        }   
        else if(choose == 2) {
            int x = 0;
            cout << "How many minutes do you want to subtract?\n";
            cin >> x;
            
            for(; x > 0; x--) {
                Morning.subtractminute();
            }
            
            if(Morning.getminutes() == 0)
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << "0" << endl;
            else if(Morning.getminutes() > 0 && Morning.getminutes() <= 9)
                cout << "The time is: " << Morning.gethours() << ":0" << Morning.getminutes() << endl;
            else
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << endl;
        
        }
        else if(choose ==3) {
            int x = 0;
            cout << "How many hours do you want to add?\n";
            cin >> x;
            
            for(; x > 0; x--) {
                Morning.addhour();
            }
            
            if(Morning.getminutes() == 0)
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << "0" << endl;
            else if(Morning.getminutes() > 0 && Morning.getminutes() <= 9)
                cout << "The time is: " << Morning.gethours() << ":0" << Morning.getminutes() << endl;
            else
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << endl;
        }
        else if (choose ==4) {
            int x = 0;
            cout << "How many hours do you want to subtract?\n";
            cin >> x;
            
            for(; x > 0; x--) {
                Morning.subtracthour();
            }
            
            if(Morning.getminutes() == 0)
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << "0" << endl;
            else if(Morning.getminutes() > 0 && Morning.getminutes() <= 9)
                cout << "The time is: " << Morning.gethours() << ":0" << Morning.getminutes() << endl;
            else
                cout << "The time is: " << Morning.gethours() << ":" << Morning.getminutes() << endl;
        }
        
        char answer = ' ';
        cout << "Do you want to continue?(y/n)\n";
        cin >> answer;
        
        if(answer == 'n') stop = true;
        cout << endl << endl;
    } 
}  
    