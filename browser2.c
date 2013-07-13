#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pango/pango-layout.h>
#include <X11/Xlib.h>
#include <pango/pango-layout.h>
#include "kern.c"

GtkWidget *redraw;

int allwidth = 200;

int to_drag = 0;
int which_s_p = 0;
int ox = 0;
int oy = 0;
char title[10000];
int ok_title = 0;
int new_stuff_ready = 0;
char *model[100000];
int num_model = 0;
int pal[4];
int ptrx = 0, ptry = 0;

char *hold[100000];
int num = 0;

int e_class[100000];
int e_method_name[100000];
int e_method_body[100000];
int nume = 0;

int curr_class = 0;
int curr_method = 0;

int show_1[20000];
int show_2[20000];
int show_3[20000];
int show_4[20000];
int show_5[20000];

char line[10000];
int p;


int bx[100];
int by[100];
int oldbx[100];
int oldby[100];
char msg_box[10000];

int mx1[10000];
int my1[10000];
int mx2[10000];
int my2[10000];
int mcode[10000];
int mitem[10000];
int numm = 0;

int entry = 0;

char *hold_class[100000];
char *hold_method[100000];
int num_classes = 0;
int num_methods = 0;
int count_meth = 0;
char m_line[1000];
int m_p;
char *space[100];
char *fname[100];
int num_space = 0;
int current_space = 1;

char work_space[10000];
int wsp = 0;

char current_class_token[1000];
char current_method_token[1000];

struct pics {
	GdkPixbuf * pixbuf;
	int x;
	int y;
	int level;
	int pnum;
};

typedef struct pics pic;

int numpixs=0;

pic holdpix[200];

//
//
//

int num_pst=0;
int npx[1000];
int npy[1000];

void spinup(){
	int lp=0;

	num_pst=10;
	for (lp=1;lp<=num_pst;lp++){
		npx[lp]=lp*57-30+500;
		//npy[lp]=lp*13+50;
		npy[lp]=63+11;
	}
}

int past_particle(int mx,int my){
	int lp;
	int which=0;
	int res=0;

	which=0;

	for (lp=1;lp<=num_pst;lp++){
		if (mx>=npx[lp]){
		if (my>=npy[lp]){
		if (mx<=npx[lp]+50){
		if (my<=npy[lp]+17){
			which=lp;
		}
		}
		}
		}
	}

	if (which!=0){
		res=which;
		npx[which]=mx-25;
		npy[which]=my-9;
	}

	return res;
}

void handle_past3(GtkWidget * wdg){
}

void handle_past(GtkWidget * wdg){
int slot=0;
int unit=0;
int value=0;

int lp;
int toggle[12];

toggle[1]=0;
toggle[2]=0;

slot= kern_find("open");
unit= kern_find("box1");
value=kern_find("true");

if (kern_get(slot,unit)==value){
	toggle[1]=1;
}

slot= kern_find("open");
unit= kern_find("box2");
value=kern_find("true");

if (kern_get(slot,unit)==value){
	toggle[2]=1;
}

for (lp=1;lp<=num_pst;lp++){

if (lp==1){
if (toggle[1]==0){
 gdk_draw_rectangle (wdg->window, wdg->style->black_gc, FALSE,
		      npx[lp],npy[lp],50,17);
}
else {
 gdk_draw_rectangle (wdg->window, wdg->style->black_gc, TRUE,
		      npx[lp],npy[lp],50,17);
}
}
//
if (lp==2){
if (toggle[2]==0){
 gdk_draw_rectangle (wdg->window, wdg->style->black_gc, FALSE,
		      npx[lp],npy[lp],50,17);
}
else {
 gdk_draw_rectangle (wdg->window, wdg->style->black_gc, TRUE,
		      npx[lp],npy[lp],50,17);
}
}
//

if (lp>2)
{
 gdk_draw_rectangle (wdg->window, wdg->style->black_gc, FALSE,
		      npx[lp],npy[lp],50,17);
}
}


}
//

//


// lod pixs!

void new_image(char * name){
	GError **error;

	printf("%d %s\n",num,name);
	numpixs++;
	
	holdpix[numpixs].x=0;
	holdpix[numpixs].y=0;
	holdpix[numpixs].level=num;
	error=NULL;
	holdpix[numpixs].pixbuf=gdk_pixbuf_new_from_file(name,error);
	
}

void load_images() {
	FILE * fp;
	char c;
	int p=0;
	char name[1000];
	int done=0;
	
	printf("Loading images...\n");
	fp=fopen("pics","r");
	while (done==0){
		c=getc(fp);
		if (c==EOF){
			done=1;
		}
		else {
			if (c=='\n'){
				name[p]='\0';
				if (p>2){
					new_image(name);
				}
				p=0;
			}
			else {
				name[p]=c;
				p++;
			}
		}
	}
	fclose(fp);
}

// load spaces
// each space has a name linked to filename which holds its contents

