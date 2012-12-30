
b=1
for(a=1,a<6,a=a+1){
	b=b*a
}

def _H(){
	def b(){ return 10 }
	global a,b
	tmp_a=a
	a=b
	b=tmp_a
	local b,a
	return b()+a
}


a="ciao"
b=_H()