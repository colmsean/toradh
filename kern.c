#define doSet 1
#define doGet 2


char * mtype[10];
#define max 1000000
int field[max];
int base_obj[max];
int value_obj[max];
int num_set=0;
char * kern_sym[max];
int num_kern_sym=0;
struct instrs {
	int obj;
	int setOrGet;
	int fname;
	int obj2;
	int msg;
	int next;
};

typedef struct instrs instr;

void copy_instr(instr * p1,instr * p2){
	p1->obj=p2->obj;
	p1->setOrGet=p2->setOrGet;
	p1->fname=p2->fname;
	p1->obj2=p2->obj2;
	p1->msg=p1->msg;
	p1->next=p2->next;
}
#define max_obj 2000000

int obj[max_obj];
int method[max_obj];
int code[max_obj];
int num_code=0;

instr pool[max];
int num_pool=0;

void kern_add(int s,int u,int v){
	num_set++;
	field[num_set]=s;
	base_obj[num_set]=u;
	value_obj[num_set]=v;
}

int kern_get(int s,int u){
	int lp;
	int res=0;
	for (lp=1;lp<=num_set;lp++){
		if (s==field[lp]){
		if (u==base_obj[lp]){
			res=value_obj[lp];
		}	
		}
	}
	return res;
}

void print_store(){
	int lp;
	for (lp=1;lp<=num_set;lp++){
		printf("%d: %s %s %s\n",lp,kern_sym[field[lp]],kern_sym[base_obj[lp]],kern_sym[value_obj[lp]]);
	}
}

void trace(){
	int lp;
	int ptr;
	for (lp=1;lp<=num_code;lp++){
		printf("Mthd: ");
		ptr=code[lp];
		while (ptr!=0){
			printf("[%s %s] ",kern_sym[pool[ptr].obj],mtype[pool[ptr].setOrGet]);
			ptr=pool[ptr].next;
		}
		printf("\n");
	}
}

int  get_code(int o,int m){
	int lp;
	int res;

	res=0;
	for (lp=1;lp<=num_code;lp++){
		if (obj[lp]==o){
		if (method[lp]==m){
			//res->obj=code[lp].obj;
			//res->setOrGet=code[lp].setOrGet;
			//res->fname=code[lp].fname;
			//res->obj2=code[lp].obj2;
			//res->msg=code[lp].msg;
			//res->next=code[lp].next;
			res=code[lp];
		}
		}
	}
	return res;
}
			
void exec(int i){
	int it;
	int ncode;

	printf("exec>>>\n");
	print_store();
	printf("%s\n",kern_sym[pool[i].obj]);
	if (pool[i].setOrGet==doSet){
		printf("doSet:\n");
		kern_add(pool[i].fname,pool[i].obj,pool[i].obj2);
		printf("set ok!\n");
	}
	if (pool[i].setOrGet==doGet){
		printf("doGet:\n");
		printf("%s\n",kern_sym[pool[i].fname]);
		printf("%s\n",kern_sym[pool[i].obj]);
it=0;
		it=kern_get(pool[i].fname,pool[i].obj);
if (it!=0){
		printf("%s\n",kern_sym[it]);
		ncode=get_code(it,pool[i].msg);
		if (ncode!=0){
			exec(ncode);
		}
}
	}
	if (pool[i].next!=0){
		exec(pool[i].next);
	}
	printf("fexec>>\n");
	print_store();
}

int kern_find(char * s){
	int res=0;
	int lp;

	for (lp=1;lp<=num_kern_sym;lp++){
		if (strcmp(s,kern_sym[lp])==0){
			res=lp;
		}
	}
	if (res==0){
		num_kern_sym++;
		kern_sym[num_kern_sym]=(char *)(malloc(sizeof(char)*(strlen(s)+3)));
		strcpy(kern_sym[num_kern_sym],s);
		res=num_kern_sym;
	}
	return res;
}

void kern_parse_instr(instr * i, char * s){

	char sym[1000];
	int obj;
	int set_or_get;
	int fname;
	int obj2;
	int msg;

	printf("*** %s ***\n",s);	

	int p=0;int p2=0;

	p2=0;

	while (s[p]!=' '){
		sym[p2]=s[p];
		p++;
		p2++;
	}
	sym[p2]='\0';	
	p++;

	obj=kern_find(sym);
	printf("%s\n",sym);
	p2=0;

	while (s[p]!=' '){
		sym[p2]=s[p];
		p++;
		p2++;
	}
	sym[p2]='\0';
	p++;

	if (strcmp(sym,"set")==0){
		set_or_get=doSet;
	}
	if (strcmp(sym,"get")==0){
		set_or_get=doGet;
	}

	printf("%s\n",sym);
	p2=0;

	while (s[p]!=' '){
		sym[p2]=s[p];
		p++;
		p2++;
	}
	sym[p2]='\0';
	
	fname=kern_find(sym);

	printf("%s\n",sym);

	p++;

	p2=0;

	while (s[p]!=' '){
		sym[p2]=s[p];
		p++;
		p2++;
	}
	sym[p2]='\0';

	p++;
	if (s[p]!='.'){
		printf("error\n");
	}

	if (set_or_get==doSet){
		obj2=kern_find(sym);
	}
	if (set_or_get==doGet){
		msg=kern_find(sym);
	}

	i->obj=obj;
	i->setOrGet=set_or_get;
	i->fname=fname;
	i->obj2=obj2;
	i->msg=msg;
	i->next=0;
}

void kern_init(){
	int lp;

	mtype[doGet]=malloc(sizeof(char)*10);
	strcpy(mtype[doGet],"get");
	mtype[doSet]=malloc(sizeof(char)*10);
	strcpy(mtype[doSet],"set");
	for (lp=1;lp<=100;lp++){
		pool[lp].next=0;
	}
	num_set=0;
	num_code=0;
	num_kern_sym=0;
	num_pool=1;


	kern_add(kern_find("rel"),kern_find("root"),kern_find("root"));	
}

void nuCode(char * cstr,char *mstr,char * rst){
	instr i;
	int ptr=0;
	int newq=0;
	int lp;
	int c;
	int m;

	c=kern_find(cstr);
	m=kern_find(mstr);
	

	//ni->next=0L;
	num_pool++;
	kern_parse_instr(&(pool[num_pool]),rst);

	newq=0;
	for (lp=1;lp<=num_code;lp++){
		if (c==obj[lp]){
		if (m==method[lp]){
			newq=1;
			ptr=code[lp];
		}
		}
	}

	if (newq==1){
		while (pool[ptr].next!=0){
			ptr=pool[ptr].next;
		}
		pool[ptr].next=num_pool;
	}

	if (newq==0){
		num_code++;
		obj[num_code]=c;
		method[num_code]=m;
		code[num_code]=num_pool;
	}

	
}

void kern_run(char * c,char * m){
	instr i;
	

	trace();
	printf(">>> Start run ...\n");
	kern_add(kern_find("self"),kern_find(c),kern_find(c));	
	pool[1].obj=kern_find(c);
	pool[1].setOrGet=doGet;
	pool[1].fname=kern_find("self");
	pool[1].msg=kern_find(m);
	pool[1].next=0L;
	exec(1);
	//pool[1].msg=kern_find("mouseDown");
	//exec(1);
	//pool[1].msg=kern_find("draw");
	//exec(1);
	printf(">>> Stop run ...\n");
}
