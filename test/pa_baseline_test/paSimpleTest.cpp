int main(){
	int a = 2;
	int c = 2;
	int b = 3;
	int *x = &a;
	int **y = &x;
	int *z = &b;
	z = *y;
	*y = x;
	x = z;
	//	*x = a;
	return 0;
}
