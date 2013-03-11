
def fibonacci(i){
	if(i>1){ return fibonacci(i-1)+fibonacci(i-2) }
	if(i<0){ return -1 }
	return i
}

i=10
c=fibonacci(i)
print("fibonacci("+i+")->"+c)
system("pause")

file=openfile("file_test.txt","w")
	writefile(file,"fibonacci("+i+")->"+c)
closefile(file)
system("pause")

y=""
for(a=1,a<c,a=a+1){ 
		y=y+def(x){ 
					if(toint(x/2)==x/2){ 
						return ("uguali:"+toint(x/2)+","+x/2)
					} 
					return "diversi:"+toint(x/2)+","+x/2
				  }(a) 
				  +"\n"
}
file=openfile("file_test2.txt","w")
	writefile(file,y)
closefile(file)
system("pause")