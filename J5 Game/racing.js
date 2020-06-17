var x_racecar,y_racecar,x_box,y_box,count,score_count,life,life_r;
function setup()
{
	createCanvas(500,500);
	x_racecar = width/2;
	y_racecar = height-100;
	x_box = random(160,width-160)
	y_box = 20;
	count =0;
	score_count = 0;
	//life = 3;
	life_r = 30;
} 

function draw()
{
background(255);
racecar(x_racecar,y_racecar); // draw racecar
//box(x_box,y_box);
fill(0)
rect(x_box,y_box,30,30); // draw box
y_box += 8;
if(y_box>width-100)
{
	y_box = 50;
	x_box = random(150,width-150)
	score_count++;
}

noStroke();
fill(0,250,0)
rect(0,0,100,height)
rect(width-100,0,100,height)
scoreboard()
car_crash();
wall_crash();
fill(0);
//if(lives=0)
//{
//	fill(0);
 //  textSize(30);
  // textFont("helevetica");
  // text("gameover",0,width/2);
   

//}
ellipse(70,70,life_r);
if(life_r <= 0)
{
	score_count = 0;
	fill(255,0,0);
    textSize(30);
   	textFont("helevetica");
  	text("try again",80,width/2);
	life_r = 30;
}

/*if(count>0)
{
	fill(random(255),random(255),random(255))
	ellipse(50,50,20)
}*/

}
function racecar(xcar,ycar)
{	
	push();
		translate(xcar,ycar)
		scale(0.5)
		fill(0);
		noStroke();
		ellipse(0,0,24,34);//wheel top left
		rect(0,-2,60,4);//top connection wheels
		ellipse(60,-1,24,34);// wheel top right
		rect(5,-40,50,15);// top guard
		ellipse(-20,65,24,34);
		rect(-20,65,100,4)
		ellipse(80,65,24,34);
		fill(125);
		rect(18,-40,25,80);//body
		arc(30,40,80,80,0,PI);//body
		rect(-10,70,80,20);// back guard
		fill(255);
		rect(21,40,20,30)
		fill(255,0,0)
		ellipse(31,55,20)
	pop();
}
//function box(xbox,ybox)
//{	
	//fill(0);
	//rect(xbox,ybox,50);
	//xbox += -50;

//}
function keyPressed()
{
	if(keyCode === LEFT_ARROW)
	{
		x_racecar = x_racecar - 20;
	}
	else if(keyCode === RIGHT_ARROW)
	{
		x_racecar = x_racecar + 20;
	}
	

}

function car_crash()
{
	if(y_box + 50> y_racecar)
	{
		if(x_racecar > x_box&& x_racecar-12< x_box+30 || x_racecar-40+84>x_box && x_racecar-12+84<x_box+100)	//if y < thing_starty+thing_height   //if x > thing_startx and x < thing_startx + thing_width or x+car_width > thing_startx and x + car_width < thing_startx+thing_width: Crashing algorithm
           
		{
          
                
			fill(0)
			count ++;
			//lives--;
			
			life_r -= 2;



			
			//textSize(50);
   			//textFont("helevetica");
   			//text("YOU CRASHED",10,50);
		}
	}
}
function wall_crash()
{
	if(x_racecar+67>width-100 || x_racecar<130)
	{	
		fill(255,0,0)
		textSize(50);
   		textFont("helevetica");
   		text("YOU CRASHED!!!",40,width/2);
   		score_count = 0;
   		life_r = 30;
	}
}
function scoreboard()
{	
   fill(0);
   textSize(30);
   textFont("helevetica");
   text("score:",0,30);
   text(score_count,80,30)

   fill(0);
   textSize(30);
   textFont("helevetica");
   text("life:",0,80);
   //text(life,80,60)
}

