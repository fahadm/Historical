#include <allegro.h>
#include <stdio.h>
#include "version.h"
/* GLOBAL DEFINES */
#define  DEGREES(x) int((x)/360.0*0xFFFFFF)
#define OBJ 300
#define Powerups 7
#define max(a, b) (((a) > (b)) ? (a): (b))
#define min(a, b) (((a) < (b)) ? (a): (b))
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
/* GLOBAL DEFINES */
/* TYPE DEFINES */
typedef short int cordinates;
typedef short int Ecord;
/* TYPE DEFINES */
/*GLOBAL DECLARATIONS */
BITMAP *carh;
BITMAP *car1;
BITMAP *car2;
BITMAP *active_page;
BITMAP *fsprite;
BITMAP *bigs;
BITMAP *enemy;
BITMAP *enemy1;
BITMAP *enemy2;
BITMAP *enemy3;
BITMAP *enemyweak;
BITMAP *foodmap;
BITMAP *Psprite;
short int x=600;
short int y=400;
short int x2=0,y2=0;
short int n=1;
short int frame=0;
float fps=0;
short int counter=0;
cordinates fx[300]= {};
cordinates fy[300]= {};
cordinates Px[7]= {};
cordinates Py[7]= {};
Ecord Ex=380,Ey=262,E1x=380,E1y=262,E2x=380,E2y=262,E3x=380,E3y=262
                                        ,Emovex=0,Emovey=0;
bool DirDisable[4]= {};
short int dirx=0,diry=0;
short int Edy=-5,Edx=0,E1dy=-5,E1dx=0,E2dy=-5,E2dx=0,E3dy=-5,E3dx=0;
short int rem=0;
short int lives=3;
short int all_reset=0;
short int enemy_position_reset=1;
short int Power_food=0;
short int detect=0;
short int positionx=0;
short int frameskip=1;
bool check[7]= {};
short int speed=4;
bool endgame=false;
int foodcount=0;
int levelNo=1;
int r=33,g=33,b=222;
/*GLOBAL DECLARATIONS */
/* STRUCT DECLARATIONS*/
typedef struct FOOD
{
  short int xmin1:11;
  short int ymin1:11;
  short int xmin2:11;
  short int ymin2:11;
  short int xmax1:11;
  short int ymax1:11;
  short int xmax2:11;
  short int ymax2:11;
  bool disabled;


};
FOOD levelone[OBJ];
FOOD powerup[Powerups];
/* STRUCT DECLARATIONS*/


/*FUNCTION DECLARATIONS */
void init();
void deinit();
void carhandle();
void spritecontrol(int n);
void input();
void processing();
void output();
void loading();
void collision(FOOD &object);
void _Food_control();
void _get_cordinates();
void _collision_check();
int _color_collide(int xmin1, int ymin1, BITMAP *sprite1, int xmin2, int ymin2, BITMAP *sprite2);
int _Ecolor_collide(int xmin1, int ymin1);
void EnemyModule();
void Enemy_move();
int ColDir();
void Enemy_draw();
int Ecomparex(Ecord &posx);
int Ecomparey(Ecord &posy);
int random(int n);
void spritelooper();
void foodmapper();
void Enemy1_move();
void Enemy2_move();
void Enemy3_move();
int detect_collision(BITMAP* sprite,int xmin,int ymin);
void Reset_all_BF();
void master_move();
void reverse_dir(Ecord &a,Ecord &b);
bool LevelCompleted();
/*FUNCTION DECLARATIONS */

/*TIMER FUNCTIONS */
volatile int z;
void t()
{
  z++;
}
END_OF_FUNCTION(t)
volatile int p;
void power()
{
  p++;
}
END_OF_FUNCTION(power);
/*TIMER FUNCTIONS */

