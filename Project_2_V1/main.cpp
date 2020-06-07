/* 
 * File:   main.cpp
 * Author: Fredi Brizuela
 * Created on June 2, 2020 8:14 PM
 * Purpose: Revised Blackjack, overhaul Project 1 to meet requirements
 */

//System Libraries Here
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
//Adding more in order to allow new code like a random generator
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
int main(int argc, char** argv) {

    //Set Random Number seed
    srand(static_cast<unsigned int>(time(0)));//Setting up the random variable for cards
    
    //Declare Variable Data Types and Constants
    char card1,card2,card3,card4,deal1,deal2,deal3,total,dtotal;//The cards in play for random, and the total for each cards
    
    char play,hit,hit2,game; //The chars that are used if and when the game is played
    
    //Initialize Variables
    card1=rand()%11+1;//The range from 1 to 11 on possible cards
    card2=rand()%11+1;
    card3=rand()%11+1;
    card4=rand()%11+1;
    deal1=rand()%11+1;
    deal2=rand()%11+1;
    deal3=rand()%11+1;
    total=card1+card2;//total for both dealer and player
    dtotal=deal1+deal2;
    //Input or initialize values Here
    cout<<"This is Blackjack, would you like to play? Enter Y/N"<<endl;
    cin>>play;
    game=1;//will change if on of the desired endings is met
    
    //Process/Calculations Here
    if (play == 'Y' or play == 'y'){//If the player chooses to play, it will display the cards give nto both the player and the dealer
        cout<<"You have drawn a "<<static_cast<int>(card1)<<" and a "<<static_cast<int>(card2)<<", making your total "<<static_cast<int>(total)<<endl; 
        cout<<"The dealer has drawn a "<<static_cast<int>(deal1)<<" and a "<<static_cast<int>(deal2)<<", making their total "<<static_cast<int>(dtotal)<<endl; 
        if (total == 21 & dtotal != 21){//If the player gets 21, the game ends
            cout<<"You win!";
            game=0;
        }
        if (dtotal == 21 & total != 21){//If the Dealer gets 21, the game ends
            cout<<"You Lose!";
            game=0;
        }
        if (total<=18 & game!=0){//if the player wishes to draw another card
            cout<<"Would you like to draw a card? Enter Y/N"<<endl;
            cin>>hit;
            if (hit == 'Y' or hit == 'y'){//if the player says yes, the player will get another card
                total=card1+card2+card3;
                cout<<"You have drawn a "<<static_cast<int>(card3)<<", making your total "<<static_cast<int>(total)<<endl;
                if (total<=18 & game!=0){
                    cout<<" Would you like to draw another card? Enter Y/N"<<endl;
                    cin>>hit2;
                    if (hit2 == 'Y' or hit2 == 'y'){//same as the prior code for another card
                        total=card1+card2+card3+card4;
                        cout<<"You have drawn a "<<static_cast<int>(card4)<<", making your total "<<static_cast<int>(total)<<endl;
                        if (dtotal<=16 & game!=0){//If the game has not ended, the dealer can draw another card
                            dtotal=deal1+deal2+deal3;
                            cout<<"The dealer has drawn a "<<static_cast<int>(deal3)<<", making their total "<<static_cast<int>(dtotal)<<endl;
                            }
                        }
                    
                    }
                }
            else {//if the player refuses to draw a card, it continues
                if (dtotal<=16 & game!=0){//allows dealer to draw another card
            dtotal=deal1+deal2+deal3;
            cout<<"The dealer has drawn a "<<static_cast<int>(deal3)<<", making their total "<<static_cast<int>(dtotal)<<endl;
            }
            }
        }

        
        if (dtotal > 21 & total <dtotal ){//if the player has a higher number than the dealer without surpassing 21, the game ends
            cout<<"You Win!";
            game=0;
        }
        if (total > 21 & dtotal <total ){//if the dealer has a higher number than the player without surpassing 21, the game ends
            cout<<"You Lose!";
           game=0;
        }
        if (total < dtotal & dtotal < 21 ){//if the player has a lower number than the dealer  without surpassing 21, the game ends
            cout<<"You Lose!";
            game=0;
        }
        if (dtotal < total & total < 21 ){//if the dealer has a lower number than the player without surpassing 21, the game ends
            cout<<"You Win!";
            game=0;
        }
        if (total == 21 & dtotal != 21){//if the player gets 21, the game wins
            cout<<"You win!";
        }
        if (dtotal == 21 & total != 21){//if the dealer gets 21, the game ends
            cout<<"You Lose!";
        }
        if (dtotal == total ){//if the dealer and the player are tied, it ends the game  in a draw
            cout<<"You've Tied!";
        }
    }
    
    else {//if the player selects n or anything other than Y or y, the game ends
        return 0;
    }
    
    //Exit
}