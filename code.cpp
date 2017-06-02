#include<iostream>
#include<string.h>
#include<cstdio>

using namespace std;


class PostfixExpression;     // future declaration of class PostfixExpression..and InfixExpression
class InfixExpression;       //   has to be used as an argument in a friend function --  Infix_to_Postfix()


int return_priority(char oprtr)        
{                                // to put postfix expression according to BODMAS.. returning priority..
   switch(oprtr)
    { 
      case '+':
      case '-':return 1;
               break;
      case '/':
      case '*':return 2;
               break;
      case '}':
      case ']':
      case ')':return -1;
                break;
      case '{':
      case '[':
      case '(':return 0;
                break;
      default : return 0;
    }
}

class Stack_Element
{
   
   char data;
   double oprand;
   Stack_Element * down;
   public:
   friend class Expression_vaidity;
   friend class Stack;
   friend void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y);
};


class Stack
{
   Stack_Element *top, *current;
   bool operatorORoprand;                     // 1 if Stack is of characters i.e. opreators.
                                              // and 0 if stack is of numaric values i.e. oprand.
  
    public:    
   
    friend void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y);  
                                 
     Stack()
      {
         top = current = NULL;
      }
     void OperatorOrOprandStack (bool X) 
     {
       operatorORoprand = X;
     }
     bool Is_Empty (void)
     {
         if(top==NULL)
            return 1;
         else
            return 0;
     }

     template <typename Type>
     Type Give_top(void)
     {
         if(operatorORoprand)
            return top->data;
         else
            return top->oprand;
     }
   
     template <typename Type>
     void Push(Type T)
     {
        Stack_Element *Disc = new Stack_Element;
        if(operatorORoprand)
           Disc->data = T;
        else
           Disc->oprand = T;
        Disc->down = top;
        top = Disc;
     }
      
     template <typename Type>
     Type Pop(void)
     {
         double T2;
         char T1;
         if(operatorORoprand)
             T1 = top->data;
         else
             T2 = top->oprand;
         Stack_Element *temp = top;
         top = top->down;
         delete temp;
         return (operatorORoprand ? T1 : T2);
     }
};



/////////////////////////////////////////////////////////////////////////////////////////////////
//////////  Class to check validity of expression if any syntax error... throw exception....
/////////////////////////////////////////////////////////////////////////////////////////////////
class Expression_vaidity
{
   Stack InfixExpressionStack;
   public:
    void check_char(char curr)
    {
       InfixExpressionStack.OperatorOrOprandStack(1); // declaring stack as character stack..
       bool flag=1;
       switch(curr)
        {
          case '(':flag=0;
		   InfixExpressionStack.Push<char>(curr);
		   break;
          case ')':flag=0;
		   valid_close(curr);
	           break;
          case '{':flag=0;
		   InfixExpressionStack.Push<char>(curr);
		   break;
          case '}':flag=0;
		   valid_close(curr);
	           break;
	  case '[':flag=0;
		   InfixExpressionStack.Push<char>(curr);
		   break;
	  case ']':flag=0;
		   valid_close(curr);
	           break;
          case '.':cout<<"\n\t Decimal numbers are not allowed..";
                   throw curr;
                   break;
          case '+':
          case '-':
          case '*':
          case '/':flag=0;
		    break; 
          case '\0':flag=0;
		   if(!InfixExpressionStack.Is_Empty())
 			throw curr; 
          default :break;       
	}
        if(isdigit(curr))
           flag=0;
  
        if(flag)
         throw curr; 
    }
    void valid_close (char curr)
    {
      if(InfixExpressionStack.Is_Empty())
       throw curr;
      
      switch(curr)
        {
          case '}': if(InfixExpressionStack.Give_top<char>()=='{')
		      {
			InfixExpressionStack.Pop<char>();
		      }
		    else
			throw curr;
		    break;
          case ')': if(InfixExpressionStack.Give_top<char>()=='(')
		      {
			InfixExpressionStack.Pop<char>();
		      }
		    else
			throw curr;
		    break;
          case ']': if(InfixExpressionStack.Give_top<char>()=='[')
		      {
			InfixExpressionStack.Pop<char>();
		      }
		    else
			throw curr;
		    break;
	}   
            
    }

   
};



///////////////////////////////////////////////////////////////////////////////////////////////////
//// Class to validate and store an arithmatical Infix expression in the form of double linked list
///////////////////////////////////////////////////////////////////////////////////////////////////
class Expression_element
{
  bool operatorORoprand;                     // 1 for operator & 0 for oprand.
  char opertor;
  double oprand;
  Expression_element *right, *left;
  public:
   Expression_element()
    {
      right = left = NULL;   
    }
  friend class InfixExpression;
  friend class PostfixExpression;
  friend void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y);
};


