/* 
 * File:   main.cpp
 * Author: Fredi Brizuela
 * Created on June 4, 2020 4:15 PM
 * Purpose: Revised Blackjack, overhaul Project 1 to meet requirements, in order to meet requirements, most of my code will move and be revised 
 */

//System Libraries Here
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
//Adding more in order to allow new code like a random generator or vectors
using namespace std;

//types for the card suits Spades, Clubs, Diamonds, Hearts
enum card_suit{
	S, C, D, H
};

//Structures for a score setter for players
struct acout{ //account structure. This will store all the player information
	string name; //The name on the account
	string user; //the username of the player
	string paswrd; //password for the players account

	int money; //in order to add a reward or to keep progress in wins and loses

	
	int wins; //number of wins a player has endured
	int total_played; //total of wins or losses a player has endured
	double win_percentage; //a ratio of how many games the player has played
};
struct card{ //a storage for all of the cards being used in the game
	int value; //value of the cards
	card_suit suit; //Adding suits will make the game feel more simular to those in casinos
	bool up; //for the dealer since one card is face down, something I did not include in my first project due to lack of time
};
struct player{ //basis of each player in the game including the dealer
	acout info; //player's account info in order to address them by their name
	vector<card> hand; //the players hand
	int bet; //how much the user is betting
};
//Prototype functions used throughout the project
card deal();//Dealer
card hitMe();//Hit or receive card
int score(vector<card>);//Keep score of dealer and player
bool hasAce(vector<card>);//In the case of an ace being present due to it having two values 1 or 11
char pntSuit(card);//Print what suit the card is
void dblDown(player, player&);//Double down on bet if player is certain of victory
void payout(player, player&);//How much money the player has earned
void split(player&, player&);//In the rare case that a player draws two cards of simular value, they can play both as if it were two players
void bet(player&);//The Bet made by the player, in this case 5 dollars
char pntSuit(card);//Print what suit the card is
acout load();//Loads account info if playing multiple times
void save(player);//Saves player info for next games
acout create();//Allows player to create an account with password and username
void play(vector<player>&);//Allows the game to be played
void clear(vector<card>&);//Clears the cards used to reset for a new game
void deal_ply(player&);//Dealers play if ace or split are involved
int winner(player, player&);//Displays what happened if winner is chosen
void menu();//Menu for the game allowing for the player to view rules or make an account and play
void rules();//Displays a length tutorial for Blackjack

//Program Execution Begins Here
int main(){
	unsigned int time_seed = time(0); //so we can get a random number
	srand(time_seed); //seeding the random number
	player user; //creates a player called user
	player dealer; //creates the dealer
	vector<player> players; //creates a vector of players
	char input; //for the Input
	dealer.info.user = "Dealer"; //sets up the dealer info
	players.push_back(dealer); //puts the dealer in the vector
        
	int menu; //To take in the input of the user
        
	do{ 
		menu(); //displays the menu for the player
		cin>>menu; //will take the users input

		if(menu == 1){ //if the user wants to read the rules
			rules(); //displays the rules
		}
		else if(menu == 2){ //if the user wants to play
				cout << "How many players(1)" << endl; //asks how many players, only one would suffice
				int numPlyr; //so we can take in the player
				cin >> numPlyr; //takes in the input
				for(int i = 0;i< numPlyr;i++){ //cycles through the players
					cout << "Are you a new player(N) or existing player(E)?" << endl; //asks if they are a new or existing player
					cin >> input; //takes in their input

					if(input == 'N' || input == 'n'){ //if they are new player...
						user.info = create(); //creates new account info
						save(user); //saves the players info
						players.push_back(user); //pushes info of player back into the vector
					}
					else if(input == 'E' || input == 'e'){ //if they have already played a game of blackjack
						user.info = load(); //loads their account info
						players.push_back(user); //pushes the info back in the vector
					}
			
				}
		}
		else{ //if the user can't follow directions
			cout << "Please enter (1)" << endl; //reiterates their only option in number of players
			menu = 0; //resets menu in case of error
			cin.ignore(); //ignores the buffer
		}
	}while(menu != 2); //while they don't choose play we will repeat this until we finish the never ending story
	
	bool cond = false; //asking the user if they would wish to continue
	int size = players.size();//Verifies number of players which should be one
	do{
		for(int i =1;i<players.size();i++){//Checks for players money
			if(players[i].info.money < 5){ //checks to see if they player has any money in existing account
				cout << players[i].info.user << " has no money! Would you like to add some money, yes(Y) or (N)?" << endl; //if user is out of money, they have the chance to give them self money.
				cin >> input; //takes in input
				if(input == 'Y' || input == 'y'){ //if they say yes
					cout << "How much would you like to add?" << endl; //how much money they would like to give themselves
					cin >> players[i].info.money; //takes it in
				}
				else{ //if not we save their info and leave
					save(players[i]); //saves info
					players.erase(players.begin() + (i)); //erases player from vector and game in order to create a new player
					i--;

				}
			}

			if(cond && players.size() > 1){//if they have played at least once
				cout << players[i].info.user << " Would you like to continue playing Blackjack(Y) or not(N)" << endl;
				cin >> input; //takes in input for quiting or continuing
				if(input == 'n' || input == 'N'){ //player want to quit
					save(players[i]); //saves info for next game
					players.erase(players.begin() + (i)); //erases player from vector
					i--;
				}
			}
		}
		play(players); //Begins the game
		cond = true; //They finished one hand
	}while(players.size() > 1);
	//!(input == 'n' || input == 'N')

	return 0;//Ends Program
}