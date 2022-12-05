// Citation: Modeled after : https://termspar.wordpress.com/2019/04/11/c-sfml-textbox-and-button-classes/
#ifndef searchBar_h
#define searchBar_h

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class searchBar {
public:
    searchBar(int size, sf::Color color, bool selected);
    void setSelected(bool sel);
    string getText();
    void drawTo(sf::RenderWindow &window);
    void typedOn(sf::Event input);
    void setFont(sf::Font& fonts);
    void setPosition(sf::Vector2f point);
private:
    sf::Text searchBox;
    ostringstream text;
    bool isSelected = false;
    void deleteLastChar();
};

searchBar::searchBar(int size, sf::Color color, bool selected) {
    searchBox.setCharacterSize(35);
    searchBox.setFillColor(sf::Color::Black);
    setSelected(selected);

    // Check if the textbox is selected upon creation and display it accordingly:
    if(isSelected) {
        searchBox.setString("_");
    } else {
        searchBox.setString("");
    }
}

void searchBar::setFont(sf::Font& fonts) {
    searchBox.setFont(fonts);
}

void searchBar::setPosition(sf::Vector2f point) {
    searchBox.setPosition(300, 150);
}

// Change selected state:
void searchBar::setSelected(bool sel) {
    isSelected = sel;

    // If not selected, remove the '_' at the end:
    if (!sel) {
        string type = text.str();
        string newT = "";
        for (int i = 0; i < type.length(); i++) {
            newT += type[i];
        }
        searchBox.setString(newT);
    }
}

string searchBar::getText() {
    return text.str();
}

void searchBar::drawTo(sf::RenderWindow &window) {
    window.draw(searchBox);
}

// Function for event loop: receiving user input
void searchBar::typedOn(sf::Event input) {
    if (isSelected) {
        int charTyped = input.text.unicode;

        // If the key pressed isn't delete, or the two selection keys, then append the text with the char:
        if (charTyped != 8 && charTyped != 13) {
            text << static_cast<char>(charTyped);
        }
        // If the key is delete, then delete the char:
        else if (charTyped == 8) {
            if (text.str().length() > 0) {
                deleteLastChar();
            }
        }
        
        searchBox.setString(text.str() + "_");
    }
}

// Delete the last character of the text:
void searchBar::deleteLastChar() {
    string typed = text.str();
    string newText = "";
    for (int i = 0; i < typed.length() - 1; i++) {
        newText += typed[i];
    }
    
    // reset the string text
    text.str("");
    text << newText;
    searchBox.setString(typed + "_");
}

#endif /* searchBar_h */