class InfixExpression
{
  string Exp;
  Expression_element *first, *last, *current;
  public:
   InfixExpression()
   {
     first = last = current = NULL;
   }

   void getdata(void)
     {
       cout<<"\n\t Enter your Expression(Do NOT use white spaces): ";
       cin>>Exp;
       int temp_num=0;
       Expression_vaidity check_validity;
       int len = Exp.length();
       int operator_occerance = 0;
           for(int j=0; j<=len ; j++)            //to check if any opretor is placed 
            {                                    //  on first or last position  
                                                 // '-' is allowed on first position
               
                  switch(Exp[j])
                  { 
                    case '/':
                    case '*':if(j==0|| j==len-1)
                              throw Exp[j];
                             if(++operator_occerance>1)       //if two cocecutive operators occur
                              throw Exp[j];                   // throw an exception
			     break;                           // but dont throw if 2nd occured operator 
                    case '+':                                 // is '+' '-'.. as it could show next number +ve or -ve.
                    case '-':if(j==len-1)
                              throw Exp[j];
                             if(++operator_occerance>2)
                              throw Exp[j];
                              break;
                    case '{':
                    case '[':
                    case '(':
                    case ')':
                    case ']':
                    case '}': break;
		    default: operator_occerance = 0;
                  }
                
                
               check_validity.check_char(Exp[j]); // function of class Expression_vaidity
            }                                     //  to check if expression is invalid then throw execption

       for(int i=0; Exp[i]!='\0' ; i++)
        {


          Expression_element *node = new Expression_element();
          if(!isdigit(Exp[i]))
           {
             
                switch(Exp[i])
                 {
                    case '{':
                    case '[':
                    case '(':if(isdigit(Exp[i-1])&&(i!=0))
                             {
                               cout<<"\n\t Please place operators properly in the priphery of '"<<Exp[i]<<"'\n";
                               throw Exp[i];
 			     }
                    case ')':
                    case ']':
                    case '}':
                               node->operatorORoprand = 1;
                              node->opertor = Exp[i];
				goto Link_to_store_data;       //store the parantheses directly in linked list
                             
                    default: break;
                 }
            
             
             if(isdigit(Exp[i-1])&& (i!=0))
             {
 		node->operatorORoprand = 1;
                node->opertor = Exp[i];
             }
             if(!isdigit(Exp[i-1]))
             {
              if(return_priority(Exp[i-1])>=0  && isdigit(Exp[i+1]))
               {
                 if(Exp[i]=='+')
                 {  i++;  
                    for(temp_num=0 ; isdigit(Exp[i]) ; i++, temp_num*=10 )
                      temp_num += ( (int)Exp[i] ) - 48; 
                   i--;
                 }
                 else if(Exp[i]=='-')
                 {   i++;
                    for(temp_num=0 ; isdigit(Exp[i]) ; i++, temp_num*=10 )
                      temp_num += ( (int)Exp[i] ) - 48;
		    temp_num *= -1;
                    i--; 
                 }
                node->operatorORoprand = 0;
                temp_num/=10;
                node->oprand = temp_num;
              }
              else
               { 
                  node->operatorORoprand = 1;
                  node->opertor = Exp[i];
		}
            }
           }
          else
	   {
             for(temp_num=0 ; isdigit(Exp[i]) ; i++, temp_num*=10 )
               temp_num += ( (int)Exp[i] ) - 48;

             i--; 
             temp_num/=10;

             node->operatorORoprand = 0;
             node->oprand = temp_num;
	   }

Link_to_store_data:          // link to get jumped from above switch case..
          
           if(last==NULL)
            {
	      last=node;
	      first=node;
	    }
	  last->right = node;
          node->left = last;
          last = node;
	}
     }
 
   friend void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y);

   void putdata(void)
    {
      cout<<"\nInfix Expression is: ";
      current = first;
      while(current!=NULL)
       	{
          if(current->operatorORoprand)
	    cout<<current->opertor;
          else
            cout<<current->oprand;
  
          current = current->right;	
	}
	cout<<endl<<endl;
    }
};