void
load_spaces ()
{
  FILE *fp;
  char s[1000];
  char f[1000];
  int p1, p2;
  int done = 0;
  char c;
  fp = fopen ("fspaces.txt", "r");

  while (done == 0)
    {
      c = getc (fp);
      if (c == '!')
	{
	  done = 1;
	}
      else
	{
	  p1 = 0;
	  s[p1] = c;
	  p1++;
	  while (c != ' ')
	    {
	      c = getc (fp);
	      s[p1] = c;
	      p1++;
	    }
	  s[p1] = '\0';
	  p2 = 0;

	  while (c != '\n')
	    {
	      c = getc (fp);
	      if (c != '\n')
		{
		  f[p2] = c;
		  p2++;
		}
	    }
	  f[p2] = '\0';



	  num_space++;
	  space[num_space] = malloc (sizeof (char) * 1000);
	  fname[num_space] = malloc (sizeof (char) * 1000);
	  strcpy (space[num_space], s);
	  strcpy (fname[num_space], f);


	}


    }
  fclose (fp);
}

// pango rutines for drawing a string, various options

void
drawStringWithWidth (GtkWidget * widget, char *str, int width, int x, int y)
{
  PangoLayout *pl;



  pl = gtk_widget_create_pango_layout (widget, str);
  pango_layout_set_width (pl, width * 100 * 8);
  gdk_draw_layout (widget->window, widget->style->black_gc, x, y - 16, pl);
  g_object_unref (pl);

}

void
drawString (GtkWidget * w, char *s, int x, int y)
{
  drawStringWithWidth (w, s, 800, x, y);
}

// all draw string listed

void
strip (char *s)
{
  int sptr;

  sptr = 0;
  while (s[sptr] != '.')
    {
      sptr++;
    }
  sptr++;
  s[sptr] = '\0';
}

// write the current model out to disk
// get the filename for string the current space

void
write_model ()
{
  FILE *fp;
  int lp;

  fp = fopen (fname[current_space], "w");
  for (lp = 1; lp <= num_model; lp++)
    {
      if (model[lp][0] != ' ')
	{
	  strip (model[lp]);
	  fprintf (fp, "%s\n", model[lp]);
	}
    }
  fprintf (fp, "!");
  fclose (fp);
}

// find the class number for class : c

int
get_class_num (char *c)
{
  int res;
  int lp;

  res = 0;
  for (lp = 1; lp <= num_classes; lp++)
    {
      if (strcmp (hold_class[lp], c) == 0)
	{
	  res = lp;
	}
    }
  if (res == 0)
    {
      num_classes++;
      hold_class[num_classes] =
	(char *) malloc (sizeof (char) * (5 + strlen (c)));
      strcpy (hold_class[num_classes], c);
      res = num_classes;
    }
  return res;

}

// end find class


// find the method number for method : m

int
get_method_num (char *m)
{
  int res;
  int lp;

  res = 0;
  for (lp = 1; lp <= num_methods; lp++)
    {
      if (strcmp (hold_method[lp], m) == 0)
	{
	  res = lp;
	}
    }
  if (res == 0)
    {
      num_methods++;
      hold_method[num_methods] =
	(char *) malloc (sizeof (char) * (5 + strlen (m)));
      strcpy (hold_method[num_methods], m);
      res = num_methods;
    }
  return res;

}

// end find method number 

// add a line to the model
// stored as is (string)
// not yet processed

void
add_to_model (char *ns)
{
  num_model++;
  model[num_model] = (char *) malloc (sizeof (char) * (3 + strlen (ns)));
  strcpy (model[num_model], ns);

}

// end add to model

// read in the model for the current space

void
read_model ()
{
  FILE *fp;
  char c;
  int done;


  fp = fopen (fname[current_space], "r");
  m_p = 0;
  num_model = 0;
  done = 0;
  while (done == 0)
    {
      c = getc (fp);
      if (c == '!')
	{
	  done = 1;
	}
      else
	{
	  if (c == '\n')
	    {
	      m_line[m_p] = '.';
	      m_p++;
	      m_line[m_p] = '\0';
	      add_to_model (m_line);
	      m_p = 0;
	    }
	  else
	    {
	      m_line[m_p] = c;
	      m_p++;
	    }
	}
    }
  fclose (fp);
  printf ("\n***\nfinished reading\n***\n");
}

// end read model

int num_assigned[100000];
int spunner[100000];

void
add_assign (int c)
{
  num_assigned[c]++;
}

void
spun (int m, int cm)
{
  spunner[m] = cm;
}

int add_prev_method = 0;

