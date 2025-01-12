#include <iostream>


int temp(){
	using namespace std;
	int row, col;
	while(true){
		cin>> col >> row;
		if (col < 0) col +=20;
		else col += 19;
		if(row < 0) row += 20;
		else row += 19;
		cout << col <<"    " << row <<endl;
	}
	return 0;
}
