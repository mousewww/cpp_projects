#include <iostream>
#include <stack>
#include <cctype>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;
//计算器项目
// 已经支持简易的加减乘除（2024/11/22）
// 多项式的混合运算尚未达到（2024/11/23）达到
// 括号优先级暂未支持（2024/11/23）达到


// 函数声明
double applyOp(double a, double b, char op);
bool isOperator(char c);
bool isHigherPrecedence(char op1, char op2);//比较级函数

// 主函数
stack<double> values;//存放数值的栈
stack<char> ops;//存放操作符的栈
int main() {
   
    string expression;//声明表达式

    cout << "Enter an expression: ";//提示信息
    getline(cin, expression);//得到一行包括空格的字符串
    //将中文括号替换
    //(报错)replace(expression.begin(), expression.end(), '（', '(');
    for (char& c : expression) {
        if (c == '（') {
            c = '(';
        }
        else if (c == '）') {
            c = ')';
        }

    }

    int i = 0;
    bool isNegative = false;//负数标志
    int hasleft = 0;//是否有左括号
    double value = 0;
    //开始遍历整个字符串
    while (i < expression.length()) 
    {//是空白符号就跳过
        if (isspace(expression[i])) {
            // 忽略空白
            i++;
            continue;
        }
        //判断是否为数字或者负号
        if (isdigit(expression[i]) || (expression[i] == '-' )) 
        {
                     
            //如果是负号，判断前一位是否为左括号，空白，或是操作符
             isNegative = (expression[i] == '-' && (i == 0 || expression[i - 1] == '(' || isspace(expression[i - 1]) || isOperator(expression[i - 1])));
            if (isNegative) {//确定是数字前的符号
                i++;//下一位  
                continue;//因为到下一位了，需要重新判断类型
            }
            //如果是数字,累加结果，处理小数点
            else 
            {
                value = value * 10 + (expression[i] - '0');
                //只要下一位有效且是数字
                while (i + 1 < expression.length() && isdigit(expression[i + 1])) 
                {//累加
                    i++;
                    value = value * 10 + (expression[i] - '0');
                }
                //如果下一位是小数点
                if (i + 1 < expression.length() && expression[i + 1] == '.') 
                {//跳下一位
                    i++;
                    double decimal = 0.1;
                    //只要下一位有效且是数字就累加
                    while (i + 1 < expression.length() && isdigit(expression[i + 1])) 
                    {                       
                        value = value + (expression[i + 1] - '0') * decimal;
                        decimal *= 0.1;
                        i++;
                    }
                    
                }
               
            }
             //执行结束，到达最后一位有效数字      
            if (isNegative) {//如果是负值就取反
                value = -value;
                isNegative = false;
            }
            //将值压入栈中
            values.push(value);
            //重置数字，开始下一位数字的累加
            value = 0.0;
            i++;
            //跳到下一位开始判断
            continue;
        }
        // 处理左括号
        else if (expression[i] == '(') {
            hasleft++;
            ops.push(expression[i]);
            i++;
        }
        // 处理右括号
        else if (expression[i] == ')') 
        {//处理逻辑：找到第一个适配的左括号，并且计算括号内的值  
            //如果栈为空或栈顶为左括号（括号中没有数据）就跳过
            if (ops.empty() || ops.top() == '(') {
                i++;
                continue;
            }
            //如果栈里有左括号且数值栈里有值
            // 则计算括号内的值
            if (hasleft && (!values.empty()))
            {//一直计算直到遇到左括号
                while ((!ops.empty()) && ops.top() != '(' && (values.size() >= 2))
                {
                    //取出栈顶的两个值，和一个操作符                
                    double val3 = values.top(); values.pop();
                    double val2 = values.top(); values.pop();
                    char op2 = ops.top(); ops.pop();        
                    //判断当前符号和栈顶符号优先级
                    if ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
                    {
                        //如果优先级大就将计算后的结果压入栈中
                        values.push(applyOp(val2, val3, op2));
                    }
                    //如果优先级不够就需要先计算前两位数字直到优先级符合
                    else 
                    {                      
                        //直到满足当前操作符优先级高于栈顶(true)
                        while ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
                        {                          
                            double val1 = values.top(); values.pop();
                            char op1 = ops.top(); ops.pop();
                            val2 = applyOp(val1, val2, op1);
                        }
                        //将计算结果压入栈中
                        values.push(applyOp(val2, val3, op2));
                    }
                    
                }
                //弹出左括号
                if(!ops.empty())
                {
                    ops.pop();
                    hasleft--;
                }
                i++;
                //到下一个符号
                continue;
            }
            //没有则跳过
            else 
            {
                i++;
                continue;
            }
            
        }
        // 处理运算操作符
        else if (isOperator(expression[i])) 
        {
            ops.push(expression[i]);
            i++;
            continue;
        }
    }

    //遍历完后开始计算直到操作符为空
    while ((!ops.empty()) && (values.size() >= 2)) {
        //取出栈顶的两个值，和一个操作符                
        double val3 = values.top(); values.pop();
        double val2 = values.top(); values.pop();
        char op2 = ops.top(); ops.pop();
        //判断当前符号和栈顶符号优先级
        if ((!ops.empty()) && isHigherPrecedence(op2, ops.top()))
        {
            //如果优先级大就将计算后的结果压入栈中
            values.push(applyOp(val2, val3, op2));
        }
        //如果优先级不够就需要先计算前两位数字直到优先级符合
        else
        {
            //直到满足当前操作符优先级高于栈顶
            while ((!ops.empty()) && (isHigherPrecedence(op2, ops.top())))
            {
                double val1 = values.top(); values.pop();
                char op1 = ops.top(); ops.pop();
                val2 = applyOp(val1, val2, op1);
            }
            //将计算结果压入栈中
            values.push(applyOp(val2, val3, op2));
        }
    }

    //如果栈内最后超过一个值则抛出表达式异常
    if (values.size() != 1) {
        throw invalid_argument("Invalid expression");
    }
    //输出结果
    cout << "The result is " << values.top() << endl;
  // cout << "continue?(1 or 0)" << endl;
   
    return 0;
}

// 应用运算符
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

// 检查是否是运算符
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// 检查优先级
bool isHigherPrecedence(char op1, char op2) {
    if (op2 == '(' || op2 == ')') return false;
    if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) return false;
    return true;
}