void
new_v_part (char *c, char *m, char *b)
{
  nuCode (c, m, b);
  hold[num] = (char *) malloc (sizeof (char) * (3 + strlen (c)));
  strcpy (hold[num], c);
  show_1[entry] = num;
  num++;
  hold[num] = (char *) malloc (sizeof (char) * (3 + strlen (m)));
  strcpy (hold[num], m);
  show_2[entry] = num;
  num++;
  hold[num] = (char *) malloc (sizeof (char) * (3 + strlen (b)));
  strcpy (hold[num], b);
  show_3[entry] = num;
  num++;
  //show only
  show_4[entry] = get_class_num (c);
  if (get_class_num (c) != 1)
    {
      if (show_4[entry] != show_4[entry - 1])
	{
	  //
	  count_meth = 0;
	}
    }
  // is add item : method diff ??
  if (get_method_num (m) != add_prev_method)
    {
      count_meth++;
    }
  //show_5[entry]=get_method_num(m);
  if (spunner[get_method_num (m)] == 0)
    {
      spun (get_method_num (m), count_meth);
    }
  show_5[entry] = spunner[get_method_num (m)];
  entry++;
  add_prev_method = get_method_num (m);
}

// parse a line into class method and code
// local names
// (head, sec, rest)
// "sec" second