/*MAIN START */
int main()
{
  int l=0;
  init();
  loading();

  while (!key[KEY_ESC] && !endgame)
    {
        if (key[KEY_J]) foodcount--;

          if (detect ==0)
            input();
          processing();



      output();
      if (z >= 100 )
        {
          fps = (100. * frame) / (z+1);
          z = 0;

          frame = 0;
        }
      if (lives<0) endgame = true;
      if(LevelCompleted())
        {
          deinit();
          levelNo++;
          loading();

        }


      //rest(0);
    }//end main loop

  deinit();
  set_gfx_mode(GFX_TEXT,0, 0, 0, 0);
  clear_keybuf();
  allegro_exit();
  return 0;
}
END_OF_MAIN()
/*MAIN END */
void init()
{
  int depth, res;
  allegro_init();
  depth = desktop_color_depth();
  if ( depth==8)
    {
      allegro_message("This game doesnt run in 256 colors");
      exit(-1);
    }
  if (depth == 0) depth = 16;
  set_color_depth(depth);
  res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
  if (res != 0)
    {
      allegro_message(allegro_error);
      exit(-1);
    }

  install_timer();
  install_keyboard();
  install_mouse();
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0) ;
  LOCK_VARIABLE(z);
  LOCK_FUNCTION(t);
  install_int_ex(t,10000);
  LOCK_VARIABLE(p);
  LOCK_FUNCTION(power);
  install_int_ex(power,BPS_TO_TIMER(1));
}

void deinit()
{
    if (car1!= NULL && car2!= NULL &&fsprite!= NULL && bigs!= NULL && Psprite!= NULL && enemy!= NULL && enemy1!= NULL && enemy2!= NULL && enemy3!= NULL && enemyweak!= NULL && active_page!= NULL )
 {

  destroy_bitmap(car1);
  car1=NULL;
  destroy_bitmap(car2);car2=NULL;
  destroy_bitmap(fsprite);fsprite = NULL;
  destroy_bitmap(Psprite);Psprite = NULL;
  destroy_bitmap(enemy);enemy = NULL;
  destroy_bitmap(bigs);bigs = NULL;
  destroy_bitmap(enemy1);enemy1 = NULL;
  destroy_bitmap(enemy2);enemy2 = NULL;
  destroy_bitmap(enemy3);enemy3 = NULL;
  destroy_bitmap(enemyweak);enemyweak = NULL;
  destroy_bitmap(active_page);active_page = NULL;
 }


  /* add other deinitializations here */
}

void carhandle()
{
  if ( carh==car1)
    {
      carh=car2;
    }
  else
    carh=car1;
}

