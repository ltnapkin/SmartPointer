/*
Travieon Fields
Assignment Smart Pointer


 I made an escape room type game to play where you need to find the key to progress and exit

*/


//Liberies
#include <iostream>
#include <string>
#include <list>
#include<Windows.h>
#include <memory>
#include <time.h>
using namespace std;


//This is all my functions in order
// the first one makes the key in a random room Except for exit rooms which are the front door and back door 
void keyplacementRand();

// this makes sure your movement is a valid one
void validAction(int playersMove);
//this uses code to move the player to the room selected
void movingPlayer(int playersMove);
//this is a fail safe to stop you from going to rooms that don't exist
void hysteriaReset();
//this checks to see whether or not you won or lost
void winorLose(int playersMove);

// this is use to check whether the monster knows where you are
int monsterSense;
// this is for holding the key
int playersPocket = 10;
// this is the key varible that can point to the player or the monster
int key;
// this is for the players movement
int playerMovingTowards;

// this is the players health
int playersLives = 3;
//this is where the player currently is
int playersCurretMove;
//checks to see if you keep putting in not valid areas.
int hysteriaCounter;
//Checks to see if you have key or not
bool hasKey = false;
// checks if game is over
bool isGameOver = false;
//see if player is close to monster
bool playerIsClose = false;
// makes it so i can change the text color
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);






int main()
{

	
	//allows random based on time
	srand(time(0));
	
	//calls the fuctions to randomly change where the key is 
	keyplacementRand();

	cout << "There is a monster in your house you must find the Key to escape!" << endl;
	cout << "The Monster is looking for you be careful!" << endl;
	cout << "Where should you look first?" << endl;
	//this code basically sees if the game is over
	while (!isGameOver)
	{
		//resets the color back to white
		SetConsoleTextAttribute(h, 7);
		cout << "0)Bedroom,1)Kitchen,2)Bathroom,3)LivingRoom,4)Basement,5)FrontDoor,6)Attic,7)Backdoor" << endl;
		//stores players choice
		cin >> playerMovingTowards;
		validAction(playerMovingTowards);
	}

	winorLose(playerMovingTowards);

}

//this places the key at a random area That is not 5 or 7 which are win/lose states if you collect the key
void keyplacementRand()
{

	shared_ptr<int> KeyToDoor = make_shared<int>(rand() % 7);
	key = *KeyToDoor;
	
	//keyplacement = rand() % 7;
	if (key == 5 || key == 7)
	{
		keyplacementRand();
	}

}

//this checks to see if the player  made a valid move
void validAction(int playersMove)
{


	// checks to make sure the player didn't just re select where they are.
	if (playersCurretMove != playersMove)
	{
		switch (playersMove)
		{
		case 0:
			cout << "You are in your Bedroom" << endl;

			//Resets hysteria lose state
			hysteriaReset();
			break;
		case 1:
			cout << "You are in your Kitchen" << endl;

			hysteriaReset();
			break;
		case 2:
			cout << "You are in your Bathroom" << endl;

			hysteriaReset();
			break;
		case 3:
			cout << "You are in your LivingRoom" << endl;

			hysteriaReset();
			break;
		case 4:
			cout << "You are in your Basement" << endl;

			hysteriaReset();
			break;
		case 5:
			cout << "You are in your FrontDoor" << endl;


			hysteriaReset();
			break;
		case 6:
			cout << "You are in your Attic" << endl;

			hysteriaReset();
			break;
		case 7:


			cout << "You are in your Backdoor" << endl;

			hysteriaReset();
			break;


		default:
			cout << "The panic that the monster might be near has made you hysterical,take a deep breath" << endl;
			//makes it so if you pick the invalid chice you begin a lose state
			hysteriaCounter++;
			break;
		}
		//calls the move player function
		movingPlayer(playersMove);
		//checks to see if player has key and is standing in a win/lose state room
		if (key == playersPocket && playersMove == 5 || key == playersPocket && playersMove == 7)
		{
			winorLose(playersMove);
		}
	}
	else
	{
		//picking the same place you were in make you collect yourself
		cout << "You took time to collect yourself. . ." << endl;
		movingPlayer(playersMove);
		//makes it so if you stay in the place you a re the monster can still attack you
		if (playersLives <= 0)
		{
			cout << "However The monster Found you And showed no mercy!" << endl;
		}
	}
	//sets current move to your recent move
	playersCurretMove = playersMove;

	//Causes a game over state if you make invaild inputs
	if (hysteriaCounter >= 3)
	{
		cout << "You had a panic attack.And everything went black." << endl;
		isGameOver = true;

	}

}


//This checks to see if the player move to the same place as the monster or the key or win/lose state
void movingPlayer(int playersMove)
{

	//Makes two smart pointers That make your character and Monster Move
	unique_ptr<int> playersMov = make_unique<int>(playersMove);
	unique_ptr<int> monsterMovement = make_unique<int>(rand() % 8);
	


		//if the monster is in the same spot as the player by checking numbers
		if (*monsterMovement == *playersMov)
		{
			//if you land on the monsters place 3 times you lose
			playersLives--;
			if (playersLives <= 0)
			{
				isGameOver = true;
				
				return;
			}
			//else it just attacks you
			else if (playersLives > 0)
			{
				SetConsoleTextAttribute(h, 4);
				cout << "the monster Attacks you but you were able to get away." << endl;
			}
			//Takes the key from you if you have it and puts it in a random area.
			if (key == playersPocket)
			{
				//Change the Keys Spawn
				keyplacementRand();
				hasKey = false;
				cout << "the monster Attacks you And Stole the Key as it ran off!" << endl;
			}
		}
	//if you are near the monster it will go to your space
	else if (monsterSense +1 == *playersMov || monsterSense - 1 == *playersMov)
	{
		monsterSense = *playersMov;
		playerIsClose = true;
		unique_ptr<int> monsterMovement = make_unique<int>(*playersMov);
		SetConsoleTextAttribute(h, 6);
		cout << "You can hear footsteps from a room near you." << endl;
	}
	else {
		playerIsClose = false;
	}


	//if the player finds the key it gives them it a sets up the win/lose state
	if (key == playersMove && hasKey == false)
	{
		//makes the key pointer the players pocket
		key = playersPocket;
		hasKey = true;
		SetConsoleTextAttribute(h, 3);
		cout << "You found The Key! You may exit through the back or front door, But be careful that the monsters not near by." << endl;
	}

}

//resets the hysteria counter
void hysteriaReset()
{
	hysteriaCounter = 0;
}

//win or lose state checks certain varibles 
void winorLose(int playersMove) {

	//This basically states If you have the key and the monster is not near by you win As well as checking if the key is pointing at the player.
	if (key == playersPocket && playersMove != monsterSense && !playerIsClose)
	{
		SetConsoleTextAttribute(h, 2);
		cout << "you succefully fleed your house and contacted the police about the monster." << endl;
		isGameOver = true;
	}
	//this says if you have the key and the monster is near by or is on the space you lose.
	else if (key == playersPocket && playersMove == monsterSense || key == playersPocket && playerIsClose)
	{
		SetConsoleTextAttribute(h, 4);
		cout << "you run for the door  the monsters right behind you Jumps to Catch you. you were never heard from again" << endl;
	}
	else if (playersLives <= 0)
	{
		SetConsoleTextAttribute(h, 4);
		cout << "you run for the door It was Locked! the monsters right behind you Jumps to Catch you. you were never heard from again" << endl;
	}
}