void
doln (int ln)
{
  int p = 0;
  int p2 = 0;
  char head[1000];
  char sec[1000];
  char rst[1000];


  printf ("***** begine *****\n");
  p2 = 0;
  while (((model[ln])[p]) != ' ')
    {
      head[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  while (((model[ln])[p]) == ' ')
    {
      p++;
    }
  head[p2] = '\0';
  p2 = 0;
  while (((model[ln])[p]) != ' ')
    {
      sec[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  while (((model[ln])[p]) == ' ')
    {
      p++;
    }
  sec[p2] = '\0';
  p2 = 0;
  while ((((model[ln])[p]) != '\0') && (((model[ln])[p]) != '.'))
    {
      rst[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  rst[p2] = '\0';

  printf ("*%s*%s*%s*\n", head, sec, rst);
  new_v_part (head, sec, rst);

  printf ("*** end ****\n");
}

// end do a line

// the class and method numbers for a line

int calcc[100000];
int calcm[100000];

// find the class and method numbers for a line

void
calc_a_line (int ln)
{
  int p = 0;
  int p2 = 0;
  char head[1000];
  char sec[1000];
  char rst[1000];

  p2 = 0;
  while (((model[ln])[p]) != ' ')
    {
      head[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  while (((model[ln])[p]) == ' ')
    {
      p++;
    }
  head[p2] = '\0';
  p2 = 0;
  while (((model[ln])[p]) != ' ')
    {
      sec[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  while (((model[ln])[p]) == ' ')
    {
      p++;
    }
  sec[p2] = '\0';
  p2 = 0;
  while ((((model[ln])[p]) != '\0') && (((model[ln])[p]) != '.'))
    {
      rst[p2] = ((model[ln])[p]);
      p++;
      p2++;
    }
  rst[p2] = '\0';

  printf ("*%s*%s*%s*\n", head, sec, rst);
  calcc[ln] = get_class_num (head);
  calcm[ln] = get_method_num (sec);

}

// end finding numbers

void
calc_num ()
{
  int lp;
  for (lp = 1; lp <= num_model; lp++)
    {
      calc_a_line (lp);
    }
}

// sort

void
sort ()
{
  int lp;
  int done = 0;
  char *swp;
  int swpi;

  printf ("sort...\n");
  while (done == 0)
    {
      done = 1;
      for (lp = 1; lp < num_model; lp++)
	{
	  if (calcc[lp] > calcc[lp + 1])
	    {
	      done = 0;
	      swp = model[lp];
	      model[lp] = model[lp + 1];
	      model[lp + 1] = swp;
	      swpi = calcc[lp];
	      calcc[lp] = calcc[lp + 1];
	      calcc[lp + 1] = swpi;
	      swpi = calcm[lp];
	      calcm[lp] = calcm[lp + 1];
	      calcm[lp + 1] = swpi;

	    }
	}
    }

  done = 0;
  while (done == 0)
    {
      done = 1;
      for (lp = 1; lp < num_model; lp++)
	{
	  if (calcc[lp] == calcc[lp + 1])
	    {
	      if (calcm[lp] > calcm[lp + 1])
		{
		  done = 0;
		  swp = model[lp];
		  model[lp] = model[lp + 1];
		  model[lp + 1] = swp;
		  swpi = calcc[lp];
		  calcc[lp] = calcc[lp + 1];
		  calcc[lp + 1] = swpi;
		  swpi = calcm[lp];
		  calcm[lp] = calcm[lp + 1];
		  calcm[lp + 1] = swpi;
		}
	    }
	}
    }
  printf ("end sort\n");
}

// end sort

// refresh
// adjust to changes which were text, then added to model
// calculate numbers for each line
// sort
// redo storage that drawBrowser uses [show1][show2][show3]

void
refresh_view ()
{
  int lp;
  printf ("refresh_view\n");



  calc_num ();

  sort ();

  add_prev_method = 0;
  for (lp = 1; lp <= 1000; lp++)
    {
      num_assigned[lp] = 0;
      spunner[lp] = 0;
    }

  num_classes = 0;
  num_methods = 0;
  count_meth = 0;
  num = 1;			// strs are new
  entry = 1;
  printf ("\nloop\n");
  for (lp = 1; lp <= num_model; lp++)
    {
      doln (lp);
    }
  show_1[entry] = 0;
  show_2[entry] = 0;
  show_3[entry] = 0;
  printf ("\nend loop\n");

}

// end refresh

// lookup table for class and method numbers
// used to ignore skipped line in linear arrangement

int hold_r[20000];
int hold_r2[20000];
void
addref (int l, int c)
{
  hold_r[l] = c;
}

int
deref (int l)
{
  return hold_r[l];
}

void
addref2 (int l, int m)
{
  hold_r2[l] = m;
}

int
deref2 (int l)
{
  return hold_r2[l];
}

// end look up c/m.


void
switch_space (int ns)
{
  current_space = ns;
  read_model ();
  refresh_view ();
}

// if a rectangle was registered here it is 'called' if mouse
// down in its bounds
// code:
// 2: 
// 3: run code on current method/class.
// 4: choose class
// 5: choose method
// 6: choose code
// 7: change colour

void
dom (int thex, int they)
{
  int lp;
  int some_occured = 0;
  char new_msg[10000];
  char anum[10];
int done_code=0;

  strcpy (new_msg, "Action: ");
  for (lp = 1; lp <= numm; lp++)
    {
      if (thex >= mx1[lp])
	{
	  if (they >= my1[lp])
	    {
	      if (thex <= mx2[lp])
		{
		  if (they <= my2[lp])
		    {
		      if (mcode[lp] == 2)
			{

			  strcat (new_msg, "Change scroll of pane: ");

			  strcpy (anum, "0");
			  anum[0] += mitem[lp] - 4;
			  anum[1] = '\0';
			  strcat (new_msg, anum);
			  some_occured = 1;
			  to_drag = 2;
			  ox = thex;
			  oy = they;
			  which_s_p = mitem[lp];
			  for (lp = 1; lp <= 20; lp++)
			    {
			      oldbx[lp] = bx[lp];
			      oldby[lp] = by[lp];
			    }
			}
done_code=0;

		      if (mcode[lp] == 3)
			{

			}


if (mcode[lp]==13){
			  kern_init ();
			  refresh_view ();
			  kern_run (current_class_token,
				    current_method_token);
			
			  some_occured = 1;done_code=1;
}

if (done_code==0){
		      if (mcode[lp] == 4)
			{
			  //
			  curr_class = deref (mitem[lp]);
			  //
			  strcat (new_msg, "Class name: ");
			  strcpy (anum, "0");
			  anum[0] += deref (mitem[lp]);
			  strcat (new_msg, anum);
			  some_occured = 1;
			  strcpy (current_class_token,
				  hold[show_1[mitem[lp]]]);
			}
		      if (mcode[lp] == 5)
			{
			  //
			  curr_method = deref2 (mitem[lp]);
			  //
			  strcat (new_msg, "Method name: ");
			  strcpy (anum, "0");
			  anum[0] += deref2 (mitem[lp]);
			  strcat (new_msg, anum);
			  some_occured = 1;
			  strcpy (current_method_token,
				  hold[show_2[mitem[lp]]]);
			}
		      if (mcode[lp] == 6)
			{
			  strcat (new_msg, "Code body: ");
			  strcpy (anum, "0");
			  anum[0] += mitem[lp];
			  strcat (new_msg, anum);
			  some_occured = 1;
			}
		      if (mcode[lp] == 7)
			{
			  printf ("scc\n");
			  strcat (new_msg, "Change scroll of colour: ");

			  strcpy (anum, "0");
			  anum[0] += mitem[lp] - 4;
			  anum[1] = '\0';
			  strcat (new_msg, anum);
			  some_occured = 1;
			  to_drag = 3;
			  ox = thex;
			  oy = they;
			  which_s_p = mitem[lp];
			  for (lp = 1; lp <= 20; lp++)
			    {
			      oldbx[lp] = bx[lp];
			      oldby[lp] = by[lp];
			    }
			}
}

		    }
		}
	    }
	}
    }
  if (some_occured == 0)
    {

      to_drag = 1;
      ox = thex;
      oy = they;
      for (lp = 1; lp <= 20; lp++)
	{
	  oldbx[lp] = bx[lp];
	  oldby[lp] = by[lp];
	}
    }
  strcpy (msg_box, "");
  strcat (msg_box, current_class_token);
  strcat (msg_box, " ");
  strcat (msg_box, current_method_token);
  strcat (msg_box, " .");
}


void
start_register ()
{
  numm = 1;
}


// register the active areas of the screen
// pressing them will fire a (code,item)

void
mregister (int rx1, int ry1, int rx2, int ry2, int rcode, int ritem)
{
  numm++;
  mx1[numm] = rx1;
  my1[numm] = ry1;
  mx2[numm] = rx2;
  my2[numm] = ry2;
  mcode[numm] = rcode;
  mitem[numm] = ritem;
}

void
readfile ()
{
  FILE *fp;
  char c;
  int done = 0;

  fp = fopen ("texit.txt", "r");
  while (done == 0)
    {
      c = getc (fp);
      if (c == EOF)
	{
	  done = 1;
	}
      else
	{
	  if (c == '\n')
	    {

	    }
	  else
	    {
	      line[p] = c;
	      p++;
	    }
	}
    }
  fclose (fp);
}

int
match (char *s1, char *s2, int start)
{
  int res = 0;
  int all = 1;
  int lp;

  for (lp = 0; lp < strlen (s1); lp++)
    {
      if (s1[lp] != s2[lp])
	{
	  all = 0;
	}
    }

  if (all == 1)
    {
      res = 1;
    }
  return res;
}

int
offset (char *s1, char *s2)
{
  int lp;
  // s1 source
  // s2 target
  int res = 0;

  for (lp = 0; lp < strlen (s2); lp++)
    {
      if (match (s1, s2, lp) != 0)
	{
	  res = lp;
	}
    }
  return res;
}

void
init ()
{
  int lp;


  pal[1] = 27857;
  pal[2] = 35595;
  pal[3] = 24452;
  for (lp = 1; lp <= 100; lp++)
    {
      hold[lp] = (char *) malloc (sizeof (char) * 1000);
    }

  strcpy (hold[1], "duck");
  strcpy (hold[2], "sheep");
  strcpy (hold[3], "goose");

  strcpy (hold[4], "quack");
  strcpy (hold[5], "baa");
  strcpy (hold[6], "honk");

  strcpy (hold[7], "env sound: animal");
  strcpy (hold[8], "animal expects: reply");
  strcpy (hold[9], "animal state: state indicate: sound");

  curr_class = 1;
  curr_method = 1;

  show_1[1] = 1;
  show_1[2] = 2;
  show_1[3] = 3;
  show_1[4] = 0;
  show_2[1] = 4;
  show_2[2] = 5;
  show_2[3] = 6;
  show_2[4] = 0;
  show_3[1] = 7;
  show_3[2] = 8;
  show_3[3] = 9;
  show_3[4] = 0;

  bx[1] = 130;
  by[1] = 150;
  bx[2] = 230;
  by[2] = 150;
  bx[3] = 130;
  by[3] = 220;
  bx[4] = 340;
  by[4] = 283;

  bx[5] = 130 - 16;

  by[5] = 150;
  bx[6] = 340 - 5;
  by[6] = by[1];
  bx[7] = 340 - 5;
  by[7] = by[3];


  bx[8] = 130;
  bx[9] = 130;
  bx[10] = 130;
  by[8] = 300;
  by[9] = 320;
  by[10] = 340;
  bx[11] = 130;



}

void
add_item ()
{
  new_stuff_ready = 1;
}

void
add_to_mem (GtkWidget * widget)
{
  // grab text in to date
  int lp;
  int s1, s2;
  char new_stuff[1000];
  for (lp = 0; lp < 100; lp++)
    {
      new_stuff[lp] = ' ';
    }
  new_stuff[100] = '\0';
  s1 = 80;
  s2 = 80;
  printf ("%d\n", s2);
  printf ("%s\n", new_stuff);
  add_to_model (new_stuff);
  refresh_view ();
  write_model ();
  //
}

int prev_c[20000];
int prev_m[20000];
int n_prev_c = 0;
int n_prev_m = 0;


int
ok_cl (int c)
{
  int res = 1;

  if (n_prev_c == 0)
    {
      prev_c[1] = c;
      n_prev_c = 1;
    }
  else
    {
      if (strcmp (hold[prev_c[1]], hold[c]) == 0)
	{
	  res = 0;
	}
    }
  prev_c[1] = c;
  return res;
}

int
ok_me (int m)
{
  int res = 1;
  if (n_prev_m == 0)
    {
      prev_m[1] = m;
      n_prev_m = 1;
    }
  else
    {
      if (strcmp (hold[prev_m[1]], hold[m]) == 0)
	{
	  res = 0;
	}
    }
  prev_m[1] = m;
  return res;
}

void
start_ok ()
{
  n_prev_c = 0;
  n_prev_m = 0;
}


// rough work ; plase avoid !
void
drawBrowser (GtkWidget * widget)
{
  int px, py;
  int done = 0;
  int ptr = 0;
  int knsx = 10;
  int knsy = 10;
  int lp;
  int ccount = 1;
  int mcount = 1;
  int ditx = 0;
  int dity = 0;
  int eps = -4;
  double myAngle = 3.1415;
  int crlp;
GdkColor dull;
  GdkColor black;
  GdkColor green;
  GdkColor red;
  GdkColor lblu;
  GdkGC *r;
  black.pixel = 0;
  black.red = 0;
  black.green = 0;
  black.blue = 0;

  dull.pixel = 40000;
  dull.red = 40000;
  dull.green = 40000;
  dull.blue = 40000;


  red.pixel = 20000;
  red.red = pal[1];
  red.green = pal[2];
  red.blue = pal[3];
  green.pixel = 20000;
  green.red = pal[2];
  green.green = pal[3];
  green.blue = pal[1];
  lblu.pixel = 0;
  lblu.red = pal[3];
  lblu.green = pal[1];
  lblu.blue = pal[2];

  r = widget->style->black_gc;
  GdkRectangle cr;
  GdkRegion *gr;
  GdkRectangle glb;
int pdx=0;
int pdy=0;
int shdy=227;

  glb.x = 0;
  glb.y = 0;
  glb.width = 1200;
  glb.height = 1200;
  start_ok ();


  start_register ();

// Shadow .

gdk_gc_set_rgb_fg_color (r, &dull);

 gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
		      bx[11] - 4 - 3 - 13, by[1] - 20 - 20 - 20 - 13, bx[4] - bx[11] + 3 + 12 + 3 +  13 + 13,
		      shdy + 13 +  13);



  // bkgnd and title

  gdk_gc_set_rgb_fg_color (r, &lblu);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
		      bx[11] - 4 - 3, by[4] - 23 + 2, bx[4] - bx[11] + 3 + 12,
		      40);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
		      bx[11] - 4 - 3, by[1] - 20 - 20 - 10 ,
		      bx[4] - bx[11] + 3 + 12, 20 + 10);



  gdk_gc_set_rgb_fg_color (r, &red);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
    bx[1] - 2, by[1] - 18, (bx[2] - bx[1]) - 14,
		      (by[3] - by[1]) - 10);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
    bx[2] - 6, by[1] - 18, (bx[2] - bx[1]) + 4 + 2,
		      (by[3] - by[1]) - 10);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
		      bx[11] - 2, by[3] - 18, (bx[4] - bx[11]) - 4 - 3,
		      (by[3] - by[1]) - 10 - 3);

  mregister (bx[2] - 7 - 10, by[3] - 18 - 7 - 3, bx[2] - 7 - 10 + knsx,
	     by[3] - 18 - 7 - 3 + knsy, 3, 1);

  gdk_gc_set_rgb_fg_color (r, &green);
  for (lp = 1; lp <= 3; lp++)
    {

        //
        
      gdk_draw_rectangle (widget->window, widget->style->black_gc, TRUE,
			  bx[lp + 4], by[4 + lp], knsx, knsy);

      mregister (bx[lp + 4], by[lp + 4], bx[lp + 4] + knsx, by[lp + 4] + knsy,
		 2, lp + 4);
    }


  gdk_gc_set_rgb_fg_color (r, &black);
  drawString (widget, "Code browser ...", bx[11] + 12, 1 * (by[1] - 30));
//
//

// outline rects in black .

  gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
		      bx[11] - 4 - 3, by[4] - 23 + 2, bx[4] - bx[11] + 3 + 12,
		      40);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
		      bx[11] - 4 - 3, by[1] - 20 - 20 - 10,
		      bx[4] - bx[11] + 3 + 12, 20 + 10);



  gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
		      bx[1] - 2, by[1] - 18, (bx[2] - bx[1]) - 14,
		      (by[3] - by[1]) - 10);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
		      bx[2] - 6, by[1] - 18, (bx[2] - bx[1]) + 4 + 2,
		      (by[3] - by[1]) - 10);
  gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
		      bx[11] - 2, by[3] - 18, (bx[4] - bx[11]) - 4 - 3,
		      (by[3] - by[1]) - 10 - 3);