/////////////////////////////////////////////////////////////////////////////////////////////////////
///////  Class to store POSTFIX arithmatic expression and evaluate it
/////////////////////////////////////////////////////////////////////////////////////////////////////
class PostfixExpression
{   
  Stack Result;
  Expression_element *first, *last, *current;
  double Answer;
  public:
   PostfixExpression()
    {
     first = last = current = NULL;
     Result.OperatorOrOprandStack(0);            // Declaring Result Stack as number stack.. i.e. Oprand stack..
    }
   friend void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y);  
   
   template <typename Type>
   void Add_Elementat_Last(Type T)
   {
     Expression_element *New_element = new Expression_element; 
      if(sizeof(T)==1)
      {
        New_element->operatorORoprand = 1;
        New_element->opertor = T;
      }
      else
      {
        New_element->operatorORoprand = 0;
        New_element->oprand = T;
      }
     if(first==NULL)
      {
        first=New_element;
        last=New_element;
      } 
     else
      {
        New_element->left = last;
        last->right = New_element;
        last = New_element;
      }
   }

    void putdata(void)
    {
      cout<<"\n Postfix Expression is: ";
      current = first;
      while(current!=NULL)
       	{
          if(current->operatorORoprand)
	    cout<<current->opertor;
          else
            cout<<"("<<current->oprand<<")";
  
          current = current->right;	
	}
	cout<<endl<<endl;
    }

    void ShowResult (void)
    {
       cout<<"\n\t Result of given expression is: ";
       cout<<Answer;
    }

    void Evaluate(void)
    {
       double num1;
       double num2;
       char Operator;
       current = first;
       while(current != NULL)
        {
 	  if(current->operatorORoprand)
           {
              num2 = Result.Pop<double>();
              num1 = Result.Pop<double>();
              switch(current->opertor)
		{
 		   case '+':num1 += num2;
			    break;
 		   case '-':num1 -= num2;
			    break;
 		   case '*':num1 *= num2;
			    break;
 		   case '/':if(num2==0)
				throw 1;
                            num1 /= num2;
			    break;
		}
              Result.Push<double>(num1);
           }
           else
           {
              
              Result.Push<double>(current->oprand);
              
           }
           current = current->right;
        }
        Answer = Result.Pop<double>();
    }

};



/////////////////////////////////////////////////////////////
// friend Function to convert Infix expression to postfix
/////////////////////////////////////////////////////////////

void Infix_to_Postfix  (InfixExpression & X, PostfixExpression & Y)
{
    Stack OperatorsStack;

    OperatorsStack.OperatorOrOprandStack(1);         // Declaring OperatorsStack as character stack.. i.e. Operator stack..
    X.current = X.first;

    while(X.current!=NULL)
    {
      char temp_char;
      char curr_oprtr = X.current->opertor;
      
      if(X.current->operatorORoprand)
      {
         if(OperatorsStack.Is_Empty())
          {
            OperatorsStack.Push<char>(curr_oprtr);
          }
         else if(return_priority(curr_oprtr)<=0)    
           { 
             switch(curr_oprtr)
             {
		case '}':
		case ']':
		case ')':temp_char = OperatorsStack.Pop<char>();
			  while(return_priority(temp_char)!=0 )
			  {
			     Y.Add_Elementat_Last<char>(temp_char);
			     temp_char = OperatorsStack.Pop<char>();
                             
			  }  
			  break;
		case '{':
		case '[':
		case '(': OperatorsStack.Push<char>(curr_oprtr);
                          
			  break;     
	     }
           }
         else if(return_priority(OperatorsStack.Give_top<char>())<=0)
           {
              
	      OperatorsStack.Push<char>(curr_oprtr);
              
	   }
         else if(return_priority(curr_oprtr) < return_priority(OperatorsStack.Give_top<char>()))
         {
           Y.Add_Elementat_Last<char>(OperatorsStack.Pop<char>());
           if(return_priority(curr_oprtr) == return_priority(OperatorsStack.Give_top<char>()))
            {
              Y.Add_Elementat_Last<char>(OperatorsStack.Pop<char>());
              OperatorsStack.Push<char>(curr_oprtr); 
                    
             }
	 }
         else if(return_priority(curr_oprtr) == return_priority(OperatorsStack.Give_top<char>()))
         {             
            Y.Add_Elementat_Last<char>(OperatorsStack.Pop<char>());
            OperatorsStack.Push<char>(curr_oprtr); 
		         
         }
         else
         {
 	    OperatorsStack.Push<char>(curr_oprtr);
		
         }
         
      }
      else
      {
        
         Y.Add_Elementat_Last<int>(X.current->oprand);
      }

      X.current = X.current->right;
    }
    while(!OperatorsStack.Is_Empty())
     {
           Y.Add_Elementat_Last<char>(OperatorsStack.Pop<char>());
     }
}



/////////////////////////////////////////////////////////////////////////////////////////////////
////   Main function..
/////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
  InfixExpression Infix;
  PostfixExpression Postfix;
  try
  {
    Infix.getdata();
  }
  catch (char)
  { 
     cout<<"\n\t Sorry.. Given expression is invalid\n";
     return 0;
  }
  Infix.putdata(); 
   
  Infix_to_Postfix(Infix, Postfix);
  
  Postfix.putdata();

  try
    {
       Postfix.Evaluate();
    }
  catch (int)
    {
       cout<<"\n\t Sorry.. Given expression contains a divide by zero condition\n";
       return 0;
    }
  Postfix.ShowResult();
  cout<<endl<<endl;
  return 0;
}
