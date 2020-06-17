var arcadeVX = 0
var arcadeSC = 1
	
var characterSC = 1
var x_character,y_character;

var x_racecar, y_racecar ,x_box, y_box, count, score_count, life, life_r;
var cond_win;

var racecarSC = 0.5
var flash_portal; 				// condition for portal to appear
var characterSideSC = 1
	
var legAngle
var moveLegsOut
var x_Scharacter,y_Scharacter;

var flash_count;
var cond_scene2;
var stopHere
	
var spiralX = []
var spiralY = []
var spiralRad = []
	
var snow1,snow2,snow3;

var x_Scharacter2, y_Scharacter;

var back1,back2,back3;

var monster=[], cxMonster=[], cyMonster=[], monsterMax, monsterVelocityX=[], monsterVelocityY=[], demodog=[]
var thetaDG
	
var upsideX = 0
var upsideY = -75
	
var dustinX = 125
var dustinY =  125
var dustinSC = 1

var lucasX = 375
var lucasY =  125
var lucasSC = 1

var willX = 125
var willY =  375
var willSC = 1

var mikeX = 375
var mikeY =  375
var mikeSC = 1

var vinePic;
var outsideOk;
var randRed=[], randX=[], randY=[], gradientRect
	
var move = 0;
var finalScene;

var drag_item,orbcX,orbcY,orbcR,itemN0;
var itemX = []
var itemY = []
var itemR = []
	
var krakenX = 250
var krakenY = 250
var krakenSC = 0.8
var redKraken=false
	
var batROT=0
var moveBat=false
var batDown=true
	
var elevenPop = false;
var countKill = 0;
var arcadeStart = false;

var waffleX = [];
var waffleY = [];
var dx = [];
var dy = [];
var num;

function preload()
{
 	 	monsterMax=5
  			for(i=0; i<monsterMax; i++){
    					demodog[i]=loadImage("demodog.png")
  					}
			
  			vinePic = loadImage("vines.png")
}

//---------------------------------------------------------------------------------function setup-----------------------------------------------------//

function setup()
{
        createCanvas(500, 500);
        background(255);
      		  x_Scharacter = 0;
       		  y_Scharacter = 350;
	
       		  x_racecar = width/2;
       		  y_racecar = height-100;
	
     	      x_box = random(160,width-160)
      		  y_box = 20;
	
       			 count =0;
       			 score_count = 0;
       			 life_r = 30;
        			cond_win =false;
        			flash_portal = false;
        			legAngle=0
        			moveLegsOut=true
        			x_character = width/2
        			y_character = 350;
        			flash_count = 1;
        			cond_scene2 = false;
        			stopHere = 0
        			x_Scharacter2 = 0;

  			   snow1 = new PSys(width/2, -100, 100);  // start a new particle system

   				back1 = true
   				back2 = true		// background changes
  				back3 = true
  				outsideOk = false;
  			 	finalScene = false;

    				thetaDG=0
    					    for(i=0; i<monsterMax; i++){
    										monster[i]=true
  										 demodog[i].loadPixels();
										 cxMonster[i]=100+75*i
  										 cyMonster[i]=300
   										 monsterVelocityX[i]=random(-2, 2)
   										 monsterVelocityY[i]=random(-2, 2)
   										 thetaDG+=2*PI/5
  								}

 				var d, res;
 				 createCanvas(500, 500);
 				 background(0);
 						 noStroke();
  								for(gradientRect=0; gradientRect<height*width/16; gradientRect+=1)
  										{
  												  randRed[gradientRect]=random(-50,50)
  												  randX[gradientRect]=random(-2,2)
												  randY[gradientRect]=random(-2,2)
										}
										
				stroke(0)
				drag_item = false;
  							orbcX = width/2;
  							orbcY = height/2;
  							orbcR = 80;
							itemNo = 6;
										
  				var d, res;
 							 for(var i=0;i<itemNo;i++)
								  {
  										  itemX.push(random(0,width-250))
  										  itemY.push(random(0,height-250))
   										  itemR.push(30)
								  }

				num = 20;

   							for (var i=0; i<num; i++)
  								 {
    										  waffleX.push(width/2);
     									  waffleY.push(height/2);
     									  dx.push(0);
     									  dy.push(0);
  								 }
								 
   						  for(var i=0; i<num; i++)
              		           {
             							 waffleX[i] = width/2;
                    					     waffleY[i] = height/2;
                    					     dx[i] = random(-2, 2);
                     					 dy[i] = random(-2, 2);
             				   }
}