// finish up ...

  for (lp = 1; lp <= 3; lp++)
    {


      gdk_draw_rectangle (widget->window, widget->style->black_gc, FALSE,
			  bx[lp + 4], by[4 + lp], knsx, knsy);

     
    }

//
//

// end outline .

//
//

// show triple store

  for (crlp = 1; crlp <= num_set; crlp++)
    {
      drawString (widget, kern_sym[field[crlp]], 5, 1 * (30 + (crlp * 18)));
      drawString (widget, kern_sym[base_obj[crlp]], 85,
		  1 * (30 + (crlp * 18)));
      drawString (widget, kern_sym[value_obj[crlp]], 165,
		  1 * (30 + (crlp * 18)));
    }

// end show triple store


// draw class panel

  px = bx[1];
  py = by[1] - (1 * (by[5] - (by[1] - 10)));

  ptr = 1;

  cr.x = bx[1] - 2;
  cr.y = by[1] - 18;
  cr.width = (bx[2] - bx[1]) - 14;
  cr.height = (by[3] - by[1]) - 10;
  gr = gdk_region_rectangle (&cr);
  gdk_gc_set_clip_region (r, gr);

  while (done == 0)
    {
      if (show_1[ptr] == 0)
	{
	  done = 1;
	}
      else
	{
	  if ((ok_cl (show_1[ptr]) == 1))
	    {
	      if (py >= by[1] - 20)
		{
		  if (py <= by[3] + 20)
		    {
		      drawString (widget, hold[show_1[ptr]], px, py);
		      mregister (px, py - 16 + 8 + eps,
				 px + (bx[2] - bx[1]) - 12, py - 1 + 8 + eps,
				 4, ptr);
		    }
		}
	      addref (ptr, ccount);
	      ccount++;
	      ptr++;
	      py += 16;
	    }
	  else
	    {
	      ptr++;
	    }




	}

    }

  gr = gdk_region_rectangle (&glb);
  gdk_gc_set_clip_region (r, gr);

