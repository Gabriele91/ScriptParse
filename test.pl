
def scrivi(x,y){
	print(x,y)
}

def fibonacci(i){
	if(i>1){ return fibonacci(i-1)+fibonacci(i-2) }
	if(i<0){ return -1 }
	return i
}

for(,ex!="si",){
	val=input("scrivi un numero: ")
	fib=fibonacci(toint(val))
	scrivi("fib=",fib)
	ex=input("vuoi uscire?")
}

/*
//Esempio, scrivi su file:
file=openfile("file_test2.txt","w")
	writefile(file,y)
closefile(file)
*/