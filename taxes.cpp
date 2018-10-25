#include <iostream>

using namespace std;

int singletax(int i)
{
  int t=0; // variable to record taxes.
  int i_bracket[7] = {0, 9275, 37650, 91150, 190150, 413350, 415050};
  double t_bracket[7] = { .10, .15, .25, .28, .33, .35, .396};
  int rollover[6] = {927, 5183, 18558, 46278, 119934, 120529};

  if (i >= i_bracket[0] && i <=i_bracket[1]){
    t = i*t_bracket[0];
  }
  else if (i > i_bracket[1] && i <=i_bracket[2]){
    t = rollover[0] + (i-i_bracket[1])*t_bracket[1];
  }
  else if (i > i_bracket[2] && i <=i_bracket[3]){
    t = rollover[1] + (i-i_bracket[2])*t_bracket[2];
  }
  else if (i > i_bracket[3] && i <=i_bracket[4]){
    t = rollover[2] + (i-i_bracket[3])*t_bracket[3];
  }
  else if (i > i_bracket[4] && i <=i_bracket[5]){
    t = rollover[3] + (i-i_bracket[4])*t_bracket[4];
  }
  else if (i > i_bracket[5] && i <=i_bracket[6]){
    t = rollover[4] + (i-i_bracket[5])*t_bracket[5];
  }
  else if (i > i_bracket[6]){
      t = rollover[5] + (i-i_bracket[6])*t_bracket[6];
  }
  return t;
}

int jointtax(int i)
{
  int t = 0;
  int i_bracket[7] = {0, 18550, 75300, 151900, 231450, 413350, 466950};
  double t_bracket[7] = { .10, .15, .25, .28, .33, .35, .396};
  int rollover[6] = {1855, 10367, 29517, 51791, 111818, 130578};

  if (i >= i_bracket[0] && i <=i_bracket[1]){
    t = i*t_bracket[0];
  }
  else if (i > i_bracket[1] && i <=i_bracket[2]){
    t = rollover[0] + (i-i_bracket[1])*t_bracket[1];
  }
  else if (i > i_bracket[2] && i <=i_bracket[3]){
    t = rollover[1] + (i-i_bracket[2])*t_bracket[2];
  }
  else if (i > i_bracket[3] && i <=i_bracket[4]){
    t = rollover[2] + (i-i_bracket[3])*t_bracket[3];
  }
  else if (i > i_bracket[4] && i <=i_bracket[5]){
    t = rollover[3] + (i-i_bracket[4])*t_bracket[4];
  }
  else if (i > i_bracket[5] && i <=i_bracket[6]){
    t = rollover[4] + (i-i_bracket[5])*t_bracket[5];
  }
  else if (i > i_bracket[6]){
      t = rollover[5] + (i-i_bracket[6])*t_bracket[6];
  }
  return t;
}

int seperatelytax(int i)
{
  int t=0;
  int i_bracket[7] = {0, 9275, 37650, 75950, 115725, 206675, 233475};
  double t_bracket[7] = { .10, .15, .25, .28, .33, .35, .396};
  int rollover[6] = {927, 5183, 14758, 25895, 55909, 65289};

  if (i >= i_bracket[0] && i <=i_bracket[1]){
    t = i*t_bracket[0];
  }
  else if (i > i_bracket[1] && i <=i_bracket[2]){
    t = rollover[0] + (i-i_bracket[1])*t_bracket[1];
  }
  else if (i > i_bracket[2] && i <=i_bracket[3]){
    t = rollover[1] + (i-i_bracket[2])*t_bracket[2];
  }
  else if (i > i_bracket[3] && i <=i_bracket[4]){
    t = rollover[2] + (i-i_bracket[3])*t_bracket[3];
  }
  else if (i > i_bracket[4] && i <=i_bracket[5]){
    t = rollover[3] + (i-i_bracket[4])*t_bracket[4];
  }
  else if (i > i_bracket[5] && i <=i_bracket[6]){
    t = rollover[4] + (i-i_bracket[5])*t_bracket[5];
  }
  else if (i > i_bracket[6]){
    t = rollover[5] + (i-i_bracket[6])*t_bracket[6];
  }
  return t;
}

int main()
{
  int i = 0;
  int q1 = 0;
  int q2 = 0;

  cout << "How much money did you make in 2016? $";
  cin >> i;
  cout << "First things first, are you married?" << endl
  <<  "If married, type 0. If single, type 1." << endl;
  cin >> q1;

  if (q1 == 0){
    cout << "Are you filing jointly or seperately?" << endl
    << "If jointly, type 0. If seperately, type 1." << endl;
    cin >> q2;
    if (q2 == 0){
      cout << "This year, you together owe $" << jointtax(i) << " in taxes."
      << endl;
      return 0;
    }
  }
  if (q2 == 1){
    cout << "This year, you alone owe $" << seperatelytax(i) << " in taxes."
    << endl;
    return 0;
  }

  if (q1 == 1){
    cout << "This year, you alone owe $" << singletax(i) << " in taxes."
    << endl;
    return 0;
  }
  else{
  cout << "Error.";
  return 0;}
}
