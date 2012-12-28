
def a(x){ 	
	global a
	a=5+x
	tmp_a=a
	def l(){
		return ("ciao")
	}
	if(a>9){ return(l()) }
	local a
	return (a(tmp_a))
}


a=a(0)