void spritecontrol(int n)
{
  switch(n)
    {
    case 1:
      draw_sprite(active_page,carh,x,y);
      break;
    case 2:
      draw_sprite_h_flip(active_page, carh, x, y);
      break;
    case 3:
      rotate_sprite(active_page, carh, x,y, DEGREES(90));
      break;
    case 4:
      rotate_sprite(active_page, carh, x,y, DEGREES(-90));
      break;
    }

}
void input()
{
  bool input_check=false;


  if ( _Ecolor_collide(x,y) ==1)
    {
      if ( dirx>0)
        {
          x-=5;
          DirDisable[LEFT]=true;
          dirx=0;
        }
      else if (diry>0)
        {
          y-=5;
          DirDisable[UP]=true;
          diry=0;
        }
      else if ( dirx<0)
        {
          x+=5;
          DirDisable[RIGHT]=true;
          dirx=0;
        }
      else if (diry<0)
        {
          y+=5;
          DirDisable[DOWN]=true;
          diry=0;
        }
      diry=0;
      dirx=0;

    }


  if ( key[KEY_LEFT] && DirDisable[LEFT]==false)
    {
      dirx=-5;
      diry=0;
      input_check=true;

      //carhandle();
      n=2;

    }
  else if ( key[KEY_RIGHT] && DirDisable[RIGHT]==false   )
    {

      dirx=5;
      diry=0;
      input_check=true;
      //carhandle();
      n=1;
    }
  else if ( key[KEY_UP] && DirDisable[UP]==false )
    {

      diry=-5;
      dirx=0;
      input_check=true;
      //carhandle();
      n=4;
    }
  else if ( key[KEY_DOWN] && DirDisable[DOWN]==false )
    {
      diry=5;
      dirx=0;
      input_check=true;
      //carhandle();
      n=3;
    }

  if (dirx!=0  && input_check==false)
    {
      //    carhandle();
    }
  if (diry!=0 && input_check==false )
    {
      //      carhandle();
    }
  if ( _Ecolor_collide(x,y) ==0)
    for ( counter=0; counter<4; counter++)
      {
        DirDisable[counter]=false;
      }
  x+=dirx;
  y+=diry;

  if (key[KEY_PLUS_PAD])
    {
      frameskip++;

    }
  if (key[KEY_MINUS_PAD])
    {
      frameskip--;
      if ( frameskip==0)
        {
          frameskip=1;
        }

    }
  carhandle();
  clear_keybuf();
  clear_keybuf();


}
void processing()
{
  spritecontrol(n);
  _get_cordinates();
  _collision_check();
  _Food_control();
  EnemyModule();

  spritelooper();
  if (Power_food==1)
    {
      speed = 2 ;
    }
  else
    speed = 5 ;
  if ( lives ==0)
    {
      endgame = true;
    }

  if (all_reset==1)
    {
      Reset_all_BF();
      rest(500);
    }

  if ( p>=10)
    {
      Power_food=0;

    }
  if ( key[KEY_S])
    {
      rest(20);
      rest(30);
      rest(20);
      rest(30);
      rest(20);
      rest(30);
      rest(20);
      rest(30);
    }


}
void output ()
{
  Enemy_draw();
  draw_sprite(active_page,bigs,0,0);
  textprintf_ex(active_page, font, 100, 100, makecol(255, 255, 255),-1, "fps %f %d %d", fps,z, _color_collide(0,0,bigs,Ex,Ey,enemy));
  textprintf_ex(active_page, font, 100, 120, makecol(255, 255, 255),-1, " Sprite level %d %d",detect,p);
  textprintf_ex(active_page, font, 100, 140, makecol(255, 255, 255),-1, " LIVES   %d foodcount %d",lives,foodcount);

  blit(active_page,screen,0,0,0,0,SCREEN_W,SCREEN_H);
  frame++;
  clear_to_color(active_page,makecol(92,032,044));
  vsync();
}
void loading()
{
  freopen("Settings.ini","r",stdin);
  char type[15]=" ";
  char name[20]=" ";
  int __counter=1;
  if ( levelNo >1)
    {
      do
        {
          scanf("%s",type);
          if (strcmp(type,"END")==0)
          __counter ++;
           else if (strcmp(type,"ENDGAME")==0)
        {
          endgame = true;
          return;
        }

        }
      while ( __counter <levelNo );
    }

  for ( counter = 0 ; counter < OBJ; counter++)
    {
      levelone[counter].disabled= false;
    }
  for ( counter = 0 ; counter < Powerups; counter++)
    {
      powerup[counter].disabled= false;
      check[counter]=false;
    }
  car1 = load_bitmap("Data\\car.bmp",0);
  car2 = load_bitmap("Data\\car2.bmp",0);
  fsprite = load_bitmap("Data\\food.bmp",0);
  enemy= load_bitmap("Data\\enemy.bmp",0);
  enemy1= load_bitmap("Data\\enemy1.bmp",0);
  enemy2= load_bitmap("Data\\enemy2.bmp",0);
  enemy3= load_bitmap("Data\\enemy3.bmp",0);
  Psprite=load_bitmap("Data\\powerup.bmp",0);
  enemyweak=load_bitmap("Data\\weak.bmp",0);

  while (!endgame)
    {
      scanf("%s ",type);
      printf("%s",type);
      if (strcmp(type,"END")==0)
        break;
      if (strcmp(type,"ENDGAME")==0)
        {
          endgame = true;
          return;
        }
      if (strcmp(type,"Background=>")==0)
        {
          scanf("%s",name);
          if (strcmp(name,"Default"))
            bigs = load_bmp("Data\\bs2.bmp",0);
          else
            bigs = load_bmp(name,0);
        }
      else if (strcmp(type,"FoodMap=>")==0)
        {
          scanf("%s",name);
          if (strcmp(name,"Default"))
            foodmap = load_bitmap("Data\\2bs21.bmp",0);
          else
            foodmap = load_bitmap(name,0);

        }
      else if (strcmp(type,"Collision_Color=>")==0)
        {
          scanf(" %d %d %d",&r,&g,&b);


        }

    }





  // foodmap = load_bitmap("Data\\2bs21.bmp",0);

  if ( (bigs== NULL || car1==NULL || car2==NULL || fsprite==NULL || enemy==NULL || enemy1==NULL|| enemy2==NULL||enemy3==NULL||Psprite==NULL||enemyweak==NULL|| foodmap==NULL || fsprite==NULL ))
    {
      allegro_message("Media files are missing");
      exit(-1);
    }
  carh=car1;
  active_page=create_system_bitmap(SCREEN_W,SCREEN_H);
  clear_to_color(active_page,makecol(220,150,90));
  masked_blit(bigs, active_page, 0,0,0,0, bigs->w,bigs->h);
  foodmapper();
  destroy_bitmap(foodmap);
}
void collision(FOOD &object)
{
  if ( object.xmin1 <object.xmax2 && object.xmin2 <object.xmax1 &&
       object.ymin1 <object.ymax2 && object.ymin2 <object.ymax1 && !object.disabled )
    {
      object.disabled= true ;
      foodcount--;
      printf("%d",foodcount);
    }
}

