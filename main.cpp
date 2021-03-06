#include<iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<stack>
#include<stdio.h>
#include "c_char.h"
using namespace std;
char pri[10][10]={
//顶   前+  -   *   /   ^   !   (   )   \0   %
/* + */	'>','>','<','<','<','<','<','>','>','<',
/* - */	'>','>','<','<','<','<','<','>','>','<',
/* * */ '>','>','>','>','<','<','<','>','>','<',
/* / */ '>','>','>','>','<','<','<','>','>','<',
/* ^ */ '>','>','>','>','>','<','<','>','>','<',
/* ! */ '>','>','>','>','>','>','#','>','>','<',
/* ( */ '<','<','<','<','<','<','<','=','#','<',
/* ) */ '#','#','#','#','#','#','#','#','#','#',
/* \0*/ '<','<','<','<','<','<','<','#','=','<',
/* % */ '>','>','>','>','>','>','#','>','>','>'
};
int main(){
	while(true){
		bool first = false;
		string s;
		int last=-2;
		int point = 0;
		bool is_has = false;	
		stack<double> snum;
		stack<c_char> schar;
		double count = 1;
		try{
			cin >> s;
			c_char end('\0');
			schar.push(end);
			while(point<=s.length()){
				if(('0'<=s[point]&&s[point]<='9')){
					if(last==point-1&&is_has==false){
						double a = snum.top();
						snum.pop();
						snum.push(a*10+s[point]-'0');
					}else if(is_has){
						double a = snum.top();
						snum.pop();
						count = count * 0.1;
						snum.push(a+(s[point]-'0')*count);
					}else{
						snum.push(s[point]-'0');
					}
					last = point;
				}else if(s[point]=='.'){
					if(snum.empty()) throw "不能不输入数";
					if(is_has) throw "不能有两个小数点";
					is_has = true; 
					last = point;
					count = 1;
				}else{
					if(first == true&&s[point-1]>='0'&&s[point-1]<='9'){
						double mi = snum.top();
						snum.pop();
						mi = -mi;
						snum.push(mi);
					}
					first = false;
					is_has = false;
					c_char now(s[point]);
					if(snum.empty()&&now.this_char!='('&&now.this_char!='-') throw "不能不输入数"; 
					if(now.this_char=='-'&&(last!=point-1||snum.empty()==true)){
						first = !first;
						point++;
						continue;
					}
					if(now.this_char!='('&&s[point-1]!=')'&&s[point-1]!='!'&&!(s[point-1]>='0'&&s[point-1]<='9')){
						throw "算式不合法";
					}
					//核心，判断优先级
					//pri的数组的第一个括号，是栈顶数据，第二个，是当前数据
					//如果栈顶数据和当前数据的符号一样或者低，就说明栈内可以进行运算(>)
					//如果当前数据的优先级比栈顶符号高，就说明还不能运算，需要等到这个式子出现(<)
					//更低的优先级的符号才可以一次计算
					//如果栈顶数据为'\0',就说明当前栈内没有数据(这个为一开始存储的end对象)(<)
					//如果栈顶为(并且当前为)说明括号内的东西已经算完了(=),就把这个(弹出就好了 
					switch(pri[schar.top().type][now.type]){
						case '<':
							schar.push(now);
							break; 
						case '=':
							schar.pop();
							break;
						case '>':
							while(pri[schar.top().type][now.type]=='>'){
								double q = snum.top();
								snum.pop();
								double b;
								switch(schar.top().type){
									case 0:
										b = snum.top();
										snum.pop();
										snum.push(q+b);
										break;
									case 1:
										b = snum.top();
										snum.pop();
										snum.push(b-q);
										break;
									case 2:
										b = snum.top();
										snum.pop();
										snum.push(q*b);
										break;
									case 3:
										b = snum.top();
										snum.pop();
										if(q==0){
											throw "除数不为零";
										}
										snum.push(b/q);
										break;
									case 4:
										b = snum.top();
										snum.pop();
										snum.push(pow(b,q));
										break;
									case 5: 
										b = 1;
										if(int(q)!=q) throw "小数不能阶乘,我说的"; 
										for(int i =1;i<=q;i++){
										b = b*i;
										}
										snum.push(b);
										break;
									case 9:
										if(int(q)!=q||int(b)!=b) throw "小数不能取余";
										b = snum.top();
										snum.pop();
										snum.push(int(b)%int(q));
										break;
									case 6:
									case 7:
									case 8:
										break;
									}
								schar.pop();
								if(now.type==7&&schar.top().type==6){
									schar.pop();
								} 
									 
							}
							if(now.this_char!=')'){
								schar.push(now);
							}
						}
					}
				point++;
			}
			cout << snum.top()<<endl;
			system("PAUSE");
		}catch(const char* msg){
			cout << msg<<" 按#退出,按其他键重来!(这个字符不参与运算)";
			char a; 
			cin >> a;
			if(a=='#'){
				break;
			}else{
				continue;
			}
		}
	}
	return 0;
}
