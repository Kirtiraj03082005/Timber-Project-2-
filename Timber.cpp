#include<iostream>
#include<unistd.h>
#include<sstream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace std;
using namespace sf;
const int NUM_BRANCHES=6;
enum class side{LEFT,RIGHT,NONE};
side branchPositions[NUM_BRANCHES];
void updateBranches(int seed){
	for(int i=NUM_BRANCHES-1;i>0;i--){
		branchPositions[i]=branchPositions[i-1];
	}
	srand((int)time(0)+seed);
	int r=rand()%5;
	switch(r){
		case 0:
			branchPositions[0]=side::LEFT;
			break;
		case 1:
			branchPositions[0]=side::RIGHT;
			break;
		default:
			branchPositions[0]=side::NONE;
			break;
	}
}

int main(){

	VideoMode vm(1920,1080);
	RenderWindow window(vm,"My Demo Game");
	Texture textureBackground,textureCloud,textureBee,textureTree,textureBranch,texturePlayer,textureRIP,textureAxe;
	
	textureBackground.loadFromFile("background.png");
	textureTree.loadFromFile("tree.png");
	textureBee.loadFromFile("bee.png");
	textureCloud.loadFromFile("cloud.png");
	textureBranch.loadFromFile("branch.png");
	texturePlayer.loadFromFile("player.png");
	textureRIP.loadFromFile("rip.png");
	textureAxe.loadFromFile("axe.png");
	
	View view(FloatRect(0,0,1920,1080));
	window.setView(view);
	
    Sprite spriteBackground,spriteCloud1,spriteCloud2,spriteCloud3,spriteBee,spriteTree,branches[NUM_BRANCHES],spritePlayer,spriteRIP,spriteAxe;
    
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0.0f,0.0f);
	
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810.0f,0.0f);
	
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0.0f,800.0f);
	bool beeActive=false;
	float beeSpeed=0.0f;
	
	
	bool cloud1Active=false;
	float cloud1Speed=0.0f;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud1.setPosition(0.0f,0.0f);
	
	bool cloud2Active=false;
	float cloud2Speed=0.0f;
	spriteCloud2.setTexture(textureCloud);
	spriteCloud2.setPosition(0.0f,250.0f);
	
	bool cloud3Active=false;
	float cloud3Speed=0.0f;
	spriteCloud3.setTexture(textureCloud);
	spriteCloud3.setPosition(0.0f,500.0f);
	
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580.0f,720.0f);
	side playerSide=side::LEFT;
	
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600.0f,860.0f);
	
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700.0f,830.0f);
	const float AXE_POSITION_LEFT=700;
	const float AXE_POSITION_RIGHT=1075;
	
	Texture textureLog;
	textureLog.loadFromFile("log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810.0f,720.0f);
	bool logActive=false;
	float logSpeedX=1000;
	float logSpeedY=-1500;
	
	for(int i=0;i<NUM_BRANCHES;i++){
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000,-2000);
		branches[i].setOrigin(220,20);
	}
	
	bool paused=true;
	
	Font font;
	font.loadFromFile("KOMIKAP_.ttf");
	
	Text messageText,scoreText;
	messageText.setFont(font);
	messageText.setString("PRESS ENTER TO START");
	messageText.setCharacterSize(75); 
	messageText.setFillColor(Color::White);
	
	scoreText.setFont(font);
	scoreText.setString("SCORE=0");
	scoreText.setCharacterSize(75); 
	scoreText.setFillColor(Color::Red);
	scoreText.setPosition(20.0f,20.0f);
	
	FloatRect textRect=messageText.getLocalBounds();
	messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
	messageText.setPosition(1920/2.0f,1080/2.0f);
	
	int score=0;
	
	RectangleShape timeBar;
	float timeBarStartWidth=400;
	float timeBarHeight=80;
	timeBar.setFillColor(Color::Red);
	timeBar.setSize(Vector2f(timeBarStartWidth,timeBarHeight));
	timeBar.setPosition(1920/2-timeBarStartWidth/2.0f,980);
	float timeRemaining=6.0f;
	float timeBarWidthPerSecond=timeBarStartWidth/timeRemaining;
	
	//Control player 
	bool acceptInput=false;
	
	SoundBuffer chopBuffer,outBuffer,deathBuffer;
	chopBuffer.loadFromFile("chop.wav");
	outBuffer.loadFromFile("baba.wav");
	deathBuffer.loadFromFile("faaa.wav");
	
	Sound chop,out,death;
	chop.setBuffer(chopBuffer);
	out.setBuffer(outBuffer);
	death.setBuffer(deathBuffer);

	Clock clock;
	/*updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);*/
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			//Event 1
			if(event.type==Event::Closed){
				window.close();
			}
			//Event 2
			if(event.type==Event::KeyReleased && !paused){
				acceptInput=true;
				spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
			}
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Escape)){
			window.close();
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Return)){
			paused=false;
			score=0;
			timeRemaining=6.0f;
			
			for(int i=1;i<NUM_BRANCHES;i++){
				branchPositions[i]=side::NONE;
			}
			spriteRIP.setPosition(675.0f,2000.0f);
			spritePlayer.setPosition(580.0f,720.0f);
			acceptInput=true;
		}
		if(acceptInput){
			//Handle Right Arrow Key
			if(Keyboard::isKeyPressed(Keyboard::Right)){
				score=score+1;
				timeRemaining=timeRemaining+2/score+0.15f;
				spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200,700);
				updateBranches(score);
				spriteLog.setPosition(810,720);
				logSpeedX=-5000;
				logActive=true;
				acceptInput=false;
				playerSide=side::RIGHT;
				chop.play();
			}
			//Handle Left Arrow Key
			if(Keyboard::isKeyPressed(Keyboard::Left)){
				score=score+1;
				timeRemaining=timeRemaining+2/score+0.15f;
				spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
				spritePlayer.setPosition(580,720);
				updateBranches(score);
				spriteLog.setPosition(810,720);
				logSpeedX=5000;
				logActive=true;
				acceptInput=false;
				playerSide=side::LEFT;
				chop.play();
			}
		}
		
		
		if(!paused){
			Time dt=clock.restart();
			
			timeRemaining=timeRemaining-dt.asSeconds();
			timeBar.setSize(Vector2f(timeRemaining*timeBarWidthPerSecond,timeBarHeight));
			
			if(timeRemaining<=0.f){
				paused=true;
				messageText.setString("Time's Up");
				textRect=messageText.getLocalBounds();
				messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
				messageText.setPosition(1920/2.0f,1080/2.0f);
				out.play();
			}
			
			if(!beeActive){
				srand((int)time(0));
				beeSpeed=rand()%200+200;
				srand((int)time(0)*10);
				float height=rand()%500+500;
				spriteBee.setPosition(2000, height);
				beeActive=true;
			}else{
				spriteBee.setPosition(spriteBee.getPosition().x-beeSpeed*dt.asSeconds(),spriteBee.getPosition().y);
				if(spriteBee.getPosition().x<-100){
					beeActive=false;
				}
			}
		
			if(!cloud1Active){
				srand((int)time(0)*10);
				cloud1Speed=rand()%200;
				srand((int)time(0)*10);
				float height=rand()%150-0;
				spriteCloud1.setPosition(-200, height);
				cloud1Active=true;
			}else{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x+cloud1Speed*dt.asSeconds(),spriteCloud1.getPosition().y);
				if(spriteCloud1.getPosition().x>1920){
					cloud1Active=false;
				}
			}
		
			if(!cloud2Active){
				srand((int)time(0)*20);
				cloud2Speed=rand()%200;
				srand((int)time(0)*20);
				float height=rand()%300-150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active=true;
			}else{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x+cloud2Speed*dt.asSeconds(),spriteCloud2.getPosition().y);
				if(spriteCloud2.getPosition().x>1920){
					cloud2Active=false;
				}
			}
		
			if(!cloud3Active){
				srand((int)time(0)*30);
				cloud3Speed=rand()%200;
				srand((int)time(0)*30);
				float height=rand()%450-150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active=true;
			}else{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x+cloud3Speed*dt.asSeconds(),spriteCloud3.getPosition().y);
				if(spriteCloud3.getPosition().x>1920){
					cloud3Active=false;
				}
			}
			//score+=1;
			stringstream ss;
			ss<<"SCORE="<<score;
			scoreText.setString(ss.str());
			
			for(int i=0;i<NUM_BRANCHES;i++){
				int height=i*150;
				if(branchPositions[i]==side::LEFT){
					branches[i].setPosition(610,height);
					branches[i].setRotation(180);
				}else if(branchPositions[i]==side::RIGHT){
					branches[i].setPosition(1330,height);
					branches[i].setRotation(0);
				}else{
					branches[i].setPosition(3000,height);
				}	
			}
			if(logActive){
				float xpos=spriteLog.getPosition().x+logSpeedX*dt.asSeconds();
				float ypos=spriteLog.getPosition().y+logSpeedY*dt.asSeconds();
				spriteLog.setPosition(xpos,ypos);
				if(spriteLog.getPosition().x<-100 ||  spriteLog.getPosition().x>2000){
					logActive=false;
					spriteLog.setPosition(810,720);
				}
			}
			//Handle Death
			if(branchPositions[5]==playerSide){
				paused=true;
				acceptInput=false;
				spriteRIP.setPosition(525,760);
				spritePlayer.setPosition(2000,660);
				messageText.setString("SQUISHED!!!");
				textRect=messageText.getLocalBounds();
				messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
				messageText.setPosition(1920/2.0f,1080/2.0f);
				death.play();
			}
				
		}
		
		// END IF !PAUSED	
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spriteTree);
		for(int i=0;i<NUM_BRANCHES;i++){
			window.draw(branches[i]);
		}
		window.draw(spritePlayer);
		window.draw(spriteRIP);
		window.draw(spriteAxe);
		window.draw(spriteLog);	
		window.draw(spriteBee);
		window.draw(timeBar);
		if(paused){
			window.draw(messageText);
		
		}
		
		window.draw(scoreText);
		window.display();
	}
	return 0;
}
	