function draw()
{
			drawOutside()

						if(arcadeStart)
							 {
        									drawArcade(arcadeVX , 0 )

       									arcadeVX -= 0

        									drawSingle()
        									drawCharacterSide(x_Scharacter,y_Scharacter) // draws main character
        									moveCharacterLegs()
								 
        											if(x_Scharacter<width/2-30){
        															x_Scharacter += 1
														}
													
												else if(flash_count<20)
														{   x_Scharacter = width/2   }

        											if(x_Scharacter>=width/2-30)
        													{
                													drawCharacter(x_character,y_character)
																
                													arcadeSC += 0.05
                															if(arcadeSC>=4)
               																 {y_Scharacter = -200}
               															if(arcadeSC>=5)
             																 {
                        																arcadeSC = 5 //moving character out of screen // start of racecar scenes

                       																background(255);
                       																racecar(x_racecar,y_racecar); // draw racecar
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
                								 		}
     		   }


 						if(score_count >= 10)
 									{
     										   frameRate(1)
      										   fill(255,0,0);
      										   textSize(30);
       										   textFont("helevetica");
       										   text("you won",85,width/2);
      										   cond_win =true
									}

						if(cond_win)
									{
       										   y_box = -100;
       										   arcadeSC = 1;
       										   drawArcade(0,0)
       										   drawSingle()
       										   stroke(0);
										
       													 if(flash_count<20)
      															  {
        																	y_character = 350;
   																  }
																  
        						drawCharacter(x_character,y_character)
				 }

 							if(cond_win)                           													//flashing scene
             						   {

                       							 if(flash_count<20)
                       										 {
                           										     flash();
                               										 flash_count+=0.5
                               										 console.log(flash_count)
                      										 }
              							 }

							if(flash_count==20) 																	// movement after flash
										{
        											y_character = -2000;
      											y_Scharacter = 350
      											x_Scharacter++;
       											drawCharacterSide(x_Scharacter,y_Scharacter)  // draws main character
       											moveCharacterLegs()
        											frameRate(60)
															push()
      															  stroke(0)
      															  strokeWeight(4)
      															  fill(87,39,87,180)
     															  ellipse(width-50,height-100,80,140)  //Portal

															pop()
				}
							if(x_Scharacter>width-80)
										{
       											 x_Scharacter = width-80
       											 cond_scene2 = true;
										}
							if(cond_scene2)
										{
        											frameRate(60)
      											drawArcade(arcadeVX , 0 )
       											drawupsidedowneffect()
        											snow1.run();										//Snow Effect
 
       										    if(x_Scharacter2<=10)
        											x_Scharacter2 = 10;

        											stroke(0);
       											drawCharacterSide2(x_Scharacter2,350)
        											x_Scharacter2 += 1;
										}
												if(x_Scharacter2>width)
															{
     															  back1 = false;
        															  drawupsideBackground()
  																  noStroke();
																
 																		 for(i=0; i<monsterMax; i++){
																			 
    																						if(monster[i]){
      																								image(demodog[i], cxMonster[i]-40, cyMonster[i]-15)
     																								fill(135, 0);
      																								ellipse(cxMonster[i], cyMonster[i], 30)
      																								cxMonster[i]+=monsterVelocityX[i]
      																								cyMonster[i]+=monsterVelocityY[i]
																								
      																											if(cxMonster[i]>width-25 || cxMonster[i]<25){
        																																monsterVelocityX[i]=-monsterVelocityX[i]
      																														}
																												
    																						if(cyMonster[i]>height-75 || cyMonster[i]<115){
        																								monsterVelocityY[i]=-monsterVelocityY[i]
     																											}
    															}
  						}
																		 
 							 drawBat(330,410,0.5)
							 x_character =width/2
  							 y_character =350
  						    drawCharacter()
																		 
							drawupsidedowneffect()
							snow1.run();
							stroke(0);
		}

							if(monsterVelocityX[0]+monsterVelocityX[1]+monsterVelocityX[2]+monsterVelocityX[3]+monsterVelocityX[4]==0)
										{
													  drawSchool()
       												  drawupsidedowneffect()
      												  snow1.run();
      												  stroke(0)
      												  drawMike(width-200, 350, 1)
      												  drawDustin(width-150, 350, 1)
        												  drawWill(width-80, 350, 1)
      												  drawLucas(width-40, 350, 1)

      												  x_Scharacter2 = 50
      												  drawCharacterSide2(x_Scharacter2,350)
											 
      												  fill(255);
   													  textSize(15);
   													  textFont("helevetica");
   													  text("PRESS A TO CONTINUE",0,30);
										 }
										
							if(outsideOk)
										{
   												     background(0)
     											     bossBG()
      												 outsideSchool(0, 250, 1.4)
      												 drawupsidedowneffect()
     												 snow1.run();
     												 stroke(0)
     												 drawCharacterSide2(x_Scharacter2+50+move,350)
     												 drawMike(width-200,350,1)
     												 drawDustin(width-150,350,1)
       												 drawWill(width-80,350,1)
     												 drawLucas(width-40,350,1)
     												 move++
											
        															if(x_Scharacter2+move+50>width)
        																			{
                																				finalScene = true;
        																			}
										}
							
							if(finalScene)
										{
													background(0);
													bossBG()

													drawKraken(krakenX, krakenY, krakenSC)
											
																  if(drag_item)
 																				 {
    																							frameRate(10);
 																				 }
 													 fill(255);
													 textSize(15);
													 textFont("helevetica");
													 text("PRESS H FOR HELP",0,30);
  													 noStroke();
 													 fill(255,255,255,150)
 																 ellipse(orbcX,orbcY,orbcR,orbcR)
														
													 stroke(0)
       												 drawMike(width-170,450,1)
       												 drawDustin(width-120,450,1)
       												 drawWill(width-80,450,1)
      												 drawLucas(width-40,450,1)
       												 x_character = 200
      												 y_character = 400
        												 drawCharacter()
																				 
      															  if(elevenPop)
        																				{
        																						 drawEleven(80,400,1)
     																						 noStroke();
																							
        																										for(var i=0;i<itemNo;i++)
  																															{
   																																	fill(random(255),random(255),random(255));
   																																	ellipse(itemX[i],itemY[i],itemR[i])

  																																				 if((orbcX-itemX[i])*(orbcX-itemX[i])+(orbcY-itemY[i])*(orbcY-itemY[i])-(orbcR*orbcR)<=0)
   																																								{
     																																										fill(255,255,255,150)
     																																										ellipse(orbcX,orbcY,orbcR,orbcR)
      																																										itemX[i] = -50;
      																																										redKraken=true
      																																										countKill++
    																																								 }
  																															 }

        																					}


        						 if(countKill>=itemNo)
        									{
             									   for (var i=0; i<num; i++)
   										{
      											
							push();
         					translate(waffleX[i], waffleY[i]);
         							noStroke();
         							fill(255, 185, 50);
         									ellipse(0, 0, 50);
         							fill(230, 140, 60);
         									rect(-23,-10,3,22);
         									rect(-17,-19,3,38);
         									rect(-11,-22,3,45);
         									rect(-5,-25,3,50);
         									rect(1,-25,3,50);
         									rect(7,-22,3,45);
         									rect(13,-19,3,38);
         									rect(19,-10,3,22);
         									rect(-17,17,35,3);
         									rect(-22,11,43,3);
         									rect(-25,5,50,3);
         									rect(-25,-1,50,3);
         									rect(-22,-7,45,3);
         									rect(-22,-13,42,3);
         									rect(-18,-19,33,3);
        						pop();

      						waffleX[i] += dx[i];
      						waffleY[i] += dy[i];
   			}
  							fill(255,255,0);
                				textSize(20);
                				textFont("helevetica");
                				text("CONGRATS YOU BEAT THE SHADOW MONSTER!!!", 20, width/2);
        		}
		}
	}
}

