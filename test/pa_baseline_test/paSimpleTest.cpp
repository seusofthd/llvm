int main(){
	int a = 2;
	int c = 2;
	int b = 3;
	int *x = &a;
	int *y = &b;
	x = y;
	a = *y;
	*x = a;
	return 0;
}