// end show class panel

// start show method (of current class) panel

// clip region
  cr.x = bx[2] - 6;
  cr.y = by[1] - 18;
  cr.width = (bx[2] - bx[1]) + 6;
  cr.height = (by[3] - by[1]) - 10;
  gr = gdk_region_rectangle (&cr);
  gdk_gc_set_clip_region (r, gr);

// pen coords

  px = bx[2];
  py = by[2] - (by[6] - by[1]);
  ptr = 1;
  done = 0;
  while (done == 0)
    {
      if (show_2[ptr] == 0)
	{
	  done = 1;
	}
      else
	{

	  if ((ok_me (show_2[ptr]) == 1) && (curr_class == show_4[ptr]))
	    {
	      drawString (widget, hold[show_2[ptr]], px, 1 * py);
	      mregister (px, py - 16 + 8 + eps, px + (bx[4] - bx[2]) - 12,
			 py - 1 + 8 + eps, 5, ptr);
	      addref2 (ptr, mcount);
	      mcount++;
	      ptr++;
	      py += 16;
	    }
	  else
	    {
	      ptr++;
	    }

	}
    }

  gr = gdk_region_rectangle (&glb);
  gdk_gc_set_clip_region (r, gr);

// end show method panel

// show code panel

  cr.x = bx[11] - 2;
  cr.y = by[3] - 18;
  cr.width = (bx[4] - bx[11]) - 4 - 3;
  cr.height = (by[3] - by[1]) - 10 - 3;
  gr = gdk_region_rectangle (&cr);
  gdk_gc_set_clip_region (r, gr);


  px = bx[3];
  py = by[3] - (by[7] - by[3]);
  ptr = 1;
  done = 0;
  while (done == 0)
    {
      if (show_3[ptr] == 0)
	{
	  done = 1;
	}
      else
	{
	  if ((curr_class == show_4[ptr]) && (curr_method == show_5[ptr]))
	    {
	      drawString (widget, hold[show_3[ptr]], px, 1 * py);
	      mregister (px, py - 16 + 8 + eps, px + (bx[4] - bx[1]) - 12,
			 py - 1 + 8 + eps, 6, ptr);
	      ptr++;
	      py += 16;
	    }
	  else
	    {
	      ptr++;
	    }
	}
    }
  gr = gdk_region_rectangle (&glb);
  gdk_gc_set_clip_region (r, gr);

