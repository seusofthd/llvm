int main(){
	int a = 2;
	int b = 1;
	int *c = &a;
	int **d = &c;
	return **d;
}