void _Food_control()
{
  for ( counter = 0 ; counter < OBJ ; counter ++)
    {
      if ( levelone[counter].disabled ==false)
        draw_sprite(active_page,fsprite,levelone[counter].xmin1,levelone[counter].ymin1);
    }

  for ( counter = 0 ; counter < Powerups ; counter ++)
    {
      if ( powerup[counter].disabled ==false)
        draw_sprite(active_page,Psprite,powerup[counter].xmin1,powerup[counter].ymin1);

      else
        {
          if (check[counter]== false)
            {
              Power_food=1;
              p=0;
              speed=2;
              reverse_dir(Edx,Edy);
              reverse_dir(E1dx,E1dy);
              reverse_dir(E2dx,E2dy);
              reverse_dir(E3dx,E3dy);
              check[counter]= true;
            }


        }
    }


}
void _get_cordinates()
{
  for ( counter = 0 ; counter < OBJ; counter++)
    {
      levelone[counter].xmin1=fx[counter]- fsprite->w;
      levelone[counter].ymin1=fy[counter]- fsprite->h;
      levelone[counter].xmin2=x;
      levelone[counter].ymin2=y;
      levelone[counter].xmax1=fx[counter];
      levelone[counter].ymax1=fy[counter];
      levelone[counter].xmax2=x+ carh->w ;
      levelone[counter].ymax2=y+carh->h ;

    }

  for ( counter = 0 ; counter < Powerups; counter++)
    {
      powerup[counter].xmin1=Px[counter]- Psprite->w;
      powerup[counter].ymin1=Py[counter]- Psprite->h;
      powerup[counter].xmin2=x;
      powerup[counter].ymin2=y;
      powerup[counter].xmax1=Px[counter];
      powerup[counter].ymax1=Py[counter];
      powerup[counter].xmax2=x+ carh->w ;
      powerup[counter].ymax2=y+carh->h ;

    }



}
void _collision_check()
{
  for ( counter = 0 ; counter <OBJ ; counter++)
    {
      collision(levelone[counter]);
    }
  for ( counter = 0 ; counter <Powerups ; counter++)
    {
      collision(powerup[counter]);
    }
}
int _color_collide(int xmin1, int ymin1, BITMAP *sprite1, int xmin2, int ymin2, BITMAP *sprite2)
{

  int xmax1 = xmin1 + sprite1->w, ymax1 = ymin1 + sprite1->h;
  int xmax2 = xmin2 + sprite2->w, ymax2 = ymin2 + sprite2->h;
  int xmin = max(xmin1, xmin2);
  int ymin = max(ymin1, ymin2);
  int xmax = min(xmax1, xmax2);
  int ymax = min(ymax1, ymax2);
  if (xmax <= xmin || ymax <= ymin)
    {
      return 0;
    }
  int mask1 = bitmap_mask_color(sprite1);
  int mask2 = bitmap_mask_color(sprite2);
  for (int y = ymin; y < ymax; y++)
    {
      for (int x = xmin; x < xmax; x++)
        {
          int x1 = x - xmin1, y1 = y - ymin1;
          x2 = x - xmin2, y2 = y - ymin2;
          int color1 = getpixel(sprite1, x1, y1);
          int color2 = getpixel(sprite2, x2, y2);
          if (sprite2==enemy)
            {

              if (color1 != mask1 && color2 != mask2 && color1 == makecol(r,g,b))
                {
                  return 1;
                }
            }
          else
            {
              if (color1 != mask1 && color2 == mask2 && color1 == makecol(r,g,b))
                {
                  return 1;
                }
            }
        }
    }
  return 0;
}