// end show code panel

// show current input line

  drawString (widget, work_space, bx[1], by[4] + 40);

//icons
	pdx=gdk_pixbuf_get_width(holdpix[1].pixbuf);
	pdy=gdk_pixbuf_get_height(holdpix[1].pixbuf);

	gdk_draw_pixbuf(widget->window,
				widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
				holdpix[1].pixbuf,
				0,0,bx[1]+240-20-30,by[1]-30-20-32-32,pdx,pdy,
				GDK_RGB_DITHER_NONE,0,0);
mregister(bx[1]+240-20-30,by[1]-30-20-32-32,bx[1]+240-20-30+32,by[1]-30-20+32-32-32,13,1);



}

// end draw browser 

// respond to key press
// press return to add code to the current class and method
// not return add to workspace string

static gboolean
keyPressed (GtkWidget * widget, GdkEventKey * kevt)
{
  char c;
  char tom[2000];
  c = kevt->string[0];
  int p;
  printf ("** CHAR %d ***", (int) c);
  if ((c != 13))
    {
      if (c == '-')
	{
	  wsp--;
	  if (wsp < 0)
	    {
	      wsp = 0;
	    }
	  work_space[wsp] = '.';
	}
      else
	{
	  work_space[wsp] = c;
	  wsp++;
	}
    }

  if (c == 13)
    {
      add_to_model (work_space);
      refresh_view ();
      write_model ();


      wsp = 0;

      strcpy (work_space, "...........................................");
    }
  gtk_widget_queue_draw_area (redraw, 0, 0, 2000, 2000);
  return TRUE;
}

int mid = 0;
int first_time = 0;
int jump_x = 50;
int jump_y = 50;

int current_x = 0;
int current_y = 0;


static gboolean
mp (GtkWidget * widget, gpointer data)
{


  mid = 1;
  dom (current_x, current_y);
  ox = current_x;
  oy = current_y;
  gtk_widget_queue_draw_area (redraw, 0, 0, 2000, 2000);
  return TRUE;
}


