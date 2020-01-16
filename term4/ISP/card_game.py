import random
import os


class GameData:
    RANKS = ['7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    SUITS = ['♠', '♣', '♦', '♥']
    RED = "\033[1;31m"
    BLACK = "\033[1;30m"
    WHITE = "\033[0m"


class Card:
    def __init__(self, rank, suit):
        self.suit = GameData.RED if suit == '♦' or suit == '♥' else GameData.BLACK
        self.suit += suit + GameData.WHITE
        self.rank = rank

    def __str__(self):
        return self.rank + self.suit

    def __repr__(self):
        return self.__str__()

    def __eq__(self, value):
        return (self.rank, self.suit) == (value.rank, value.suit)

    def can_beat(self, value):
        return self.rank == value.rank or self.suit == value.suit

    def show_card(self):
        lines = [[] for i in range(7)]
        lines[0].append('┌─────────┐')
        lines[1].append('│{:>2s}{}      │'.format(self.rank, self.suit))
        lines[2].append('│         │')
        lines[3].append('│    {}    │'.format(self.suit))
        lines[4].append('│         │')
        lines[5].append('│      {}{:2s}│'.format(self.suit, self.rank))
        lines[6].append('└─────────┘')
        result = []
        result = [''.join(line) for line in lines]
        print('\n'.join(result))


class Player:
    def __init__(self, cards):
        self.cards = [Card(card[:-1], card[-1]) for card in cards]

    def make_a_move(self, choice):
        try:
            result = self.cards[choice - 1]
            self.cards.remove(result)
            return result
        except IndexError:
            return None

    def show_cards(self):
        cardd = ''
        for i, card in enumerate(self.cards):
            if not (i + 1) % 4:
                cardd += f"({i+1}) {card}"
                print(cardd)
                cardd = ''
            else:
                cardd += f"({i+1}) {card} "
        print(cardd)


class Skynet:
    def __init__(self, cards):
        self.cards = [Card(card[:-1], card[-1]) for card in cards]

    def make_a_move(self, human_turn):
        if not human_turn:
            result = random.choice(self.cards)
            self.cards.remove(result)
            return result
        else:
            cards_list = list(filter(lambda x: x.can_beat(human_turn), self.cards))
            if not cards_list:
                return None
            else:
                result = random.choice(cards_list)
                self.cards.remove(result)
                return result


def deck_init():
    deck = []
    for suit in GameData.SUITS:
        for rank in GameData.RANKS:
            deck.append(rank + suit)
    return deck


def player_choice(player):
    print('\nYour cards:\n')
    player.show_cards()
    print('\nChoose a card:')
    try:
        choice = int(input())
        return choice
    except ValueError:
        os.system("clear")
        print('\nInvalid input...\n')
        return None


def main():
    player_cards = ['7♠', '9♠', 'J♠', 'K♠', '8♣', '10♣', 'Q♣',
                    'A♣', '7♦', '9♦', 'J♦', 'K♦', '8♥', '10♥', 'Q♥', 'A♥']
    deck = deck_init()

    player = Player(player_cards)
    for card in player_cards:
        deck.remove(card)
    ai = Skynet(deck)

    player_last_turn = None
    player_first = None
    ai_last_turn = None
    ai_first = None

    print('\nWelcome to The Card Game\nYou\'re playing against better player - AI player.\n')
    
    while True:
        print("\nFirst turn whould make:\n1)AI\n2)Player\n")
        decision = input()
        if decision == '1':
            os.system("clear")
            ai_first = ai.make_a_move(player_first)
            break
        elif decision == '2':
            choice = player_choice(player)
            if not choice:
                continue
            os.system("clear")
            player_first = player.make_a_move(choice)
            break
        else:
            os.system("clear")
            print("\nTry again...\n")
            continue

    if ai_first:
        ai_last_turn = ai_first
    elif player_first:
        player_last_turn = player_first
        ai_last_turn = ai.make_a_move(player_last_turn)

    while True: 
        print('\nLast AI turn:\n')
        ai_last_turn.show_card()
        print('\nYour turn:\n1)Make a move\n2)Surrender\n')
        inpt = input()

        if inpt == '1':
            choice = player_choice(player)
            if not choice:
                continue
            player_last_turn = player.make_a_move(choice)

            os.system("clear")

            if player_last_turn.can_beat(ai_last_turn):
                print('\nGood turn...\n')
                ai_last_turn = ai.make_a_move(player_last_turn)
                if not ai_last_turn:
                    os.system("clear")
                    print("\nOMG, you win, its impossible :c")
                    break
                if player.cards == [] and ai.cards == []:
                    os.system("clear")
                    print('\nDraw 0_0')
                    break
            else:
                print('Bad turn, try again...')
                player.cards.append(player_last_turn)
                continue
        elif inpt == '2':
            print('HAHHA you\'re weak...\n')
            break


main()