int _Ecolor_collide(int xmin1, int ymin1)
{
  int pixel;
  pixel=getpixel(bigs,xmin1,ymin1);


  for (int i=ymin1; i<=ymin1+carh->h; i++)
    {
      for (int j=xmin1; j<=xmin1+carh->w; j++)
        {
          pixel=getpixel(bigs,j,i);
          if ( pixel == makecol(r,g,b))
            {
              return 1;
            }

        }
    }



  return 0;

}





void EnemyModule()
{


  detect=detect_collision(enemy,Ex,Ey);
  detect=detect_collision(enemy1,E1x,E1y);
  detect=detect_collision(enemy2,E2x,E2y);
  detect=detect_collision(enemy3,E3x,E3y);

  if (detect == 0)
    {

      Enemy_move();

      Enemy1_move();

      Enemy2_move();

      Enemy3_move();
    }
  else
    {
      //Edx=0,E1dx=0,E2dx=0,E3dx=0;
      //Edy=0,E1dy=0,E2dy=0,E3dy=0;

    }

  switch (detect)
    {
    case 1:
      Ecomparey(Ey);
      Ecomparex(Ex);
      break;
    case 2:
      Ecomparey(E1y);
      Ecomparex(E1x);
      break;
    case 3:
      Ecomparey(E2y);
      Ecomparex(E2x);
      break;
    case 4:
      Ecomparey(E3y);
      Ecomparex(E3x);
      break;
    default:
      break;
    }
  master_move();
  Enemy_draw();
//////////////////////////////////////////////////////////  //Ecomparey();
//////////////////////////////////////////////////////////    Ecomparex();

}

void Enemy_move()
{
  if ( (_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ) && Edy<0)
    {
      while ((_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ))
        Ey+=5;
      Edx=speed*random(7);
      Edy=0;

    }
  else if ( (_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ) && Edy>0)
    {
      while ((_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ))
        Ey-=5;
      Edx=-speed*random(7);
      Edy=0;

    }
  else if( (_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ) && Edx<0)
    {
      while ((_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ))
        Ex+=5;
      Edx=0;
      Edy=-speed*random(7);

    }
  else if( (_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ) && Edx>0)
    {
      while ((_color_collide(0, 0,bigs,Ex,Ey,enemy) == 1 ))
        Ex-=5;
      Edx=0;
      Edy=speed*random(7);

    }


}

void Enemy_draw()
{
  if (Power_food == 1)
    {

      draw_sprite(active_page,enemyweak,Ex,Ey);
      draw_sprite(active_page,enemyweak,E1x,E1y);
      draw_sprite(active_page,enemyweak,E2x,E2y);
      draw_sprite(active_page,enemyweak,E3x,E3y);

    }
  else
    {

      draw_sprite(active_page,enemy,Ex,Ey);
      draw_sprite(active_page,enemy1,E1x,E1y);
      draw_sprite(active_page,enemy2,E2x,E2y);
      draw_sprite(active_page,enemy3,E3x,E3y);
    }
}
int random(int n)
{
  rem++;
  if ( n==3)
    {
      if ( rem%n==0)
        {
          return 1;
        }
      return -1;


    }

  if ( rem%(1+rand()%(n+1))==0)
    {
      return 1;
    }
  return -1;



}