//-----------------------------------------------------------------------function draw---------------------------------------------------------------//

//-----------------------------------------------------------------------function drawArcade------------------------------------------------------//

function drawArcade(x, y)
{
		if(back1){
        				push();
                				translate(arcadeVX, 0)
                				scale(arcadeSC)

        						var arcadeX = 0

                				fill(255, 165, 0)
                        				rect(arcadeX, 0, 500, 500)

               	for (i = 0; i < 5; i++)
                		{
                        	fill(0); //body
                                rect(arcadeX, 100, 100, 300)
						
                        fill(255); //header
                                rect(arcadeX + 10, 110, 80, 30)
                        fill(250, 128, 114)
                                rect(arcadeX + 15, 115, 70, 20, 5)

                        fill(random(100, 150)); //screen
                                rect(arcadeX + 10, 170, 80, 80)

                        fill(128); //remote
                                rect(arcadeX + 18, 230, 2, 20)
                        fill(128, 0, 0)
                                ellipse(arcadeX + 20, 235, 10)
                                rect(arcadeX + 40, 248, 10, 2)
                                rect(arcadeX + 55, 248, 10, 2)
                                rect(arcadeX + 70, 248, 10, 2)

                        fill(0, 0, 205); //body detail
                                rect(arcadeX + 10, 280, 80, 100)
                        fill(255); //balls
                                ellipse(arcadeX + 30, 320, 10)
                                ellipse(arcadeX + 50, 320, 10)
                                ellipse(arcadeX + 70, 320, 10)

                        fill(255, 215, 0) //pacman
                                arc(arcadeX + 30, 320, 30, 30, PI/4, 7*PI/4, PIE)

                        arcadeX+=100
                		}
        pop();
	}

}
//-----------------------------------------------------------------------function drawSingle------------------------------------------------------//
function drawSingle()
{

if(back1)
{
        push();
                translate(250, 250)
                scale(arcadeSC);
                fill(100); //body
                                rect(-50, -110, 100, 300)

                        fill(255); //header
                                rect( -40, -100, 80, 30)
                        fill(250, 128, 114)
                                rect( -35, -95, 70, 20, 5)

                        fill(random(100, 150)); //screen
                                rect(-40, -40, 80, 80)

                        fill(128); //remote
                                rect(-32, 20, 2, 20)
                        fill(128, 0, 0)
                                ellipse( -30, 25, 10)
                                rect(-10, 38, 10, 2)
                                rect(5, 38, 10, 2)
                                rect(20, 38, 10, 2)

                        fill(255); //body detail
                                rect(-40, 70, 80, 100)
  
                        fill(0, 250, 0) //grass detail
                                rect(-40, 70, 10, 100)
                                rect(30, 70, 10, 100)

                        racecar(-12.5, 110)

                        if(cond_win)//win text
                        {
                                fill(255,255,0);
                                textSize(15);
                                textFont("copperplate");
                                text("YOU WON",-40,0);
                        }
             pop();
		}
}
//--------------------------------------------------------------------function drawCharacter-------------------------------------------------------------//
function drawCharacter()
{
        push();
                translate(x_character, y_character);
                scale(characterSC);
                        fill(222, 184, 135)

                        fill(70, 130, 180)
                                rect(0, 70, 20, 60, 20) //left leg
                        fill(0)
                                rect(0, 120, 20, 10)//left shoe

                        fill(70, 130, 180)
                                rect(40, 70, 20, 60, 20) //right leg

                        fill(0)
                                rect(40, 120, 20, 10) //rightshoe

                        fill(205, 92, 92)
                                rect(-10, 0, 10, 60) //left arm
                                rect(60, 0, 10, 60) //right arm
                                rect(0, 0, 60, 80) //body

                        fill(222, 184, 135)
                                ellipse(30, -20, 60) //head

                        fill(200) //beanie
                                rect(-5, -30, 70, 10)
                                arc(30, -25, 60, 60, PI, 2*PI)
        pop();
}
//-----------------------------------------------------------------------------function racecar------------------------------------//
function racecar(xcar,ycar)
{

if(back1){
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
}
//--------------------------------------------------------------function keyPressed--------------------------------------------------------//
function keyPressed() // race car stuff
{

if(back1){
        if(keyCode === LEFT_ARROW)
        {
                x_racecar = x_racecar - 20;
        }
        else if(keyCode === RIGHT_ARROW)
        {
                x_racecar = x_racecar + 20;
        }

	}
}
//-------------------------------------------------------------------function car_crash-------------------------------------------------------//
function car_crash() // race care stuff
{
        if(back1){
        if(y_box + 50> y_racecar)
        {
                if(x_racecar > x_box&& x_racecar-12< x_box+30 || x_racecar-40+84>x_box && x_racecar-12+84<x_box+100)    //if y < thing_starty+thing_height   //if x > thing_startx and x < thing_startx + thing_width or x+car_width > thing_startx and x + car_width < thing_startx+thing_width: Crashing algorithm

                {
                        fill(0)
                        count ++;

                        life_r -= 2;

                }
        }
	}
}
//-------------------------------------------------------------function wall_crash-------------------------------------------------------------//
function wall_crash() // race car stuff
{ if(back1){
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
}
//-----------------------------------------------------------function scoreboard------------------------------------------------------------------//
function scoreboard() //race car stuff
{
        if(back1)
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
		  }
}
//--------------------------------------------------------------------function moveCharacterLegs-------------------------------------------------//
function moveCharacterLegs()
{
  if(legAngle>PI/8)
    moveLegsOut=false
  if(legAngle<0)
    moveLegsOut=true
  if(moveLegsOut)
    legAngle+=PI/250
  else
    legAngle-=PI/250
}
//---------------------------------------------------------------------------------function drawCharacterSide--------------------------------------//
function drawCharacterSide(xCharacterSide, yCharacterSide)
{
  push();
    translate(xCharacterSide, yCharacterSide);
    scale(characterSideSC);
      fill(222, 184, 135)

    //Left Leg
    push();
      translate(15, 70)
      rotate(legAngle)
      fill(70, 130, 180)
        rect(-10, 0, 20, 60, 20) //left leg
      fill(0)
        rect(-10, 50, 20, 10)//left shoe
    pop();

    fill(205, 92, 92)
      rect(0, 0, 60, 80) //body
      rect(20, 0, 20, 60) //arm

    //Right Leg
    push();
      translate(45,70);
      rotate(-legAngle)
      fill(70, 130, 180)
        rect(-10, 0, 20, 60, 20) //right leg
      fill(0)
        rect(-10, 50, 20, 10) //rightshoe
    pop();

    fill(222, 184, 135)
      ellipse(30, -20, 60) //head

    fill(200) //beanie
      rect(-5, -30, 70, 10)
      arc(30, -25, 60, 60, PI, 2*PI)

  pop();
}
//------------------------------------------------------------------function flash--------------------------------------------------------------//
function flash()
{
        frameRate(20)

        console.log(flash_count)

                if(flash_count%2==0)
                {
                        fill(255)
                        rect(0,0,width,height)
                }
}
//-----------------------------------------------------------------------function drawupsidedowneffect----------------------------------------------//
function drawupsidedowneffect()
{
        push();
                translate(0, 0)
                        fill(0, 139, 139, 160) //blue tint
                                rect(0, 0, 500, 500)

                stroke(0, 100)
                strokeWeight(200)
                        noFill() //circle shadow
                                ellipse(250, 250, 1200, 680)

                        noStroke()
                        strokeWeight(1)
                        fill(0, 10) //dark tint
                                rect(0, 0, 500, 500)

        pop();
	
        			image(vinePic,-50,-50)
}
//------------------------------------------------------------function drawSpiral----------------------------------------//
function drawSpiral(cx, cy, radi)
{
        var sx, sy, sradi;
                stroke(0);
                fill(0);
                        sradi = 0;
                                for (var i=0; i < stopHere; i++)
                                        {
                                                 sx = cx +sradi*sin(radians(i));
                                                 sy = cy + sradi*cos(radians(i));
                                                 ellipse(sx, sy, sradi/5, sradi/5);
                                                 sradi = sradi + radi/720;
                                        }

                                if (stopHere < 720)
                                        {
                                                stopHere = stopHere + 1;
                                        }
}
//--------------------------------------------------------------function Particle-----------------------------------------------//
function Particle(x , y) 
{

   this.velX = random(-1,1);
   this.velY = random(-1.7, 3);

   this.locX = x;
   this.locY = y;
   this.r = 10;
   this.l = 5;
   this.life = 100;

   this.updateP = function()
   {

      this.locX += this.velX;
      this.locY += this.velY;

      this.life -= 1.0;
      if(this.locY>height)
      {
        this.locY = y
      }
   };

   // function to draw a particle
   this.renderP = function()
   {
      noStroke();
      push();
         fill(255,255,255,150);
         translate(this.locX, this.locY);
         ellipse(0, 0, this.r, this.l);
      pop();
   };
}
//---------------------------------------------------------------function PSys--------------------------------------------------//
function PSys(sX, sY, num)
{
   this.particles = [];
   for (var i=0; i < num; i++)
   {
      this.particles.push(new Particle(sX, sY));
   }

   this.run = function()
   {
      for (var i=0; i < this.particles.length; i++)
      {
         this.particles[i].updateP();
         this.particles[i].renderP();
      }
   }
}
//----------------------------------------------------------------------------------function drawCharacterSide2-------------------------------------//
function drawCharacterSide2(xCharacterSide, yCharacterSide)
{
  push();
    translate(xCharacterSide, yCharacterSide);
    scale(characterSideSC);
      fill(222, 184, 135)

    //Left Leg
    push();
      translate(15, 70)
      rotate(legAngle)
      fill(70, 130, 180)
        rect(-10, 0, 20, 60, 20) //left leg
      fill(0)
        rect(-10, 50, 20, 10)//left shoe
    pop();

    fill(205, 92, 92)
      rect(0, 0, 60, 80) //body
      rect(20, 0, 20, 60) //arm

    //Right Leg
    push();
      translate(45,70);
      rotate(-legAngle)
      fill(70, 130, 180)
        rect(-10, 0, 20, 60, 20) //right leg
      fill(0)
        rect(-10, 50, 20, 10) //rightshoe
    pop();

    fill(222, 184, 135)
      ellipse(30, -20, 60) //head

    fill(200) //beanie
      rect(-5, -30, 70, 10)
      arc(30, -25, 60, 60, PI, 2*PI)

  pop();
}
//-----------------------------------------------------------------function mouseClicked----------------------------------------------------------//
function mouseClicked()
{
  for(i=0; i<monsterMax; i++){
    if((mouseX-cxMonster[i])*(mouseX-cxMonster[i])+(mouseY-cyMonster[i])*(mouseY-cyMonster[i])-25*25<=0){
      monsterVelocityX[i]=0
      monsterVelocityY[i]=0
      for (var j = 0; j < demodog[i].pixels.length; j+=4) {
        demodog[i].pixels[j] = 255;
      }
      demodog[i].updatePixels();
    }
  }
  moveBat=true
  batDown=true
  batROT=0
  if(mouseX>150&&mouseX<350&&mouseY>250&&mouseY<400)
  {
        arcadeStart = true;
  }
}
//--------------------------------------------------------------function drawupsideBackground-------------------------------//
function drawupsideBackground()
{
  push();
    translate(upsideX, upsideY)
      fill(200) //background
        rect(0, 0, 500, 500)

      fill(0)
      stroke(255)
        rect(0, 200, 100, 300)
        rect(100, 180, 100, 300)
        rect(200, 220, 100, 300)
        rect(300, 225, 100, 300)
        rect(400, 150, 100, 300)

      noStroke()
      fill(128, 50)
      push();
        translate(0, 25)
        ellipse(100, 100, 200, 50)
        ellipse(250, 80, 200, 50)
        ellipse(200, 60, 200, 50)
        ellipse(120, 110, 200, 50)
        ellipse(300, 110, 200, 50)
        ellipse(350, 40, 200, 50)
        ellipse(410, 70, 200, 50)
        ellipse(480, 10, 200, 50)
        ellipse(40, 50, 200, 50)
      pop();

      fill(160, 82, 45) //dirt
      stroke(0)
        ellipse(250, 500, 1000, 200)

  pop();
}
//----------------------------------------------------------------function drawSchool---------------------------------------------//
function drawSchool()
{
        var lockerX = 0
        var lockerY = 100


        push();
                translate(0, 0)
                        fill(180)
                                rect(0, 0, 500, 500);

        for (l = 0; l < 21; l++)
        {
                        fill(255, 228, 181) //locker body
                                rect(lockerX, lockerY, 50, 150)

                        fill(128) //locker wheel
                                ellipse(lockerX + 10, lockerY + 50, 10)
                        fill(0)
                                ellipse(lockerX + 10, lockerY + 50, 5)

                        fill(128) //gray handle
                                rect(lockerX + 8, lockerY + 60, 4, 20)

                        fill(235, 208, 161) //locker grates
                                noStroke()
                                rect(lockerX + 10, lockerY + 10, 30, 5)
                                rect(lockerX + 10, lockerY + 20, 30, 5)
                                rect(lockerX + 10, lockerY + 30, 30, 5)
                                rect(lockerX + 10, lockerY + 110, 30, 5)
                                rect(lockerX + 10, lockerY + 120, 30, 5)
                                rect(lockerX + 10, lockerY + 130, 30, 5)

                                stroke(1)

                lockerX += 50

                if (lockerX > 500)
                {
                        lockerX = 0
                        lockerY += 150
                }

        }

        pop();
}
//---------------------------------------------------------------------function drawDustin---------------------------------------------//
function drawDustin(x, y, sc)
{
        push();
                translate(x, y);
                scale(sc);

                fill(153, 50, 204);     //body
                        rect(-20, 0, 40, 80)

                fill(255, 218, 185);            //hand
                        ellipse(20, 50, 10)

                fill(255, 218, 185); //head
                        ellipse(0, 0, 60);

                fill(139, 69, 19);      //hair
                        ellipse(-25, 0, 10)
                        ellipse(-20, 0, 10)
                        ellipse(-15, 0, 10)
                        ellipse(-10, 0, 10)
                        ellipse(-5, 0, 10)
                        ellipse(0, 0, 10)
                        ellipse(5, 0, 10)
                        ellipse(10, 0, 10)
                        ellipse(15, 0, 10)
                        ellipse(20, 0, 10)
                        ellipse(25, 0, 10)

                fill(0, 0, 128);                //hat
                        arc(0, 0, 60, 60, PI, 3*PI/2)
                fill(255);
                        arc(0, 0, 60, 60, 3*PI/2, 2*PI)
                fill(220, 20, 60);
                        arc(30, 0, 40, 5, 3*PI/2, 2*PI)

        pop();
}
//---------------------------------------------------------------------function drawLucas---------------------------------------------------//
function drawLucas(x, y, sc)
{
        push();
                translate(x, y);
                scale(sc);

                fill(220, 20, 60);      //body
                        rect(-20, 0, 40, 80)

                fill(160, 82, 45);              //hand
                        ellipse(-20, 50, 10)

                fill(160, 82, 45); //head
                        ellipse(0, 0, 60);

                fill(0);        //hair
                        arc(0, 0, 60, 70, 5*PI/6+PI/2-PI/6, 11*PI/6+PI/2)

        pop();
}
//-------------------------------------------------------------------------function drawWill--------------------------------------------------------//
function drawWill(x, y, sc)
{
        push();
                translate(x, y);
                scale(sc);

                fill(218, 165, 32);     //body
                        rect(-20, 0, 40, 80)

                fill(255, 218, 185);            //hand
                        ellipse(-20, 50, 10)

                fill(255, 218, 185); //head
                        ellipse(0, 0, 60);

                fill(139, 69, 19);      //hair
                        arc(0, 0, 60, 60, 5*PI/6+PI/2-PI/6, 11*PI/6+PI/2)


        pop();
}
//-----------------------------------------------------------------------function drawMike----------------------------------------------//
function drawMike(x, y, sc)
{
        push();
                translate(x, y);
                scale(sc);

                fill(128);      //body
                        rect(-20, 0, 40, 80)
                fill(0, 0, 139) //stripes
                        rect(-20, 30, 40, 10)
                        rect(-20, 60, 40, 10)
                fill(218, 165, 32)
                        rect(-20, 40, 40, 5)
                        rect(-20, 70, 40, 5)

                fill(255, 218, 185);            //hand
                        ellipse(-20, 50, 10)

                fill(255, 218, 185); //head
                        ellipse(0, 0, 60);

                fill(0);        //hair
                arc(0, 0, 62, 62, 5*PI/6+PI/2-PI/6, 11*PI/6+PI/2)

        pop();
}
//------------------------------------------------------------------function drawEleven------------------------------------------------------//
function drawEleven(x, y, sc)
{
        push();
                translate(x, y);
                scale(sc);

                fill(255, 182, 193);    //body
                        rect(-20, 0, 40, 80)

                fill(255, 218, 185);            //hand
                        ellipse(20, 50, 10)

                fill(255, 218, 185); //head
                        ellipse(0, 0, 60);

                fill(255, 250, 224);    //hair
                        arc(0, 0, 60, 60, 3*PI/6, 11*PI/6)

        pop();
}
//----------------------------------------------------------------------function outsideSchool-----------------------------------//
function outsideSchool(x, y, sc)
{

        push();
                translate(x,y);
                scale(sc);
                        noStroke();

                                fill(0) //building
                                        rect(49, 30, 50, 100)
                                        rect(100, 20, 50, 100)
                                        rect(151, 40, 50, 100)
                                        rect(202, 20, 50, 100)
                                        rect(253, 10, 50, 100)


                                fill(120)
                                        ellipse(250, 150, 500, 100)

                                fill(255, 228, 181); //school walls
                                        rect(0,0,80,180);

                                fill(125,30,0); //roof
                                        quad(0,0,0,-50,80,-50,120,0)

                                fill(100,50,5); //stairs
                                        rect(80,135,15,15);
                                        rect(80,150,30,15);
                                        rect(80,165,45,15);
                                        rect(3,-60,60,20);
                                        rect(20,-90,15,30);
                                        rect(25,-110,4,20);

                                fill(95,25,20); //door
                                        rect(78,45,4,90);

                                fill(240,210,40); //bell
                                        beginShape();
                                        curveVertex(35,-85);
                                        curveVertex(40,-78);
                                        curveVertex(45,-68);
                                        curveVertex(35,-70);
                                        curveVertex(35,-75);
                                        endShape(CLOSE);

                                stroke(0);
                                fill(255); //window
                                        beginShape();
                                        curveVertex(40,30);
                                        curveVertex(55,45);
                                        curveVertex(60,85);
                                        curveVertex(40,90);
                                        curveVertex(20,85);
                                        curveVertex(25,45);
                                        curveVertex(40,30);
                                        endShape(CLOSE);

                                fill(0); //
                                        rect(38,30,2,60);
                                        rect(20,68,40,2);
                                        ellipse(36,-70,3)
        pop();
}
//------------------------------------------function keyTypde----------------------------------------------------//
function keyTyped() {
 if (key === 'a') {

        outsideOk = true

  }
 if (key === 'h') {

        elevenPop = true

  }

}
//-----------------------------------------------function distance---------------------------------------------------//
function distance(x0, y0, x1, y1)
{
  return sqrt(pow(x0-x1, 2) + pow(y0-y1, 2));
}
//-----------------------------------------------function bossBG------------------------------------------------------//
function bossBG()
{
        noStroke();
        gradientRect=0
  for(y=0; y<height; y+=8)
  {
    for(x=0; x<width; x+=20)
    {
      d = distance(x, y, x, 125);
      fill(200+randRed[gradientRect]- d, 0, 0);
      rect(x+randX[gradientRect], y+randY[gradientRect], 40, 10);
      gradientRect++
    }
  }
}
//-------------------------------------------------function mouseDragged--------------------------------------------------//
function mouseDragged()
{
  for(var i=0;i<itemNo;i++)
  {
    if((mouseX-itemX[i])*(mouseX-itemX[i])+(mouseY-itemY[i])*(mouseY-itemY[i])-(itemR[i]*itemR[i])<=0)
    {
      drag_item= true;
      itemX[i] = mouseX;
      itemY[i] = mouseY;
      return false;
    }
  }
}
//-----------------------------------------------------------function drawKraken----------------------------------------------------//
function drawKraken(x, y, krakenSC)
{
  push();
    translate(x, y);
    scale(krakenSC)
    fill(0); //orb
    ellipse(0, 0, 80);
    if(redKraken){
      fill(255, 0, 0);
      redKraken=!redKraken
    }
    else
      fill(100)
    beginShape(); //top right limb
      curveVertex(0, -200)
      curveVertex(0, -200)
      curveVertex(150, -300)
      curveVertex(250, -300)
      curveVertex(170, -280)
      curveVertex(20, -170)
      curveVertex(0, -200)
      curveVertex(0, -200)
    endShape();

    beginShape(); //top right extensions
      curveVertex(150, -300)
      curveVertex(150, -300)
      curveVertex(160, -350)
      curveVertex(170, -350)
      curveVertex(180, -300)
      curveVertex(180, -300)
    endShape();

    beginShape(); //bottom right limb
      curveVertex(0, -140)
      curveVertex(0, -140)
      curveVertex(200, -90)
      curveVertex(250, -70)
      curveVertex(210, -100)
      curveVertex(20, -170)
      curveVertex(0, -140)
      curveVertex(0, -140)
    endShape();

    beginShape(); //bottom right extensions
      curveVertex(150, -120)
      curveVertex(150, -120)
      curveVertex(160, -50)
      curveVertex(165, -30)
      curveVertex(170, -105)
      curveVertex(160, -115)
      curveVertex(160, -115)
    endShape();

    beginShape(); //bottom left limb
      curveVertex(0, -140)
      curveVertex(0, -140)
      curveVertex(-200, -90)
      curveVertex(-250, -70)
      curveVertex(-210, -100)
      curveVertex(-20, -170)
      curveVertex(0, -140)
      curveVertex(0, -140)
    endShape();

    beginShape(); //bottom left extensions
      curveVertex(-150, -120)
      curveVertex(-150, -120)
      curveVertex(-160, -50)
      curveVertex(-165, -30)
      curveVertex(-170, -105)
      curveVertex(-160, -115)
      curveVertex(-160, -115)
    endShape();

    beginShape(); //top left limb
      curveVertex(0, -200)
      curveVertex(0, -200)
      curveVertex(-150, -300)
      curveVertex(-250, -300)
      curveVertex(-170, -280)
      curveVertex(-20, -170)
      curveVertex(0, -200)
      curveVertex(0, -200)
    endShape();

    beginShape(); //top left extensions
      curveVertex(-150, -300)
      curveVertex(-150, -300)
      curveVertex(-160, -350)
      curveVertex(-170, -350)
      curveVertex(-180, -300)
      curveVertex(-180, -300)
    endShape();

    beginShape(); //kraken head
      curveVertex(0, -300);
      curveVertex(0, -300);
      curveVertex(-30, -150);
      curveVertex(0, -120);
      curveVertex(30, -150);
      curveVertex(0, -300);
      curveVertex(0, -300);
    endShape();
  pop();
}
//------------------------------------------------------------------function drawBat------------------------------------------------//
function drawBat(batX, batY, batSC)
{
  push();
    translate(batX, batY)
    scale(batSC)
    if(moveBat){
      if(batDown){
        batROT+=PI/14
      }
      else
        batROT-=PI/14
      if(batROT>=PI/2)
        batDown=false
      if(batROT<0){
        moveBat=false
        batROT=0
      }
      rotate(batROT)
    }
      fill(222, 184, 135) //bat
        beginShape()
          curveVertex(-10, 0)
          curveVertex(-10, 0)
          curveVertex(-15, -100)
          curveVertex(-0, -120)
          curveVertex(15, -100)
          curveVertex(10, 0)
          curveVertex(10, 0)
        endShape()

      fill(200) //grip
        rect(-10.5, 0, 20, 30)

      fill(222, 184, 135) //base
        ellipse(0, 30, 30, 10)

      fill(120) //nails
        ellipse(-15, -90, 10)
        ellipse(12, -70, 10)
        ellipse(0, -115, 10)
        ellipse(-5, -60, 10)
        ellipse(10, -95, 10)

  pop();

}
//----------------------------------------------------------function drawOutside-----------------------------------------------//
function drawOutside()
{
        push()
                translate(0, 0)
                        fill(244, 164, 96) //walls
                                rect(0, 0, 500, 500)

                        fill(135, 206, 250) //doors
                                rect(150, 250, 100, 150)
                                rect(250, 250, 100, 150)

                        fill(255, 215, 0) //knobs
                                ellipse(230, 350, 20)
                                ellipse(270, 350, 20)

                        fill(70) //floor
                                rect(0, 400, 500, 100)

                        fill(200) //street
                                rect(0, 450, 500, 100)

                        fill(90)
                                rect(0, 450, 500, 10)

                        fill(135, 206, 235) //sky
                                rect(0, 0, 500, 100)

                        fill(255) //sign
                                rect(50, 125, 400, 100, 20)

                        fill(255, 128, 0)
                                textSize(82)
                                textFont("helvetica")
                                text("ARCADE", 95, 200)

                        fill(255, 255, 0)
                                textSize(80)
                                textFont("helvetica")
                                text("ARCADE", 100, 200)

        pop();
}