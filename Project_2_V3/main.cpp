/* 
 * File:   main.cpp
 * Author: Fredi Brizuela
 * Created on June 5, 2020 5:48 PM
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
void pntCrds(vector<card>);//Prints Cards
void dblDown(player, player&);//Double down on bet if player is certain of victory
void payout(player, player&);//How much money the player has earned
void split(player&, player&);//In the rare case that a player draws two cards of simular value, they can play both as if it were two players
void bet(player&);//The Bet made by the player, in this case 5 dollars
acout load();//Loads account info if playing multiple times
void save(player);//Saves player info for next games
acout create();//Allows player to create an account with password and username
void play(vector<player>&);//Allows the game to be played
void clear(vector<card>&);//Clears the cards used to reset for a new game
void deal_ply(player&);//Dealers play if ace or split are involved
int winner(player, player&);//Displays what happened if winner is chosen
void disMenu();//Menu for the game allowing for the player to view rules or make an account and play
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
		disMenu(); //displays the menu for the player
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
				cin >> input; //takes in input for the player quit or continue
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
//Displaying the rules
void rules(){
	cout<< "The rules of blackjack are fairly simple and your only opponent in the game is the dealer. Each player is dealt a two cards face up while the dealer only has one card face up. The goal is to have your two cards added total to be higher than the dealer’s two cards and under 21. If you go over 21 you “bust”, or lose, or if the dealers two cards added together are higher than yours you also lose."<<endl;
        cout<< "If your two card total is equal to the dealers then it is a “stand-off” or a tie. Cards values are usually face value, 2 is 2 and 10 is 10, while all face cards, jack/queen/king, are also 10. The only exception to this rule are Aces, they can take on the value of 1 or 11. To get your two cards total you simply add your two cards together. If you have any combination of an Ace or any card that is 10 points then you have what is called blackjack, 21 in total. Getting blackjack means you get paid more if you win."<<endl;
        cout<< "With all of that being said if you’re not satisfied with your two card total then you can take extra cards, called taking a hit, and for each hit you get you get one more card. The dealer does this as well but has a strict set of rules to follow on whether or not to take a hit. If a dealer’s hand is less than or equal to 16 or a “soft” 17, meaning they have an ace and a 6, they must take a hit. If it’s 17 or higher the dealer must stand.  So now that we know the dealer rules there are a few options for users."<<endl;
        cout<< "As stated earlier you can take hits to increase your card total. You may also split your cards and double down. Splitting can be done when your first two cards are of equal value and can only be split from the original hand, split hands cannot be split, the bet has to be equal or greater than the original bet for each hand. For each time you split you will receive and additional card for that hand and then you play like regular blackjack.  Users may also double down which consists of a user placing another bet of equal or lesser value when their first two cards total is equal to 9, 10, or 11 without aces."<<endl;
        cout<< "After doubling down you will only get one additional card. Some of you may have realized that if the dealer gets a blackjack or 21 you pretty much always lose, unless you yourself have blackjack. There is a way around this and it’s called insurance. If the dealer is dealt an ace face up then the dealer will ask if you want to take out insurance, equal to half of your original bet, to insure your hand if the dealer has blackjack and only when he has blackjack and helps insure you don’t lose money if he does have blackjack, insurance pays 2 to 1 so your insurance bet will cover the loss of your hand if you bet half." << endl; //displays the rules of blackjack
}
//Displays Menu 
void disMenu(){
	cout << "Welcome to Blackjack!" << endl; //Displays welcome
	cout << "Please choose one of the two options below." << endl; //Asks user to select 1 or 2
	cout << "1) Rules of Blackjack" << endl; //Rules
	cout << "2) Play Blackjack" << endl; //Continue with the menu
}
//Lets the dealer play as one does in a casino
void deal_ply(player &dealer){
	if((score(dealer.hand) < 17) || (score(dealer.hand) == 17 && hasAce(dealer.hand))){ //dealer hits at less than 17 
		dealer.hand.push_back(hitMe()); //dealer gets a card because of house rule
	}	
}

// How the game will be played, the code will take care of all the play portions of the game. Taking hits, splitting, etc. Takes in the vector of players by reference so we can make changes directly to the players for future games

void play(vector<player> &players){
	char input; //for our input

	for(int i =1;i<players.size();i++){ //cycles through the player and dealer
		bet(players[i]); //players bet here
		cout << "Money: " << players[i].info.money << setw(10) << "Bet: " << players[1].bet << endl; //display their money and bet amount
	}
	
	//Gives the players (dealer included) one card at a time starting with the dealer
	for(int i = 0;i< (players.size()*2);i++){ 
		players[(i%players.size())].hand.push_back(deal());
		if((i%players.size()) == 0 && (i%2) == 0){ //the dealers first card
			players[(i%players.size())].hand[(i%2)].up = false; //is set to false since it's face down
		}
	}
	//Displays the score for the player
	for(int i=1;i<players.size();i++){
		cout << players[i].info.user << " has: " << score(players[i].hand) << setw(10) << endl;
	}

	//Function for the card being used by the player
	for(int i =0;i<players.size();i++){
		cout << players[i].info.user << " Cards:" << endl;
		pntCrds(players[i].hand);
	}
	
	
	//The below function  actually consists of the playing Blackjack
	
	bool cont = true; //to loop the game mechanics
	for(int i = 1;i<players.size();i++){ //cycles through the player since each player plays on their own card
		do{
			if(players[0].hand[1].value == 1 && cont){ //if the dealer has an ace and cont is true, only happens the first time if the dealer doesn't have blackjack
				if(score(players[0].hand) == 21){ //checks to see if the dealer has blackjack 
					players[0].hand[0].up = true; //if they do, set the first card to face up
			
					pntCrds(players[0].hand); //prints the dealers cards
					
					
					//The below function cycles through the players and pays them out since they lost
					
					for(int i =1;i<players.size();i++){
						payout(players[0],players[i]);
					}
					input = 'S'; //sets input to stay since they just lost
				}
				cont = false; //if the dealer didn't have blackjack this is now false
			}
			if(players[0].hand[1].value >= 10 && cont){ //if the dealer has a 10 or face card showing they have blackjack that's game
				if(score(players[0].hand) == 21){ //if they have blackjack
					players[0].hand[0].up = true; //puts the dealers first card face up

					pntCrds(players[0].hand); //prints the dealers card
					
					//The below function pays out the player since they just lost
					
					for(int i =1;i<players.size();i++){
						payout(players[0],players[i]);
					}
					input = 'S'; //input is now S since the players lost					
				}			
				cont = false; //if the dealer doesn't have 21 we don't care about this anymore
			}
			
				//As long as the players score is less than 21
			
			if(score(players[0].hand) <= 21){
					
					// If they have a pair of 5's they can split, double down, hit, or stay
					
					if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || players[i].hand[0].value == players[i].hand[1].value) && players[i].hand.size() == 2  && score(players[i].hand) == 10){ 
						cout << players[i].info.user << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to Double Down(D), split(L),take a hit(H), or stay(S), default is to take a stay?" << endl; //ask them
					}
					else if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || (players[i].hand[0].value == players[i].hand[1].value)) && players[i].hand.size() == 2){ //if they can split their cards
						cout << players[i].info.user << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to split(L) your cards, take a hit(H), or stay(S), default is to take a stay?" << endl; //ask them
					}
					else if(players[i].hand.size() == 2 && score(players[i].hand) >= 9 && score(players[i].hand) <= 11 && !(hasAce(players[i].hand))){ //can they double down
						cout << players[i].info.user << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to Double Down(D), take a hit(H), or stay(S), default is to take a stay?" << endl; //asks them
					}
					else{ //they can't do anything 
						cout << players[i].info.user << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Hit(H) or Stay(S), default is to take a stay?"; //asks them what they want to do
					}
					cin >> input; //takes in the input
					switch(input){ //what did they choose?
					case 'L': //they wanted to split
						split(players[0], players[i]); //we split them
						pntCrds(players[i].hand); //reprint their cards in case they forgot
						break; 
					case 'D':
						dblDown(players[0], players[i]); //they double down
						input = 'S'; //sets input to S since now they are done
						break;
					case 'H':
						players[i].hand.push_back(hitMe()); //we give them one more card for their hit
						pntCrds(players[i].hand); //reprint their cards
						cout << players[i].info.user << " score is now " << score(players[i].hand) << endl; //reprint their score
						break;
					default: //this is here for people can't follow directions
						input = 's'; //input is S
					}
					if(score(players[i].hand) > 21){ //if they bust they are done
						input = 'S'; //so we can quit
					}
			}
		}while(!(input == 'S' || input == 's')); //we continue doing this until they are want to stay
	}

	deal_ply(players[0]); //now the dealer plays

	players[0].hand[0].up = true; //now the everybody can see the first card
	
	
	//The below method shows everybody's score and cards including dealers
	
	for(int i =0;i<players.size();i++){
		cout << players[i].info.user << " score: " << score(players[i].hand) << " Cards: ";
		pntCrds(players[i].hand);
	}

	
	//The below method pays everybody out
	
	for(int i =1;i<players.size();i++){
		if(score(players[i].hand) > 21){ //if the player busted we tell them
			cout << "You busted! ";
		}
		int win = winner(players[0], players[i]); //we figure out who wins
		if(win == 1){
			players[i].info.wins += 1; //if the player wins it adds one to their win record
		}
		payout(players[0],players[i]); //payout everybody
		clear(players[i].hand); //we clear out their hands
		players[i].info.total_played+=1; //adds one to the total played	
	}	

	clear(players[0].hand); //clear out the dealers hand
}

//The below method clears out a players hand for the next game

void clear(vector<card> &hand){
	hand.clear();
}

//Creates new accounts for new players