static gboolean
mr (GtkWidget * widget, GdkEventMotion * event)
{
  //printf("button rel.\n");
  mid = 0;
  to_drag = 0;
  first_time = 0;
  return TRUE;
}


// mouse motion
// to_drag == [1,2,3] each have a different effect
// 1: move the browser
// 2: move a scroll bar up or down
// 3: change colour and hue

static gboolean
mouseMove (GtkWidget * widget, GdkEventMotion * event)
{
  int mx, my;
  int lp;
  int dx = 0;
  int dy = 0;
  int colch = 0;
	int proceed=0;int ask=0;
  double a, b;
  int new_col;

  current_x = event->x;
  current_y = event->y;
  if (mid == 1)
    {
//
ask=0;
ask=past_particle(current_x,current_y);
proceed=1;
if (ask!=0){proceed=0;}
//
if (proceed==1){
//
      mx = event->x;
      my = event->y;

      if (first_time == 0)
	{

	}
      else
	{


	  dx = mx - ox;
	  dy = my - oy;
	  allwidth = mx;


	  if (to_drag == 1)
	    {
	      for (lp = 1; lp <= 20; lp++)
		{
		  bx[lp] = oldbx[lp] + dx;
		  by[lp] = oldby[lp] + dy;
		}
	    }
	  if (to_drag == 2)
	    {
	      by[which_s_p] = oldby[which_s_p] + dy;
	    }
	  if (to_drag == 3)
	    {
	      bx[which_s_p] = oldbx[which_s_p] + dx;
	      colch = which_s_p - 7;
	      if (colch >= 1)
		{
		  if (colch <= 3)
		    {
		      a = (double) (bx[which_s_p] - bx[1]);
		      b = a * (double) 65000 / ((double) (bx[4] - bx[1]));
		      new_col = (int) b;
		      if (new_col < 1)
			{
			  new_col = 1;
			};
		      if (new_col > 65000)
			{
			  new_col = 65000;
			};
		      pal[colch] = new_col;
		    }
		}
	    }


	}
      first_time = 1;
    }
}

//

  gtk_widget_queue_draw_area (redraw, 0, 0, 2000, 2000);
  return TRUE;
}

// draw canvas , draw browser then .

gint
drawCanvas (GtkWidget * widget, GdkEventExpose * eevt, gpointer data)
{

  GdkRegion *gr;
  GdkRectangle glb;
  GdkGC *r;

  r = widget->style->black_gc;

  glb.x = 0;
  glb.y = 0;
  glb.width = 1200;
  glb.height = 1200;

  gr = gdk_region_rectangle (&glb);
  gdk_gc_set_clip_region (r, gr);


  drawBrowser (widget);handle_past(widget);
  return TRUE;
}

static void
bye (GtkWidget * widget, gpointer data)
{
  int lp;

  gtk_main_quit ();

  printf ("\n\n");
  for (lp = 1; lp <= 3; lp++)
    {
      printf ("%d\n", pal[lp]);
    }
}

int
main (int argc2, char *argv2[])
{
  GtkWidget *window;
  GtkWidget *canvas;
  GtkWidget *box;
  int h, v;
//
	spinup();
//

  kern_init ();
  strcpy (current_class_token, "person");
  strcpy (current_method_token, "playsport");
  strcpy (msg_box, "");
  strcat (msg_box, current_class_token);
  strcat (msg_box, " ");
  strcat (msg_box, current_method_token);
  strcat (msg_box, " .");

  strcpy (work_space, "..........................................");
  load_spaces ();

  init ();
  read_model ();
  refresh_view ();
  printf ("Starting...\n");

  gtk_init (&argc2, &argv2);

  printf ("Gtk inited ...\n");

load_images();

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  gtk_widget_show_all (window);
  h = 599;
  v = 399;

  redraw = window;
  g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (bye), NULL);
  g_signal_connect (G_OBJECT (window), "key_press_event",
		    G_CALLBACK (keyPressed), NULL);

  canvas = gtk_drawing_area_new ();
  gtk_widget_set_size_request (GTK_WIDGET (canvas), h, v);

  gtk_signal_connect (GTK_OBJECT (canvas), "expose_event",
		      GTK_SIGNAL_FUNC (drawCanvas), NULL);
  gtk_signal_connect (GTK_OBJECT (canvas), "motion_notify_event",
		      GTK_SIGNAL_FUNC (mouseMove), NULL);
  g_signal_connect (G_OBJECT (window), "button-press-event", G_CALLBACK (mp),
		    NULL);
  g_signal_connect (G_OBJECT (window), "button-release-event",
		    G_CALLBACK (mr), NULL);
  gtk_widget_set_events (canvas,
			 GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_MASK |
			 GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);

  box = gtk_hbox_new (TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box), canvas, TRUE, TRUE, 0);

  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (window), box);
  gtk_widget_show (box);

  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