int Ecomparex(Ecord &posx)
{
  int xdistance = abs(posx-380);
  if (posx > 380)
    {
      posx--;
    }
  int newxdistance= abs(posx-380);

  if  ( newxdistance < xdistance  )
    {

      Emovex=-5;
    }

  else
    {
      Emovex=5;

    }

  if ( posx==Ex)
    Edx=Emovex;
  else if ( posx==E1x)
    E1dx=Emovex;
  else if ( posx==E2x)
    E2dx=Emovex;
  else if ( posx==E3x)
    E3dx=Emovex;
  positionx=posx;
}

int Ecomparey(Ecord &posy)
{
  int ydistance = abs(posy-262);
  if (posy > 262)
    {
      posy--;
    }
  int newydistance= abs(posy-262);
  if  ( newydistance < ydistance  )
    {
      Emovey=-5;
    }
  else
    Emovey=5;

  if ( posy==262 || positionx==380)
    {

      detect =0;
      if ( &posy==&Ey)
        {
          Ex=380;
          Ey=262;

        }

      else if ( &posy==&E1y  )
        {
          E1x=380;
          E1y=262;

        }
      else if ( &posy==&E2y  )
        {
          E2x=380;
          E2y=262;

        }
      else if ( &posy==&E3y  )
        {
          E3x=380;
          E3y=262;
        }

      Emovey=-15;
      Emovex=0;
      master_move();
      if ( &posy==&Ey)
        {

          Edy=Emovey;
          Edx=Emovex;
        }
      else if ( &posy==&E1y)
        {


          E1dy=Emovey;
          E1dx=Emovex;
        }
      else if ( &posy==&E2y)
        {


          E2dy=Emovey;
          E2dx=Emovex;
        }
      else if ( &posy==&E3y)
        {


          E3dy=Emovey;
          E3dx=Emovex;
        }

      master_move();
      detect=0;
    }
  if ( posy==Ey)
    {

      Edy=Emovey;
      Edx=Emovex;
    }
  else if ( posy==E1y)
    {


      E1dy=Emovey;
      E1dx=Emovex;
    }
  else if ( posy==E2y)
    {


      E2dy=Emovey;
      E2dx=Emovex;
    }
  else if ( posy==E3y)
    {


      E3dy=Emovey;
      E3dx=Emovex;
    }


//   if (detect==0)



}

void spritelooper()
{
  if ( x< (0- (carh->w)))
    x=800+ (carh->w);
  else if ( x> 800+carh->w)
    x=0- (carh->w);

}
void foodmapper()
{
  int i=0;
  int j=0;
  int count=0;
  int pcount=0;
  int pix;
  while ( j<600)
    {
      i=0;
      while ( i <800)
        {
          pix=getpixel(foodmap,i,j);
          if ( pix == makecol(255,146,172) || pix == makecol(255,255,255) )
            {
              fx[count]=i;
              fy[count]=j;
              count++;
            }
          else if ( pix == makecol(69,69,69))
            {
              Px[pcount]=i;
              Py[pcount]=j;
              pcount++;
            }

          i++;
        }
      j++;
    }
  foodcount = count+ pcount;

}
////////////////////////////////////////////////////////////////////




void Enemy1_move()
{
  if ( (_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ) && E1dy<0)
    {
      while ((_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ))
        E1y+=5;
      E1dx=-speed*random(5);
      E1dy=0;

    }
  else if ( (_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ) && E1dy>0)
    {
      while ((_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ))
        E1y-=5;
      E1dx=speed*random(5);
      E1dy=0;

    }
  else if( (_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ) && E1dx<0)
    {
      while ((_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ))
        E1x+=5;
      E1dx=0;
      E1dy=speed*random(5);

    }
  else if( (_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ) && E1dx>0)
    {
      while ((_color_collide(0, 0,bigs,E1x,E1y,enemy) == 1 ))
        E1x-=5;
      E1dx=0;
      E1dy=-speed*random(5);

    }


}
////////////////////////////////////////////////////////////////////

