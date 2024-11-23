#include <iostream>
#include <stack>
#include <cctype>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;
//��������Ŀ
// �Ѿ�֧�ּ��׵ļӼ��˳���2024/11/22��
// ����ʽ�Ļ��������δ�ﵽ��2024/11/23���ﵽ
// �������ȼ���δ֧�֣�2024/11/23���ﵽ


// ��������
double applyOp(double a, double b, char op);
bool isOperator(char c);
bool isHigherPrecedence(char op1, char op2);//�Ƚϼ�����

// ������
stack<double> values;//�����ֵ��ջ
stack<char> ops;//��Ų�������ջ
int main() {
   
    string expression;//�������ʽ

    cout << "Enter an expression: ";//��ʾ��Ϣ
    getline(cin, expression);//�õ�һ�а����ո���ַ���
    //�����������滻
    //(����)replace(expression.begin(), expression.end(), '��', '(');
    for (char& c : expression) {
        if (c == '��') {
            c = '(';
        }
        else if (c == '��') {
            c = ')';
        }

    }

    int i = 0;
    bool isNegative = false;//������־
    int hasleft = 0;//�Ƿ���������
    double value = 0;
    //��ʼ���������ַ���
    while (i < expression.length()) 
    {//�ǿհ׷��ž�����
        if (isspace(expression[i])) {
            // ���Կհ�
            i++;
            continue;
        }
        //�ж��Ƿ�Ϊ���ֻ��߸���
        if (isdigit(expression[i]) || (expression[i] == '-' )) 
        {
                     
            //����Ǹ��ţ��ж�ǰһλ�Ƿ�Ϊ�����ţ��հף����ǲ�����
             isNegative = (expression[i] == '-' && (i == 0 || expression[i - 1] == '(' || isspace(expression[i - 1]) || isOperator(expression[i - 1])));
            if (isNegative) {//ȷ��������ǰ�ķ���
                i++;//��һλ  
                continue;//��Ϊ����һλ�ˣ���Ҫ�����ж�����
            }
            //���������,�ۼӽ��������С����
            else 
            {
                value = value * 10 + (expression[i] - '0');
                //ֻҪ��һλ��Ч��������
                while (i + 1 < expression.length() && isdigit(expression[i + 1])) 
                {//�ۼ�
                    i++;
                    value = value * 10 + (expression[i] - '0');
                }
                //�����һλ��С����
                if (i + 1 < expression.length() && expression[i + 1] == '.') 
                {//����һλ
                    i++;
                    double decimal = 0.1;
                    //ֻҪ��һλ��Ч�������־��ۼ�
                    while (i + 1 < expression.length() && isdigit(expression[i + 1])) 
                    {                       
                        value = value + (expression[i + 1] - '0') * decimal;
                        decimal *= 0.1;
                        i++;
                    }
                    
                }
               
            }
             //ִ�н������������һλ��Ч����      
            if (isNegative) {//����Ǹ�ֵ��ȡ��
                value = -value;
                isNegative = false;
            }
            //��ֵѹ��ջ��
            values.push(value);
            //�������֣���ʼ��һλ���ֵ��ۼ�
            value = 0.0;
            i++;
            //������һλ��ʼ�ж�
            continue;
        }
        // ����������
        else if (expression[i] == '(') {
            hasleft++;
            ops.push(expression[i]);
            i++;
        }
        // ����������
        else if (expression[i] == ')') 
        {//�����߼����ҵ���һ������������ţ����Ҽ��������ڵ�ֵ  
            //���ջΪ�ջ�ջ��Ϊ�����ţ�������û�����ݣ�������
            if (ops.empty() || ops.top() == '(') {
                i++;
                continue;
            }
            //���ջ��������������ֵջ����ֵ
            // ����������ڵ�ֵ
            if (hasleft && (!values.empty()))
            {//һֱ����ֱ������������
                while ((!ops.empty()) && ops.top() != '(' && (values.size() >= 2))
                {
                    //ȡ��ջ��������ֵ����һ��������                
                    double val3 = values.top(); values.pop();
                    double val2 = values.top(); values.pop();
                    char op2 = ops.top(); ops.pop();        
                    //�жϵ�ǰ���ź�ջ���������ȼ�
                    if ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
                    {
                        //������ȼ���ͽ������Ľ��ѹ��ջ��
                        values.push(applyOp(val2, val3, op2));
                    }
                    //������ȼ���������Ҫ�ȼ���ǰ��λ����ֱ�����ȼ�����
                    else 
                    {                      
                        //ֱ�����㵱ǰ���������ȼ�����ջ��(true)
                        while ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
                        {                          
                            double val1 = values.top(); values.pop();
                            char op1 = ops.top(); ops.pop();
                            val2 = applyOp(val1, val2, op1);
                        }
                        //��������ѹ��ջ��
                        values.push(applyOp(val2, val3, op2));
                    }
                    
                }
                //����������
                if(!ops.empty())
                {
                    ops.pop();
                    hasleft--;
                }
                i++;
                //����һ������
                continue;
            }
            //û��������
            else 
            {
                i++;
                continue;
            }
            
        }
        // �������������
        else if (isOperator(expression[i])) 
        {
            ops.push(expression[i]);
            i++;
            continue;
        }
    }

    //�������ʼ����ֱ��������Ϊ��
    while ((!ops.empty()) && (values.size() >= 2)) {
        //ȡ��ջ��������ֵ����һ��������                
        double val3 = values.top(); values.pop();
        double val2 = values.top(); values.pop();
        char op2 = ops.top(); ops.pop();
        //�жϵ�ǰ���ź�ջ���������ȼ�
        if ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
        {
            //������ȼ���ͽ������Ľ��ѹ��ջ��
            values.push(applyOp(val2, val3, op2));
        }
        //������ȼ���������Ҫ�ȼ���ǰ��λ����ֱ�����ȼ�����
        else
        {
            //ֱ�����㵱ǰ���������ȼ�����ջ��
            while ((!ops.empty()) && (isHigherPrecedence(op2, ops.top())))
            {
                double val1 = values.top(); values.pop();
                char op1 = ops.top(); ops.pop();
                val2 = applyOp(val1, val2, op1);
            }
            //��������ѹ��ջ��
            values.push(applyOp(val2, val3, op2));
        }
    }

    //���ջ����󳬹�һ��ֵ���׳����ʽ�쳣
    if (values.size() != 1) {
        throw invalid_argument("Invalid expression");
    }
    //������
    cout << "The result is " << values.top() << endl;
  // cout << "continue?(1 or 0)" << endl;
   
    return 0;
}

// Ӧ�������
double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw invalid_argument("Cannot divide by zero.");
        return a / b;
   /* case '(':
        ops.pop();
        if(!ops.empty())
            return applyOp(a, b, ops.top());
        return a + b;*/
    }
    throw invalid_argument("Invalid operator");
}

// ����Ƿ��������
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// ������ȼ�
bool isHigherPrecedence(char op1, char op2) {
    if (op2 == '(' || op2 == ')') return false;
    if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) return false;
    return true;
}