void Enemy2_move()
{
  if ( (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 ) && E2dy<0)
    {
      while ((_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 ))
        E2y+=5;
      E2dx=-speed*random(13);
      E2dy=0;

    }
  else if ( (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 ) && E2dy>0)
    {
      while (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 )
        E2y-=5;
      E2dx=speed*random(11);
      E2dy=0;

    }
  else if( (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 ) && E2dx<0)
    {
      while (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 )
        E2x+=5;
      E2dx=0;
      E2dy=speed*random(7);

    }
  else if( (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 ) && E2dx>0)
    {
      while (_color_collide(0, 0,bigs,E2x,E2y,enemy) == 1 )
        E2x-=5;
      E2dx=0;
      E2dy=-speed*random(3);

    }


}

////////////////////////////////////////////////////////////////////
void Enemy3_move()
{
  if ( (_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1 ) && E3dy<0)
    {
      while((_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1))
        E3y+=5;

      E3dx=speed*random(rand()%255);
      E3dy=0;

    }
  else if ( (_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1 ) && E3dy>0)
    {
      while((_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1))
        E3y-=5;
      E3dx=-speed*random(rand()%255);
      E3dy=0;

    }
  else if( (_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1 ) && E3dx<0)
    {
      while((_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1))
        E3x+=5;
      E3dx=0;
      E3dy=-speed*random(rand()%255);

    }
  else if( (_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1 ) && E3dx>0)
    {
      while((_color_collide(0, 0,bigs,E3x,E3y,enemy) == 1))
        E3x-=5;
      E3dx=0;
      E3dy=speed*random(29);

    }


}
int detect_collision(BITMAP* sprite,int xmin,int ymin)
{
  if ( ( xmin<x+carh->w && ymin <y+ carh->h && x< xmin+sprite->w  && y<ymin +sprite->h ) && Power_food==1)
    {
      enemy_position_reset=1;
      if ( sprite == enemy)
        return 1;
      else if ( sprite == enemy1)
        return 2;
      else if ( sprite == enemy2)
        return 3;
      else if ( sprite == enemy3)
        return 4;
    }
  else if ( xmin<x+carh->w && ymin <y+ carh->h && x< xmin+sprite->w  && y<ymin +sprite->h )
    {
      lives-=1;
      all_reset=1;

    }

  return detect;
}

void Reset_all_BF()
{
  Ex=380,Ey=262,E1x=380,E1y=262,E2x=380,E2y=262,E3x=380,E3y=262;
  Edy=-5,Edx=0,E1dy=-5,E1dx=0,E2dy=-5,E2dx=0,E3dy=-5,E3dx=0;
  Power_food=0;
  all_reset=0;
  x=380,y=425;

}
void master_move()
{
  if (detect==0)
    {
      Ey+=Edy;
      Ex += Edx;
      E1y+=E1dy;
      E1x += E1dx;
      E2y+=E2dy;
      E2x += E2dx;
      E3y+=E3dy;
      E3x += E3dx;
    }
  else if (detect == 1)
    {
      Ey+=Edy;
      Ex += Edx;
    }
  else if (detect == 2)
    {
      E1y+=E1dy;
      E1x += E1dx;
    }
  else if (detect == 3)
    {
      E2y+=E2dy;
      E2x += E2dx;
    }
  else if (detect == 4)
    {
      E3y+=E3dy;
      E3x += E3dx;
    }

}
void reverse_dir(Ecord &a,Ecord &b)
{
  if ( a>0)
    a=-1*speed;
  else if ( a<0)
    a=1*speed;
  if ( b>0)
    b=-1*speed;
  else if (a<0)
    b=1*speed;

}
bool LevelCompleted()
{
  if(foodcount<=0)
    return true;
  else return false;

}
